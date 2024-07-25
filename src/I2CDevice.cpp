#ifdef ESP32
#include "I2CDevice.h"

I2CDevice::I2CDevice(uint8_t startAddress, uint8_t endAddress, const char * name, uint8_t interruptPin)
{
    Addr = startAddress;
    ProbeStartAddr = startAddress;
    ProbeEndAddr = endAddress;
    Name = name;
    InterruptPin = digitalPinToInterrupt(interruptPin);
    IsPresent = false;
}

I2CDevice::I2CDevice(uint8_t deviceAddress, const char * name, uint8_t interruptPin): 
    I2CDevice(deviceAddress, deviceAddress, name, interruptPin)
{
}

bool I2CDevice::Detect(TwoWire *wire, TwoWire *wire2, uint8_t devId)
{
    char buffer[100];
    Serial.printf("probing %s on 0x%x-0x%x\n", Name, ProbeStartAddr, ProbeEndAddr);
    IsPresent = false;
    this->Wire = wire;
    for (Addr = ProbeStartAddr; Addr <= ProbeEndAddr; Addr++) {
        WireChannel = 0;
        Wire = wire;
        //Serial.printf("probing 0x%x, wire 1...\n", Addr);
        if (Check())
        {
            IsPresent = true;
        //    Serial.println("found");
        }
        else if (wire2) {
        //    Serial.printf("probing 0x%x, wire 2...\n", Addr);
            WireChannel = 1;
            Wire = wire2;
            IsPresent = Check();
        //    if (IsPresent) Serial.println("found");
        }
        if (IsPresent) break;
    }
    if (!IsPresent) Addr = -1;

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

TwoWire* I2CDevice::beginTransmission()
{
    this->Wire->beginTransmission(Addr);
    return this->Wire;
}

uint8_t I2CDevice::endTransmission()
{
    LastError = this->Wire->endTransmission();
    return LastError;
}


uint8_t I2CDevice::endTransmission(bool sendStop)
{
    LastError = this->Wire->endTransmission(sendStop);
    return LastError;
}

bool I2CDevice::sendMessageData(uint8_t messageId, uint8_t *messageData, size_t dataSize)
{
    this->Wire->beginTransmission(Addr);
    this->Wire->write(messageId);
    if(messageData != nullptr)
    {
        this->Wire->write(messageData, dataSize);
    }
    this->endTransmission();
    Serial.printf("Wire send result = %d \n", LastError);

    return LastError == 0;
}

bool I2CDevice::getData(uint8_t messageId, uint8_t *receiveBuffer, size_t receiveBufferSize, bool strictSize, uint8_t* messageData, size_t dataSize)
{
//    Serial.printf("sending %i", messageId);
    if(!sendMessageData(messageId, messageData, dataSize))
        return false;
//    Serial.printf("Requesting %i bytes from %x\n", messageId, Addr);
    this->Wire->requestFrom(Addr, receiveBufferSize);
    if (this->Wire->available())
    {
//        Serial.printf("Zeroing %i bytes in buffer\n", dataSize);
        memset(receiveBuffer, 0, receiveBufferSize);
        unsigned int cnt = 0;
        while (this->Wire->available())
        {
            // Buffer exceeded, try to recover wire (read all buffer), and return an error
            if (cnt == receiveBufferSize)
            {                
                while (this->Wire->available()) {
                    this->Wire->read();
                    cnt++;
                }
//                Serial.printf("Too many bytes : %i\n", cnt);
                LastError = 1;
                return false;
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
            return false;
        }
    } else {
//        Serial.printf("No data available\n");
        LastError = 16;
        return false;
    }
    return true;
}
#endif