#ifndef ESP32

#include "EDIpcProtocolSlave.h"


EDIpcProtocolSlave* EDIpcProtocolSlave::instance = nullptr;

EDIpcProtocolSlave::EDIpcProtocolSlave(TwoWire *wire)
{
    _wire = wire;
    instance = this;
    _joystick = new Joystick_(JOYSTICK_DEFAULT_REPORT_ID, 
        JOYSTICK_TYPE_MULTI_AXIS, 0, 0,
        true, true, true, true, true, true,
        false, false, false, false, false);
    _currentRequestType = mREQUEST_TYPE::NONE;
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
}

void EDIpcProtocolSlave::_handleRequest()
{
    size_t l;
    char buffer[100];

    switch (_currentRequestType) {
        case mREQUEST_TYPE::PING_SLAVE:
            Wire.print(F("Pong"));
            Wire.write(0);
            _currentRequestType = mREQUEST_TYPE::NONE;
            break;       
        default:
            break;
    }
}

void EDIpcProtocolSlave::_handleReceivedData(int numBytes)
{
    int requestVal = _wire->read();
    _currentRequestType = static_cast<mREQUEST_TYPE>(requestVal);

    switch(_currentRequestType) {
        case mREQUEST_TYPE::NONE:
            break;
        case mREQUEST_TYPE::TRACKER_DATA:
            const size_t axisStructSize = sizeof(AxisStruct);
            uint8_t *dataPtr = reinterpret_cast<uint8_t*>(&_axis);
            size_t bytesRead = _wire->readBytes(dataPtr, axisStructSize);
            _hasAxisChanges = bytesRead == axisStructSize;
            break;
        case mREQUEST_TYPE::PING_SLAVE:        
            break;
        // default:
        //     break;
    }
}

#endif