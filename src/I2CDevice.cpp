#ifdef ESP32
#include "I2CDevice.h"
#include "CrcParameters.h"
#include "CRC8.h"

// #define DEBUG_COM

#ifdef DEBUG_COM
#define debug(MyCode) MyCode;
#else
#define debug(MyCode) {}
#endif

CRC8 crc;

I2CDevice::I2CDevice(uint8_t startAddress, uint8_t endAddress, const char *name, uint8_t interruptPin)
{
    Addr = startAddress;
    ProbeStartAddr = startAddress;
    ProbeEndAddr = endAddress;
    Name = name;
    InterruptPin = digitalPinToInterrupt(interruptPin);
    IsPresent = false;
}

I2CDevice::I2CDevice(uint8_t deviceAddress, const char *name, uint8_t interruptPin) : I2CDevice(deviceAddress, deviceAddress, name, interruptPin)
{
}

bool I2CDevice::Detect(TwoWire *wire, TwoWire *wire2, uint8_t devId)
{
    char buffer[100];
    Serial.printf("probing %s on 0x%x-0x%x\n", Name, ProbeStartAddr, ProbeEndAddr);
    IsPresent = false;
    this->Wire = wire;
    for (Addr = ProbeStartAddr; Addr <= ProbeEndAddr; Addr++)
    {
        WireChannel = 0;
        Wire = wire;
        // Serial.printf("probing 0x%x, wire 1...\n", Addr);
        if (Check())
        {
            IsPresent = true;
            //    Serial.println("found");
        }
        else if (wire2)
        {
            //    Serial.printf("probing 0x%x, wire 2...\n", Addr);
            WireChannel = 1;
            Wire = wire2;
            IsPresent = Check();
            //    if (IsPresent) Serial.println("found");
        }
        if (IsPresent)
            break;
    }
    if (!IsPresent)
        Addr = -1;

    if (IsPresent)
    {
        DevId = devId;
        snprintf_P(buffer, 99, (const char *)F("Device #%d %s detected at 0x%x on channel %d"), DevId, Name, (byte)Addr, (byte)WireChannel);
    }
    else
    {
        snprintf_P(buffer, 99, (const char *)F("Couldn't find device %s"), Name);
    }
    Serial.println(buffer);

    return IsPresent;
}

bool I2CDevice::Check()
{
    beginTransmission();
    uint8_t ec = endTransmission(true);
    return ec == 0;
}

bool I2CDevice::Init()
{
    return true;
}

TwoWire *I2CDevice::beginTransmission()
{
    this->Wire->beginTransmission(Addr);
    return this->Wire;
}

uint8_t I2CDevice::endTransmission()
{
    LastError = this->Wire->endTransmission();
    if (LastError != 0)
    {
        this->Wire->clearWriteError();
        this->Wire->flush();
        this->Wire->end();
        this->Wire->begin();
    }
    return LastError;
}

uint8_t I2CDevice::endTransmission(bool sendStop)
{
    LastError = this->Wire->endTransmission(sendStop);
    return LastError;
}

bool I2CDevice::sendMessageData(uint8_t messageId, uint8_t *messageData, size_t dataSize, uint8_t chunkId)
{
    this->Wire->beginTransmission(Addr);
    this->Wire->write(messageId);
    if (chunkId < 255)
        this->Wire->write(chunkId);
    if (messageData != nullptr)
    {
        this->Wire->write(messageData, dataSize);
    }
    this->endTransmission();
    // Serial.printf("Wire send result = %d \n", LastError);

    return LastError == 0;
}

uint8_t I2CDevice::requestData(uint8_t *receiveBuffer, size_t receiveBufferSize, bool strictSize)
{
    uint8_t cnt = 0;
    this->Wire->requestFrom(Addr, receiveBufferSize);
    if (this->Wire->available())
    {
        //        Serial.printf("Zeroing %i bytes in buffer\n", dataSize);
        memset(receiveBuffer, 0, receiveBufferSize);
        while (this->Wire->available())
        {
            // Buffer exceeded, try to recover wire (read all buffer), and return an error
            if (cnt == receiveBufferSize)
            {
                while (this->Wire->available())
                {
                    this->Wire->read();
                    cnt++;
                }
                //                Serial.printf("Too many bytes : %i\n", cnt);
                LastError = 1;
                return 0;
            }
            //            Serial.printf("Reading byte\n");
            char c = this->Wire->read();
            //            Serial.printf("got %i\n", c);
            receiveBuffer[cnt++] = c;
        }
        if (strictSize && (cnt < receiveBufferSize))
        {
            //            Serial.printf("Size mismatch\n");
            LastError = 17;
            return 0;
        }
    }
    else
    {
        //        Serial.printf("No data available\n");
        LastError = 16;
        return 0;
    }
    return cnt;
}

// uint8_t I2CDevice::getData(uint8_t messageId, uint8_t *receiveBuffer, size_t receiveBufferSize, bool strictSize, uint8_t *messageData, size_t dataSize)
// {
//     //    Serial.printf("sending %i", messageId);
//     if (!sendMessageData(messageId, messageData, dataSize))
//         return false;
//     //    Serial.printf("Requesting %i bytes from %x\n", messageId, Addr);
//     return requestData(receiveBuffer, receiveBufferSize, strictSize);
// }

bool checkCRC(uint8_t initial, uint8_t expected, uint8_t* data, uint8_t size)
{
    crc.reset();
    crc.setInitial(initial);
    crc.add(data, size);
    uint8_t r = crc.calc();
    debug(Serial.printf("Checking crc : Initial=%d, expected=%d, size=%d, result=%d, data=%s\n", initial, expected, size, r, data))
    return expected == r;
}

uint8_t chunkBuffer[I2C_MAX_PACKET_SIZE];

uint8_t I2CDevice::getData(uint8_t messageId, uint8_t *receiveBuffer, size_t receiveBufferSize, bool strictSize, uint8_t *messageData, size_t dataSize)
{
    uint8_t chunkCount = 0;
    uint8_t chunkId = 0;
    uint8_t lastCrc = CRC8_INITIAL;
    uint8_t index = 0;
    uint8_t size;
    uint8_t end;
    uint8_t receivedSize;
    uint8_t tries = 0;
    uint8_t expectedSize = 0;
    uint8_t receivedDataSize = 0;
    bool hasSignature = false;
    memset(receiveBuffer, 0, receiveBufferSize);

    do
    {
        debug(Serial.printf("Requesting messageId %d, chunkId %d\n", messageId, chunkId))
        if (!sendMessageData(messageId, messageData, dataSize, chunkId))
            return 0;
        debug(Serial.println("Request accepted"))
        //size = ((receiveBufferSize - index) + 3) % I2C_MAX_PACKET_SIZE;        
        // if (size == 0) size = I2C_MAX_PACKET_SIZE;
        size = I2C_MAX_PACKET_SIZE;

        debug(Serial.printf("Requesting %d bytes at index %d\n", size, index))
        memset(chunkBuffer, 0, I2C_MAX_PACKET_SIZE);
        receivedSize = requestData(chunkBuffer, size);
        debug(Serial.printf("Received %d bytes\n", receivedSize))
        if (receivedSize > 0)
        {
            hasSignature = chunkBuffer[0] == 255;

            if (chunkId == 0)
            {
                // first block and doesn't have signature ? this is not a chunked data. Unsafe, for specific use only
                if (!hasSignature) {
                    debug(Serial.println("No signature, single frame data"))
                    memcpy(receiveBuffer, chunkBuffer, receiveBufferSize);
                    return receiveBufferSize;
                }

                //chunkCount = chunkBuffer[1];
                expectedSize = chunkBuffer[1];
                chunkCount = (expectedSize + (I2C_CHUNK_SIZE-1)) / I2C_CHUNK_SIZE;
            }
            else
                chunkId = chunkBuffer[1];

            receivedDataSize = expectedSize - index;
            if (receivedDataSize > I2C_CHUNK_SIZE) receivedDataSize = I2C_CHUNK_SIZE;

            if (hasSignature && checkCRC(lastCrc, chunkBuffer[2], chunkBuffer+3, receivedDataSize)) //receivedSize-3))
            {
                debug(Serial.printf("Signature matched, crc matched, chunkdata = %d, crc = %d\n", chunkBuffer[1], chunkBuffer[2]))

                lastCrc = chunkBuffer[2];

                if (chunkId < chunkCount)
                    chunkId++;

                memcpy(receiveBuffer+index, chunkBuffer + 3, receivedDataSize);//receivedSize - 3);

                index += receivedDataSize; // receivedSize - 3;

                delay(5);
            }
            else
            {
                if (receiveBuffer[index] == 255)
                    debug(Serial.printf("Ouch, crc failed :(\n"))
                else
                    debug(Serial.printf("Ouch, signature failed :(\n"))

                // response format incorrect, or crc check fail, delay and restart
                delay(10);
                chunkId = 0;
                index = 0;

                memset(receiveBuffer, 0, receiveBufferSize);
                if (tries++ >= I2C_MAX_TRIES)
                    return 0;
            }
        }
        else
        {
            debug(Serial.printf("Ouch, request failed, error code %d, retrying\n", LastError))
            delay(10);
            chunkId = 0;
            if (tries++ >= I2C_MAX_TRIES)
                return 0;
        }
    } while (chunkId < chunkCount);
    debug(Serial.println("All done !"))
    return expectedSize;
}


#endif