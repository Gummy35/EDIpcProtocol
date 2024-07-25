#ifdef ESP32

#include "DeviceManager.h"

void DeviceManager::AddDevice(IAbstractDevice *device)
{
    if (deviceCount < MAX_DEVICES) {
        Devices[deviceCount++] = device;
    }
}

void DeviceManager::Init()
{
  uint8_t cnt = 0;
  for (uint8_t i = 0; i < 128; i++)
  {
    Wire.beginTransmission(i);
    uint8_t ec = Wire.endTransmission(true);
    if (ec == 0)
    {
      cnt++;
      Serial.printf("Wire0 Dev %x\n", i);
    }
  }

  cnt = 0;
  for (uint8_t i = 0; i < 128; i++)
  {
    Wire1.beginTransmission(i);
    uint8_t ec = Wire1.endTransmission(true);
    if (ec == 0)
    {
      cnt++;
      Serial.printf("Wire1 Dev %x\n", i);
    }
  }

  IAbstractDevice* d;
  uint8_t devId = 0;

  for (size_t i = 0; i < deviceCount; ++i)
  {    
    bool detected = Devices[i]->Detect(&Wire, &Wire1, devId++);
  } 

  for (size_t i = 0; i < deviceCount; ++i)
  { 
    if (Devices[i]->IsPresent)   
        Devices[i]->Init();
  } 

}
#endif