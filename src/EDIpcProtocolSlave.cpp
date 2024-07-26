#ifndef ESP32

#include "EDIpcProtocolSlave.h"
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
    _currentRequestType = COM_REQUEST_TYPE::NONE;
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
    for(int i=0;i<20;i++)
    {
        LocationStationName[i] = 0;
        LocationSystemName[i] = 0;
    }

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

void EDIpcProtocolSlave::_handleRequest()
{
    size_t l;
    char buffer[100];

    if (_currentRequestType == COM_REQUEST_TYPE::GET_UPDATES) {
        Wire.write(_updateFlag);
        _currentRequestType = COM_REQUEST_TYPE::NONE;
    } else if (_currentRequestType == COM_REQUEST_TYPE::PING_SLAVE) {
        Wire.print(F("Pong"));
        Wire.write(0);
        _currentRequestType = COM_REQUEST_TYPE::NONE;
    } else if (_currentRequestType == COM_REQUEST_TYPE::GET_LOCATION) {
        Wire.println(LocationSystemName);
        Wire.println(LocationStationName);
        Wire.write(0);
        _updateFlag = (_updateFlag & (~ (uint8_t)(UPDATE_CATEGORY::LOCATION)));
        _currentRequestType = COM_REQUEST_TYPE::NONE;
    }
}

void EDIpcProtocolSlave::_handleReceivedData(int numBytes)
{
    int requestVal = _wire->read();
    uint8_t *dataPtr;
    size_t bytesRead;
    _currentRequestType = static_cast<COM_REQUEST_TYPE>((uint8_t)requestVal);
    if (_currentRequestType == COM_REQUEST_TYPE::TRACKER_DATA)
    {
        Serial.println("tracker");
        const size_t axisStructSize = sizeof(AxisStruct);
        dataPtr = reinterpret_cast<uint8_t *>(&_axis);
        bytesRead = _wire->readBytes(dataPtr, axisStructSize);
        _hasAxisChanges = bytesRead == axisStructSize;
    } 
    else if (_currentRequestType == COM_REQUEST_TYPE::KEY_DATA)
    {
        KeyEvent *keyEvent = new KeyEvent();
        bytesRead = _wire->readBytes((uint8_t *)keyEvent, sizeof(KeyEvent));
        _addKeyEventToQueue(keyEvent);
    } 
    else if (_currentRequestType == COM_REQUEST_TYPE::PING_SLAVE)
    {
        Serial.println("ping");
    }
    else if (_currentRequestType == COM_REQUEST_TYPE::GET_LOCATION)
    {
        Serial.println("Location");
    }
}

#endif