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
    _updateFlag |= (uint32_t)flag;
}

void EDIpcProtocolSlave::clearUpdate(UPDATE_CATEGORY flag)
{
    _updateFlag &= ~ (uint32_t)(flag);    
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
        // Serial.println("L\tkey loop");
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
                    // Serial.print((int)item->key);
                }
                else
                    for (uint8_t c = 0; c < item->count; c++)
                    {
                        Keyboard.press(item->key);
                        delay(50);
                        // Serial.print("L\t");
                        // Serial.print((int)item->key);
                        // Serial.println();
                        Keyboard.release(item->key);
                        if (item->count > 1)
                            delay(50);
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
    // Serial.println("new key add to queue");
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
    if (responseSize + l > I2C_MAX_MESSAGE_SIZE)
    {
        Serial.println(F("L\tI2C MF Buffer overflow"));
        return;
    }
    memcpy((void *)(responseBuffer+responseSize), data, l);
    responseSize += l;
}

void EDIpcProtocolSlave::_writeTxBuffer(const __FlashStringHelper *ifsh)
{
    PGM_P p = reinterpret_cast<PGM_P>(ifsh);
    size_t n = 0;
    while (1) {
        unsigned char c = pgm_read_byte(p++);
        if (c == 0) break;
        if (responseSize + n >= I2C_MAX_MESSAGE_SIZE)
        {
            Serial.println(F("L\tI2C MF Buffer overflow"));
            return;
        }
        responseBuffer[responseSize+n] = c;
        n++;
    }
    responseSize += n;
}


void EDIpcProtocolSlave::_writeTxBuffer(uint8_t data)
{
    if (responseSize + 1 > I2C_MAX_MESSAGE_SIZE)
    {
        Serial.println(F("L\tI2C MF Buffer overflow"));
        return;
    }
    responseBuffer[responseSize++] = data;
}

void EDIpcProtocolSlave::_writeTxBuffer(uint16_t data)
{
    _writeTxBuffer((uint8_t *)(&data), 2);
}

void EDIpcProtocolSlave::_writeTxBuffer(uint32_t data)
{
    _writeTxBuffer((uint8_t *)(&data), 4);
}


void EDIpcProtocolSlave::_writeTxBuffer(uint8_t * data, uint8_t dataSize)
{
    if (responseSize + dataSize > I2C_MAX_MESSAGE_SIZE)
    {
        Serial.println(F("L\tI2C MF Buffer overflow"));
        return;
    }
    memcpy((void *)(responseBuffer+responseSize), data, dataSize);
    responseSize += dataSize;
}

void EDIpcProtocolSlave::_sendChunk(uint8_t chunkId)
{
    if (chunkId == 0) {
//        Serial.println(F("Received request, preparing response"));
        responseBuffer[responseSize++] = 0;
        // Serial.print(F("response = "));
        // Serial.println((char *)responseBuffer);
        // Serial.print(F("response size = "));
        // Serial.println(responseSize);
        chunkCount = (responseSize + (I2C_CHUNK_SIZE-1)) / I2C_CHUNK_SIZE;
//        Serial.print(F("ChunkCount = "));
//        Serial.println(chunkCount);
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
                // Serial.print("crc = ");
                // Serial.println(crcs[crcChunkId]);
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
    // Serial.print(F("Sending "));
    // Serial.println(chunkSize);
    // Serial.println((char *)(responseBuffer + (chunkId * I2C_CHUNK_SIZE)));
    Wire.write((uint8_t *)(responseBuffer + (chunkId * I2C_CHUNK_SIZE)), chunkSize);
}


void EDIpcProtocolSlave::_handleRequest()
{
    if (_currentRequestType == COM_REQUEST_TYPE::CRT_GET_UPDATES) {
        if (chunkId == 0) {
            _resetTxBuffer();
            _writeTxBuffer(_updateFlag);
        }
        _sendChunk(chunkId);
    } else if (_currentRequestType == COM_REQUEST_TYPE::CRT_GET_PING_SLAVE) {
        if (chunkId == 0) {
            _resetTxBuffer();
            _writeTxBuffer(F("Pong"));
        }
        _sendChunk(chunkId);
    } else if (_currentRequestType == COM_REQUEST_TYPE::CRT_GET_LOCATION) {
        if (chunkId == 0) {
            _resetTxBuffer();
            _writeTxBuffer(EDGameVariables.LocationSystemName);
            _writeTxBuffer("\t");
            _writeTxBuffer(EDGameVariables.LocationStationName);
        }
        _sendChunk(chunkId);
        _updateFlag = (_updateFlag & (~ (uint32_t)(UPDATE_CATEGORY::UC_LOCATION)));
    } else if (_currentRequestType == COM_REQUEST_TYPE::CRT_GET_SYSTEM_POLICY) {
        if (chunkId == 0) {
            _resetTxBuffer();
            _writeTxBuffer(EDGameVariables.LocalAllegiance);
            _writeTxBuffer("\t");
            _writeTxBuffer(EDGameVariables.SystemSecurity);
        }
        _sendChunk(chunkId);
        _updateFlag = (_updateFlag & (~ (uint32_t)(UPDATE_CATEGORY::UC_LOCATION)));
    } else if (_currentRequestType == COM_REQUEST_TYPE::CRT_GET_NAVROUTE) {
        if (chunkId == 0) {
            _resetTxBuffer();
            _writeTxBuffer(EDGameVariables.Navroute1);
            _writeTxBuffer("\t");
            _writeTxBuffer(EDGameVariables.Navroute2);
            _writeTxBuffer("\t");
            _writeTxBuffer(EDGameVariables.Navroute3);
        }
        _sendChunk(chunkId);
        _updateFlag = (_updateFlag & (~ (uint32_t)(UPDATE_CATEGORY::UC_LOCATION)));
    } else if (_currentRequestType == COM_REQUEST_TYPE::CRT_GET_INFOS) {
        if (chunkId == 0) {
            _resetTxBuffer();
            _writeTxBuffer(EDGameVariables.InfosCommanderName);
            _writeTxBuffer("\t");
            _writeTxBuffer(EDGameVariables.InfosShipName);
        }
        _sendChunk(chunkId);
        _updateFlag = (_updateFlag & (~ (uint32_t)(UPDATE_CATEGORY::UC_INFOS)));
    } else if (_currentRequestType == COM_REQUEST_TYPE::CRT_GET_URGENT_INFO) {
        if (chunkId == 0) {
            _resetTxBuffer();
            _writeTxBuffer(EDGameVariables.AlertDuration);
            _writeTxBuffer(EDGameVariables.AlertMessageTitle);
            _writeTxBuffer("\t");
            _writeTxBuffer(EDGameVariables.AlertMessage1);
            _writeTxBuffer("\t");
            _writeTxBuffer(EDGameVariables.AlertMessage2);
            _writeTxBuffer("\t");
            _writeTxBuffer(EDGameVariables.AlertMessage3);
        }
        _sendChunk(chunkId);
        _updateFlag = (_updateFlag & (~ (uint32_t)(UPDATE_CATEGORY::UC_URGENT_INFO)));
    } else if (_currentRequestType == COM_REQUEST_TYPE::CRT_GET_STATUS) {
        if (chunkId == 0) {
            _resetTxBuffer();
            _writeTxBuffer(EDGameVariables.StatusFlags1);
            _writeTxBuffer(EDGameVariables.StatusFlags2);
            _writeTxBuffer(EDGameVariables.GuiFocus);
            _writeTxBuffer(EDGameVariables.FireGroup);
            _writeTxBuffer(EDGameVariables.StatusLegal);
        }
        _sendChunk(chunkId);
        _updateFlag = (_updateFlag & (~ (uint32_t)(UPDATE_CATEGORY::UC_STATUS)));
    } else if (_currentRequestType == COM_REQUEST_TYPE::CRT_GET_LOADOUT) {
        if (chunkId == 0) {
            _resetTxBuffer();
            _writeTxBuffer(EDGameVariables.LoadoutFlags1);
            _writeTxBuffer(EDGameVariables.LoadoutFlags2);
        }
        _sendChunk(chunkId);
        _updateFlag = (_updateFlag & (~ (uint32_t)(UPDATE_CATEGORY::UC_STATUS)));
    } else if (_currentRequestType == COM_REQUEST_TYPE::CRT_MOCK) {
        if (chunkId == 0) {
            _resetTxBuffer();
            _writeTxBuffer(F("LocationSystemName\t"));
            _writeTxBuffer(F("LocationStationName\t"));
            _writeTxBuffer(F("LocalAllegiance\t"));
            _writeTxBuffer(F("SystemSecurity\t"));
            _writeTxBuffer(F("Navroute1\t"));
            _writeTxBuffer(F("Navroute2\t"));
            _writeTxBuffer(F("Navroute3"));
        }
        _sendChunk(chunkId);
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
        else if (_currentRequestType == COM_REQUEST_TYPE::CRT_GET_LOADOUT)
        {
            // Serial.println("L\tLocation request from master");
        }
        else if (_currentRequestType == COM_REQUEST_TYPE::CRT_MOCK)
        {
        }
    }
}

#endif