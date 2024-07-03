#pragma once

#include <Arduino.h>
#include <Wire.h>

class IAbstractDevice
{
public:    
    virtual bool Detect(TwoWire *wire, TwoWire *wire2, uint8_t devId) = 0;
    virtual bool Check() = 0;
    virtual bool Init() = 0;

    uint8_t DevId;
    const char* Name;
    bool IsPresent;
    uint8_t LastError;
};
