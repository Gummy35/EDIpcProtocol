#ifndef ESP32

#include "EDIpcProtocolSlave.h"
#define KEY_EVENT_QUEUE_SIZE 10

KeyEvent* _keyQueue[KEY_EVENT_QUEUE_SIZE];
volatile int _keyQueueHead = 0;
volatile int _keyQueueTail = 0;
volatile int _keyQueueCount = 0;

EDIpcProtocolSlave* EDIpcProtocolSlave::instance = nullptr;

EDIpcProtocolSlave::EDIpcProtocolSlave(TwoWire *wire)
{
    _wire = wire;
    instance = this;
    _joystick = new Joystick_(JOYSTICK_DEFAULT_REPORT_ID, 
        JOYSTICK_TYPE_MULTI_AXIS, 0, 0,
        true, true, true, true, true, true,
        false, false, false, false, false);
    _currentRequestType = COM_REQUEST_TYPE::NONE;
    _hasAxisChanges = false;
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
    if (_hasAxisChanges) {
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

void EDIpcProtocolSlave::_processKeyEventQueue() {
    while (_keyQueueCount > 0) {
        noInterrupts();
        KeyEvent* item = _keyQueue[_keyQueueHead];
        _keyQueueHead = (_keyQueueHead + 1) % KEY_EVENT_QUEUE_SIZE;
        _keyQueueCount--;
        interrupts();

        // process key event

        if (item->key != 0) {            
            if (item->pressed) {
                if (item->count == 0)
                {
                    Keyboard.press(item->key);
                } 
                else for(uint8_t c = 0; c<item->count; c++) {
                    Keyboard.press(item->key);
                    Keyboard.release(item->key);
                }
            } else {
                Keyboard.release(item->key);
            }
        }
        delete item;
    }
}

void EDIpcProtocolSlave::_addKeyEventToQueue(KeyEvent* keyEvent) {
    // Désactive les interruptions pour assurer la sécurité des données
    noInterrupts();
    if (_keyQueueCount < KEY_EVENT_QUEUE_SIZE) {
        _keyQueue[_keyQueueTail] = keyEvent;
        _keyQueueTail = (_keyQueueTail + 1) % KEY_EVENT_QUEUE_SIZE;
        _keyQueueCount++;
    } else {
        // queue full, ignore
    }
    // Réactive les interruptions
    interrupts();
}

void EDIpcProtocolSlave::_handleRequest()
{
    size_t l;
    char buffer[100];

    switch (_currentRequestType) {
        case COM_REQUEST_TYPE::PING_SLAVE:
            Wire.print(F("Pong"));
            Wire.write(0);
            _currentRequestType = COM_REQUEST_TYPE::NONE;
            break;       
        default:
            break;
    }
}

void EDIpcProtocolSlave::_handleReceivedData(int numBytes)
{
    int requestVal = _wire->read();
    uint8_t *dataPtr;
    size_t bytesRead;
    _currentRequestType = static_cast<COM_REQUEST_TYPE>(requestVal);

    switch(_currentRequestType) {
        case COM_REQUEST_TYPE::NONE:
            break;
        case COM_REQUEST_TYPE::TRACKER_DATA:
            const size_t axisStructSize = sizeof(AxisStruct);
            dataPtr = reinterpret_cast<uint8_t*>(&_axis);
            bytesRead = _wire->readBytes(dataPtr, axisStructSize);
            _hasAxisChanges = bytesRead == axisStructSize;
            break;
        case COM_REQUEST_TYPE::KEY_DATA:
            KeyEvent* keyEvent = new KeyEvent();
            dataPtr = reinterpret_cast<uint8_t*>(&keyEvent);
            bytesRead = _wire->readBytes(dataPtr, sizeof(KeyEvent));
            _addKeyEventToQueue(keyEvent);
            break;
        case COM_REQUEST_TYPE::PING_SLAVE:        
            break;
        // default:
        //     break;
    }
}

#endif