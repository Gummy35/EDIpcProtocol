#pragma once
#ifdef ESP32
#include "EDIpcProtocol.h"
#include "I2CDevice.h"

#define KEY_EVENT_QUEUE_SIZE 10

class EDIpcProtocolMaster {

public:
    static EDIpcProtocolMaster* instance;


    char LocationSystemName[21];
    char LocationStationName[21];

    EDIpcProtocolMaster(I2CDevice* comMcu);
    bool begin();
    void setAxis(int32_t x, int32_t y, int32_t z, int32_t rx, int32_t ry, int32_t rz);
    void sendChanges();
    uint8_t retrieveChanges();
    bool pingSlave();
    bool resetSlave();
    bool sendKey(KeyEvent event);

protected:
    I2CDevice* comMcu;
    AxisStruct _axis;
    bool _hasAxisChanges;    
    bool _sendAxisData();
    bool _sendKeyData(KeyEvent* keyEvent);
    bool _getGameFlags();
    bool _getGameInfo();
    bool _getKeypadConfig();
    bool _getWifiCredentials();
    bool _getLocationData();

    unsigned long _lastUpdate = 0;
};

#endif