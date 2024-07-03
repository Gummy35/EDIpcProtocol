#include "EDIpcProtocolMaster.h"

EDIpcProtocolMaster* EDIpcProtocolMaster::instance = nullptr;

EDIpcProtocolMaster::EDIpcProtocolMaster(I2CDevice *comMcu)
{
    this->comMcu = comMcu;
    instance = this;
    _hasAxisChanges = false;
}

bool EDIpcProtocolMaster::begin()
{
    return true;
}

void EDIpcProtocolMaster::setAxis(int32_t x, int32_t y, int32_t z, int32_t rx, int32_t ry, int32_t rz)
{
    _axis.rx = rx;
    _axis.ry = ry;
    _axis.rz = rz;
    _axis.x = x;
    _axis.y = y;
    _axis.z = z;
    _hasAxisChanges = true;
}

// void getComMcuData()
// {
//   long start = micros();
//   TwoWire* wire;
//   int serial = 0;
//   Serial.printf((const char *)F("Current star system %s, requesting com mcu\n"), currentStarSystem);
//   wire = comMcuDC->beginTransmission();
//   wire->write(0);               // send data to the slave
//   if (requestSerial == 255) requestSerial = 0;
//   requestSerial++;
//   Serial.printf((const char *)F("Sending request #%d\n"), requestSerial);
//   wire->write(requestSerial);
//   uint8_t ec = comMcuDC->endTransmission();
//   if (ec == 0) {
//     while(serial != requestSerial) 
//     {
//       wire->requestFrom(comMcuDC->Addr, 30);    // request 20 bytes from slave device 0x12
//       if (wire->available())
//       {
//         serial = wire->read();
//         Serial.printf((const char *)F("Received data for request #%d\n"), serial);
//         if (serial != requestSerial) {
//           while (wire->available()) wire->read(); // slave may send less than requested
//         }
//       }
//       wire = comMcuDC->beginTransmission();
//       wire->write(0);               // send data to the slave
//       wire->write(requestSerial);
//       comMcuDC->endTransmission();
//     }
//     memset(currentStarSystem, 0, 21);
//     int cnt = 0;
//     while (wire->available()) { // slave may send less than requested
//       char c = wire->read(); // receive a byte as character
//       if (cnt < 20)
//         currentStarSystem[cnt++] = c;
//     }    
//     comMcuHasData = false;
//     long end = micros();
//     Serial.printf((const char *)F("New star system %s\n"), currentStarSystem);
//     Serial.printf((const char *)F("roundtrim took %6d\n"), end-start);
//   } else {
//     Serial.println(F("Request failed"));
//   }
// }

bool EDIpcProtocolMaster::_sendAxisData()
{
    const size_t axisStructSize = sizeof(AxisStruct);
    uint8_t *dataPtr = reinterpret_cast<uint8_t*>(&_axis);
    comMcu->sendMessageData(COM_REQUEST_TYPE::TRACKER_DATA, dataPtr, axisStructSize);
    return false;
}

bool EDIpcProtocolMaster::_getGameFlags()
{
    return false;
}

bool EDIpcProtocolMaster::_getGameInfo()
{
    return false;
}

bool EDIpcProtocolMaster::_getKeypadConfig()
{
    return false;
}

bool EDIpcProtocolMaster::_getWifiCredentials()
{
    uint8_t *receiveBuffer = reinterpret_cast<uint8_t *>(&wifiCredentials);    
    return comMcu->getData(COM_REQUEST_TYPE::GET_WIFI_CREDENTIALS, receiveBuffer, sizeof(WifiCredentials));
}

void EDIpcProtocolMaster::sendChanges()
{
    if (_hasAxisChanges) {
        if (_sendAxisData())
            _hasAxisChanges = false;
    }
}

uint8_t EDIpcProtocolMaster::retrieveChanges()
{
    bool result = false;
    uint8_t updateFlags;
    uint8_t *dataPtr = reinterpret_cast<uint8_t*>(&_lastUpdate);
    uint8_t *receiveBuffer = reinterpret_cast<uint8_t *>(&updateFlags);
    if (comMcu->getData(COM_REQUEST_TYPE::GET_UPDATES, receiveBuffer, 1, true, dataPtr, sizeof(unsigned long)))
    {
        result = true;
        if (updateFlags & UPDATE_CATEGORY::GAME_FLAGS)
        {
            Serial.println("Game flags changes");
            result &= _getGameFlags();
        }
        if (updateFlags & UPDATE_CATEGORY::GAME_INFO)
        {
            Serial.println("Game info changes");
            result &= _getGameInfo();
        }
        if (updateFlags & UPDATE_CATEGORY::KEYPAD)
        {
            Serial.println("Keypad config changes");
            result &= _getKeypadConfig();
        }
        if (updateFlags & UPDATE_CATEGORY::WIFI_CREDS)
        {
            Serial.println("Wifi creds changed");
            result &= _getWifiCredentials();
        }    
        return updateFlags;    
    }
    return 0;
}

bool EDIpcProtocolMaster::pingSlave()
{    
    char* pong;
    pong = (char *)malloc(10);
    if (comMcu->getData(COM_REQUEST_TYPE::PING_SLAVE, (uint8_t *)pong, 5))
    {
        Serial.println(pong);
        free(pong);
        return true;
    }
    free(pong);
    return false;
}