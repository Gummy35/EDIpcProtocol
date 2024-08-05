#pragma once

#define CHUNKID_IGNORE 255
#define I2C_MAX_PACKET_SIZE 32
#define I2C_CHUNK_SIZE (I2C_MAX_PACKET_SIZE-3)
#define I2C_MAX_TRIES 10
// 32 bytes pro micro I2C limitation is way too small, but don't oversize buffer. 128 bytes should be enough.
#define I2C_MAX_MESSAGE_SIZE 128

#ifdef ESP32
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

    virtual bool sendMessageData(uint8_t messageId, uint8_t* messageData = nullptr, size_t dataSize = 0, uint8_t chunkId = CHUNKID_IGNORE);
    virtual uint8_t requestData(uint8_t *receiveBuffer, size_t receiveBufferSize, bool strictSize = false);
    virtual uint8_t getData(uint8_t messageId, uint8_t *receiveBuffer = nullptr, size_t receiveBufferSize = 0, bool strictSize = false, uint8_t *messageData = nullptr, size_t dataSize = 0);

//    uint8_t getLargeData(uint8_t messageId, uint8_t *receiveBuffer = nullptr, size_t receiveBufferSize = 0, bool strictSize = false, uint8_t *messageData = nullptr, size_t dataSize = 0);

    uint8_t ProbeStartAddr;
	uint8_t ProbeEndAddr;

	uint8_t Addr;
    TwoWire* Wire;
    uint8_t WireChannel;
    uint8_t InterruptPin;

    // See wire.endTransmission for standard Errors.
    // supplemental errors :
    // 16: Data requested, but device return data not available
    // 17: Response data size mismatch
};
#endif