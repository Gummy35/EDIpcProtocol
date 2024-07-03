#pragma once

#include "EDIpcProtocol.h"
#include "I2CDevice.h"

class EDIpcProtocolMaster {

public:
    static EDIpcProtocolMaster* instance;

    EDIpcProtocolMaster(I2CDevice* comMcu);
    bool begin();
    void setAxis(int32_t x, int32_t y, int32_t z, int32_t rx, int32_t ry, int32_t rz);
    void sendChanges();
    uint8_t retrieveChanges();
    bool pingSlave();

    WifiCredentials wifiCredentials;

protected:
    I2CDevice* comMcu;
    AxisStruct _axis;
    bool _hasAxisChanges;


    bool _sendAxisData();

    bool _getGameFlags();
    bool _getGameInfo();
    bool _getKeypadConfig();
    bool _getWifiCredentials();

    unsigned long _lastUpdate = 0;
};