#pragma once
#ifdef ESP32

#include <Arduino.h>
#include <Wire.h>
#include <IAbstractDevice.h>

constexpr size_t MAX_DEVICES = 10;

class DeviceManager
{
public:
    DeviceManager() : deviceCount(0) {}

    void AddDevice(IAbstractDevice *device);
    void Init();
    
    IAbstractDevice* Devices[MAX_DEVICES];

private:
    size_t deviceCount;
};

#endif