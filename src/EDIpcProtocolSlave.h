#pragma once

#include "EDIpcProtocol.h"
#ifndef ESP32

#include <Keyboard.h>
#include <Joystick.h>
#include <I2CDevice.h>
#include <CRC8.h>

class EDIpcProtocolSlave {

public:
    static EDIpcProtocolSlave* instance;

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
    uint8_t chunkId = 0;
    uint8_t responseBuffer[I2C_MAX_MESSAGE_SIZE];
    uint8_t responseSize;
    CRC8 sendcrc;
    uint8_t crcs[8];
    uint8_t chunkCount = 0;

    void _handleRequest();
    void _handleReceivedData(int numBytes);
    void _addKeyEventToQueue(KeyEvent* keyEvent);
    void _resetTxBuffer();
    void _writeTxBuffer(const char *data);
    void _writeTxBuffer(const __FlashStringHelper *ifsh);
    void _writeTxBuffer(uint8_t data);
    void _writeTxBuffer(uint8_t *data, uint8_t dataSize);
    void _sendChunk(uint8_t chunkId);
    void _processKeyEventQueue();
};

#endif