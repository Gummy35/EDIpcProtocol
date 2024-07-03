#pragma once

#include <IAbstractDevice.h>

class I2CDevice: public IAbstractDevice
{
public:
    I2CDevice(uint8_t deviceAddress, const char * name, uint8_t interruptPin);
    I2CDevice(uint8_t startAddress, uint8_t endAddress, const char * name, uint8_t interruptPin);
    virtual bool Detect(TwoWire *wire, TwoWire *wire2, uint8_t devId);
    virtual bool Check();
    virtual bool Init();
    TwoWire* beginTransmission();
    uint8_t endTransmission();
    uint8_t endTransmission(bool sendStop);

    bool sendMessageData(uint8_t messageId, uint8_t* messageData = nullptr, size_t dataSize = 0);
    bool getData(uint8_t messageId, uint8_t* receiveBuffer = nullptr, size_t receiveBufferSize = 0, bool strictSize = false, uint8_t* messageData = nullptr, size_t dataSize = 0);

	uint8_t ProbeStartAddr;
	uint8_t ProbeEndAddr;

	uint8_t Addr;
    TwoWire* Wire;
    uint8_t WireChannel;
    uint8_t InterruptPin;

    static std::function<void(char *logString)> Log;

    // See wire.endTransmission for standard Errors.
    // supplemental errors :
    // 16: Data requested, but device return data not available
    // 17: Response data size mismatch
};
