#ifndef ESP32

#include <I2CDevice.h>
#include "EDIpcProtocolSlave.h"
#include <EDGameVariables.h>
#include <CRC8.h>

#define KEY_EVENT_QUEUE_SIZE 10

KeyEvent *_keyQueue[KEY_EVENT_QUEUE_SIZE];
volatile int _keyQueueHead = 0;
volatile int _keyQueueTail = 0;
volatile int _keyQueueCount = 0;

EDIpcProtocolSlave *EDIpcProtocolSlave::instance = nullptr;

EDIpcProtocolSlave::EDIpcProtocolSlave(TwoWire *wire, uint8_t signalMasterPin)
{
    _wire = wire;
    _signalMasterPin = signalMasterPin;
    instance = this;
    _joystick = new Joystick_(JOYSTICK_DEFAULT_REPORT_ID,
                              JOYSTICK_TYPE_MULTI_AXIS, 0, 0,
                              true, true, true, true, true, true,
                              false, false, false, false, false);
    _currentRequestType = COM_REQUEST_TYPE::CRT_NONE;
    _hasAxisChanges = false;
}

void EDIpcProtocolSlave::reset()
{
    _wire->flush();
    _wire->end();
    _wire->begin(0x12);
}

bool EDIpcProtocolSlave::begin()
{    
    _wire->onReceive(HandleReceivedData);
    _wire->onRequest(HandleRequest);

    _joystick->setXAxisRange(-127, 127);
    _joystick->setYAxisRange(-127, 127);
    _joystick->setZAxisRange(-127, 127);
    _joystick->setRxAxisRange(-127, 127);
    _joystick->setRyAxisRange(-127, 127);
    _joystick->setRzAxisRange(-127, 127);
    _joystick->begin(false);

    Keyboard.begin();

    return true;
}

// function that executes whenever data is requested by master
void EDIpcProtocolSlave::HandleRequest()
{
    instance->_handleRequest();
}

// function that executes whenever data is received by master
void EDIpcProtocolSlave::HandleReceivedData(int numBytes)
{
    instance->_handleReceivedData(numBytes);
}

void EDIpcProtocolSlave::updateDevices()
{
    if (_hasAxisChanges)
    {
        _joystick->setRxAxis(_axis.rx);
        _joystick->setRyAxis(_axis.ry);
        _joystick->setRzAxis(_axis.ry);
        _joystick->setXAxis(_axis.x);
        _joystick->setYAxis(_axis.y);
        _joystick->setZAxis(_axis.z);
        _joystick->sendState();
        _hasAxisChanges = false;
    }
    _processKeyEventQueue();
}

void EDIpcProtocolSlave::addUpdate(UPDATE_CATEGORY flag)
{
    _updateFlag |= (uint8_t)flag;
}

void EDIpcProtocolSlave::signalMaster()
{
    digitalWrite(_signalMasterPin, LOW);
    delay(10);
    digitalWrite(_signalMasterPin, HIGH);
}

void EDIpcProtocolSlave::_processKeyEventQueue()
{
    while (_keyQueueCount > 0)
    {
        Serial.println("key loop");
        noInterrupts();
        KeyEvent *item = _keyQueue[_keyQueueHead];
        _keyQueueHead = (_keyQueueHead + 1) % KEY_EVENT_QUEUE_SIZE;
        _keyQueueCount--;
        interrupts();

        // process key event

        if (item->key != 0)
        {
            if (item->pressed)
            {
                if (item->count == 0)
                {
                    Keyboard.press(item->key);
                    Serial.print((int)item->key);
                }
                else
                    for (uint8_t c = 0; c < item->count; c++)
                    {
                        Keyboard.press(item->key);
                        Serial.print((int)item->key);
                        Keyboard.release(item->key);
                    }
            }
            else
            {
                Keyboard.release(item->key);
            }
        }
        delete item;
    }
}

void EDIpcProtocolSlave::_addKeyEventToQueue(KeyEvent *keyEvent)
{
    // Désactive les interruptions pour assurer la sécurité des données
    Serial.println("new data");
    noInterrupts();
    if (_keyQueueCount < KEY_EVENT_QUEUE_SIZE)
    {
        _keyQueue[_keyQueueTail] = keyEvent;
        _keyQueueTail = (_keyQueueTail + 1) % KEY_EVENT_QUEUE_SIZE;
        _keyQueueCount++;
    }
    else
    {
        // queue full, ignore
    }
    // Réactive les interruptions
    interrupts();
}


void EDIpcProtocolSlave::_resetTxBuffer()
{
    responseSize = 0;
    memset(responseBuffer, 0, I2C_MAX_MESSAGE_SIZE);
    memset(crcs, 0, 8);
    chunkId = 0;
    chunkCount = 0;
}

void EDIpcProtocolSlave::_writeTxBuffer(const char * data)
{
    uint8_t l = strlen(data);
    memcpy((void *)(responseBuffer+responseSize), data, l);
    responseSize += l;
}

void EDIpcProtocolSlave::_writeTxBuffer(uint8_t * data, uint8_t dataSize)
{
    memcpy((void *)(responseBuffer+responseSize), data, dataSize);
    responseSize += dataSize;
}

void EDIpcProtocolSlave::_sendChunk(uint8_t chunkId)
{
    if (chunkId == 0) {
        Serial.println("Received request, preparing response");
        responseBuffer[responseSize++] = 0;
        Serial.print("response = ");
        Serial.println((char *)responseBuffer);
        Serial.print("response size = ");
        Serial.println(responseSize);
        chunkCount = (responseSize + (I2C_CHUNK_SIZE-1)) / I2C_CHUNK_SIZE;
        Serial.print("ChunkCount = ");
        Serial.println(chunkCount);
        //Serial.printf("Response : %s, size %d, chunkcount %d\n", responseBuffer, responseSize, chunkcount);
        sendcrc.reset();
        sendcrc.setInitial(CRC8_INITIAL);
        uint8_t crcChunkId = 0;
        // calc crc for each chunk
        for (uint8_t i = 0; i < responseSize; i++)
        {
            sendcrc.add(responseBuffer[i]);
            if ((((i + 1) % I2C_CHUNK_SIZE) == 0) || (i == (responseSize - 1)))
            {
                crcs[crcChunkId] = sendcrc.calc();
                sendcrc.reset();
                sendcrc.setInitial(crcs[crcChunkId]);
                Serial.print("crc = ");
                Serial.println(crcs[crcChunkId]);
                //Serial.printf("byte %d, crc#%d = %d\n", i, crcChunkId, crcs[crcChunkId]);
                crcChunkId++;
            }                        
        }
    }

    if (chunkId >= chunkCount) return;

    // 0 signature
    Wire.write((uint8_t) 255);
    // 1 chunkCount / chunkId
    if (chunkId == 0)
        //Wire.write((uint8_t) chunkCount);
        Wire.write((uint8_t) responseSize);
    else
        Wire.write((uint8_t) chunkId);
    // 2 chunk's crc
    Wire.write((uint8_t) crcs[chunkId]);

    // 3.. chunk data
    uint8_t chunkSize = (chunkId < (chunkCount - 1)) ? I2C_CHUNK_SIZE :  responseSize % I2C_CHUNK_SIZE;
    Serial.print("Sending ");
    Serial.println(chunkSize);
    Serial.println((char *)(responseBuffer + (chunkId * I2C_CHUNK_SIZE)));
    Wire.write((uint8_t *)(responseBuffer + (chunkId * I2C_CHUNK_SIZE)), chunkSize);
    //memcpy(receiveBuffer + 3, (void *)(responseBuffer + (mockChunkId * I2C_CHUNK_SIZE)), chunkSize);
    //Serial.printf("Sending [0]=%d, [1]=%d, [2]=%d, size=%d, data=%s\n", receiveBuffer[0], receiveBuffer[1], receiveBuffer[2], chunkSize, receiveBuffer+3);
    //return chunkSize + 3;
}


void EDIpcProtocolSlave::_handleRequest()
{
//    size_t l;
//    char buffer[100];

    if (_currentRequestType == COM_REQUEST_TYPE::CRT_GET_UPDATES) {
        Wire.write(_updateFlag);
        _currentRequestType = COM_REQUEST_TYPE::CRT_NONE;
    } else if (_currentRequestType == COM_REQUEST_TYPE::CRT_GET_PING_SLAVE) {
        Wire.print(F("Pong"));
        Wire.write(0);
        _currentRequestType = COM_REQUEST_TYPE::CRT_NONE;
    } else if (_currentRequestType == COM_REQUEST_TYPE::CRT_GET_LOCATION) {
        Wire.print(EDGameVariables.LocationSystemName);
        Wire.print('\t');
        Wire.print(EDGameVariables.LocationStationName);
        Wire.write(0);
        _updateFlag = (_updateFlag & (~ (uint8_t)(UPDATE_CATEGORY::UC_LOCATION)));
        _currentRequestType = COM_REQUEST_TYPE::CRT_NONE;
    } else if (_currentRequestType == COM_REQUEST_TYPE::CRT_GET_SYSTEM_POLICY) {
        Wire.print(EDGameVariables.LocalAllegiance);
        Wire.print('\t');
        Wire.print(EDGameVariables.SystemSecurity);
        Wire.write(0);
        _updateFlag = (_updateFlag & (~ (uint8_t)(UPDATE_CATEGORY::UC_LOCATION)));
        _currentRequestType = COM_REQUEST_TYPE::CRT_NONE;        
    } else if (_currentRequestType == COM_REQUEST_TYPE::CRT_GET_NAVROUTE) {
        Wire.print(EDGameVariables.Navroute1);
        Wire.print('\t');
        Wire.print(EDGameVariables.Navroute2);
        Wire.print('\t');
        Wire.print(EDGameVariables.Navroute3);
        Wire.write(0);
        _updateFlag = (_updateFlag & (~ (uint8_t)(UPDATE_CATEGORY::UC_LOCATION)));
        _currentRequestType = COM_REQUEST_TYPE::CRT_NONE;        
    } else if (_currentRequestType == COM_REQUEST_TYPE::CRT_GET_INFOS) {
        Wire.print(EDGameVariables.InfosCommanderName);
        Wire.print('\t');
        Wire.print(EDGameVariables.InfosShipName);
        Wire.write(0);
        _updateFlag = (_updateFlag & (~ (uint8_t)(UPDATE_CATEGORY::UC_INFOS)));
        _currentRequestType = COM_REQUEST_TYPE::CRT_NONE;
    } else if (_currentRequestType == COM_REQUEST_TYPE::CRT_GET_STATUS) {
        Wire.write((uint8_t *)(&EDGameVariables.StatusFlags1), 4);
        Wire.write((uint8_t *)(&EDGameVariables.StatusFlags2), 4);        
        Wire.print(EDGameVariables.StatusLegal);
        Wire.write(0);
        _updateFlag = (_updateFlag & (~ (uint8_t)(UPDATE_CATEGORY::UC_STATUS)));
        _currentRequestType = COM_REQUEST_TYPE::CRT_NONE;
    } else if (_currentRequestType == COM_REQUEST_TYPE::CRT_MOCK) {
        if (chunkId == 0) {
            _resetTxBuffer();
            _writeTxBuffer("LocationSystemName\t");
            _writeTxBuffer("LocationStationName\t");
            _writeTxBuffer("LocalAllegiance\t");
            _writeTxBuffer("SystemSecurity\t");
            _writeTxBuffer("Navroute1\t");
            _writeTxBuffer("Navroute2\t");
            _writeTxBuffer("Navroute3");
        }
        _sendChunk(chunkId);
    // } else if (_currentRequestType == COM_REQUEST_TYPE::CRT_MOCK) {
    //     if (chunkId == 0) {
    //         _resetTxBuffer();
    //         uint8_t d[10];
    //         for(int i=0;i<10;i++)
    //             d[i] = i;
    //         _writeTxBuffer(d, 10);
    //         _writeTxBuffer(d, 10);
    //         _writeTxBuffer(d, 10);
    //         _writeTxBuffer(d, 10);
    //         _writeTxBuffer(d, 10);
    //         _writeTxBuffer(d, 10);
    //         _writeTxBuffer(d, 10);
    //         _writeTxBuffer(d, 10);
    //         _writeTxBuffer(d, 10);
    //         _writeTxBuffer(d, 10);
    //         _writeTxBuffer("End");
    //     }
    //     _sendChunk(chunkId);
    }
}

void(* resetFunc) (void) = 0;


void EDIpcProtocolSlave::_handleReceivedData(int numBytes)
{
    int requestVal = _wire->read();
    uint8_t *dataPtr;
    size_t bytesRead;
    _currentRequestType = static_cast<COM_REQUEST_TYPE>((uint8_t)requestVal);
    if (_currentRequestType == COM_REQUEST_TYPE::CRT_SEND_TRACKER_DATA)
    {
        Serial.println("tracker");
        const size_t axisStructSize = sizeof(AxisStruct);
        dataPtr = reinterpret_cast<uint8_t *>(&_axis);
        bytesRead = _wire->readBytes(dataPtr, axisStructSize);
        _hasAxisChanges = bytesRead == axisStructSize;
    } 
    else if (_currentRequestType == COM_REQUEST_TYPE::CRT_SEND_KEY_DATA)
    {
        KeyEvent *keyEvent = new KeyEvent();
        bytesRead = _wire->readBytes((uint8_t *)keyEvent, sizeof(KeyEvent));
        _addKeyEventToQueue(keyEvent);
    } 
    else if (_currentRequestType == COM_REQUEST_TYPE::CRT_SEND_REBOOT)
    {
        resetFunc();
    }
    else {
        if (numBytes > 1 )
        {
            chunkId = _wire->read();
            Serial.print("chunkId = ");
            Serial.println(chunkId);
        }
        if (_currentRequestType == COM_REQUEST_TYPE::CRT_GET_PING_SLAVE)
        {
            //Serial.println("L\tPing from master");
        }
        else if (_currentRequestType == COM_REQUEST_TYPE::CRT_GET_LOCATION)
        {
            // Serial.println("L\tLocation request from master");
        }
        else if (_currentRequestType == COM_REQUEST_TYPE::CRT_GET_INFOS)
        {
            // Serial.println("L\tLocation request from master");
        }
        else if (_currentRequestType == COM_REQUEST_TYPE::CRT_GET_STATUS)
        {
            // Serial.println("L\tLocation request from master");
        }
        else if (_currentRequestType == COM_REQUEST_TYPE::CRT_MOCK)
        {
        }
    }
}

#endif