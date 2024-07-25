#ifdef ESP32
#include "EDIpcProtocolMaster.h"
#include "Logger.h"

EDIpcProtocolMaster* EDIpcProtocolMaster::instance = nullptr;
static StaticQueue_t _keyQueueBuffer;
uint8_t _keyQueueStorage[KEY_EVENT_QUEUE_SIZE * sizeof(KeyEvent)];
//QueueHandle_t _keyQueueHandle = xQueueCreateStatic(KEY_EVENT_QUEUE_SIZE, sizeof(KeyEvent), &(_keyQueueStorage[0]), &_keyQueueBuffer);;
QueueHandle_t _keyQueueHandle;// = xQueueCreate(KEY_EVENT_QUEUE_SIZE, sizeof(KeyEvent*));

EDIpcProtocolMaster::EDIpcProtocolMaster(I2CDevice *comMcu)
{
    this->comMcu = comMcu;
    instance = this;
    _hasAxisChanges = false;
}

bool EDIpcProtocolMaster::begin()
{
    _keyQueueHandle = xQueueCreateStatic(KEY_EVENT_QUEUE_SIZE, sizeof(KeyEvent), _keyQueueStorage, &_keyQueueBuffer);
    if (_keyQueueHandle == NULL) 
    {
        Serial.println("Failed to create key queue");
        return false;
    }
    return true;
}

bool EDIpcProtocolMaster::sendKey(KeyEvent event)
{
    if (_keyQueueHandle == NULL) {
        Logger.Log("Key queue unavailable");
        return false;
    }
    if (xQueueSend(_keyQueueHandle, &event, portMAX_DELAY) != pdPASS) {
        Logger.Log("Failed to add item to the queue");
        return false;
    }
    Serial.println("Key queued");

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
    comMcu->sendMessageData((uint8_t)COM_REQUEST_TYPE::TRACKER_DATA, dataPtr, axisStructSize);
    return false;
}

bool EDIpcProtocolMaster::_sendKeyData(KeyEvent* keyEvent)
{
    const size_t keyStructSize = sizeof(KeyEvent);
    return comMcu->sendMessageData((uint8_t)COM_REQUEST_TYPE::KEY_DATA, (uint8_t*)keyEvent, keyStructSize);
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

void EDIpcProtocolMaster::sendChanges()
{
    // if (_hasAxisChanges) {
    //     if (_sendAxisData())
    //         _hasAxisChanges = false;
    // }

    KeyEvent keyEvent;
    while (xQueueReceive(_keyQueueHandle, &keyEvent, 0) == pdPASS) {
        Serial.printf("Sending %d, %d, %d \n", keyEvent.key, keyEvent.count, keyEvent.pressed);
        _sendKeyData(&keyEvent);
    };
}

uint8_t EDIpcProtocolMaster::retrieveChanges()
{
    bool result = false;
    uint8_t updateFlags;
    uint8_t *dataPtr = reinterpret_cast<uint8_t*>(&_lastUpdate);
    uint8_t *receiveBuffer = reinterpret_cast<uint8_t *>(&updateFlags);
    if (comMcu->getData((uint8_t)COM_REQUEST_TYPE::GET_UPDATES, receiveBuffer, 1, true, dataPtr, sizeof(unsigned long)))
    {
        result = true;
        if (updateFlags & (uint8_t)UPDATE_CATEGORY::GAME_FLAGS)
        {
            Serial.println("Game flags changes");
            result &= _getGameFlags();
        }
        if (updateFlags & (uint8_t)UPDATE_CATEGORY::GAME_INFO)
        {
            Serial.println("Game info changes");
            result &= _getGameInfo();
        }
        if (updateFlags & (uint8_t)UPDATE_CATEGORY::KEYPAD)
        {
            Serial.println("Keypad config changes");
            result &= _getKeypadConfig();
        }
        
        return updateFlags;    
    }
    return 0;
}

bool EDIpcProtocolMaster::pingSlave()
{    
    char* pong;
    pong = (char *)malloc(10);
    if (comMcu->getData((uint8_t)COM_REQUEST_TYPE::PING_SLAVE, (uint8_t *)pong, 5))
    {
        Serial.println(pong);
        free(pong);
        return true;
    }
    free(pong);
    return false;
}


#endif