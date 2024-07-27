#pragma once

#include "EDIpcProtocol.h"
#ifndef ESP32

#include <Keyboard.h>
#include <Joystick.h>

class EDIpcProtocolSlave {

public:
    static EDIpcProtocolSlave* instance;

    char LocationSystemName[21];
    char LocationStationName[21];

    EDIpcProtocolSlave(TwoWire* wire, uint8_t signalMasterPin);
    bool begin();
    static void HandleRequest();
    static void HandleReceivedData(int numBytes);
    void updateDevices();
    void addUpdate(UPDATE_CATEGORY flag);
    void signalMaster();
    void reset();
    uint8_t _updateFlag = 0;

protected:
    TwoWire* _wire;
    COM_REQUEST_TYPE _currentRequestType;
    AxisStruct _axis;
    bool _hasAxisChanges;
    Joystick_* _joystick;
    uint8_t _signalMasterPin;
    void _handleRequest();
    void _handleReceivedData(int numBytes);
    void _addKeyEventToQueue(KeyEvent* keyEvent);
    void _processKeyEventQueue();
};

#endif