#pragma once

#include "EDIpcProtocol.h"
#ifndef ESP32

#include <Keyboard.h>
#include <Joystick.h>

class EDIpcProtocolSlave {

public:
    static EDIpcProtocolSlave* instance;

    EDIpcProtocolSlave(TwoWire* wire);
    bool begin();
    static void HandleRequest();
    static void HandleReceivedData(int numBytes);
    void updateDevices();

protected:
    TwoWire* _wire;
    COM_REQUEST_TYPE _currentRequestType;
    AxisStruct _axis;
    bool _hasAxisChanges;
    Joystick_* _joystick;

    void _handleRequest();
    void _handleReceivedData(int numBytes);
    void _addKeyEventToQueue(KeyEvent* keyEvent);
    void _processKeyEventQueue();
};

#endif