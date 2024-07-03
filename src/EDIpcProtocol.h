#pragma once

#include <Wire.h>

struct AxisStruct {
    int32_t x;
    int32_t y;
    int32_t z;
    int32_t rx;
    int32_t ry;
    int32_t rz;
};

struct WifiCredentials {
    char ssid[16];
    char password[32];
    uint32_t ip;
    unsigned long __update;
};

enum UPDATE_CATEGORY {
    KEYPAD = 1,
    GAME_FLAGS = 2,
    GAME_INFO = 4,
    WIFI_CREDS = 8,
};

enum COM_REQUEST_TYPE {
    NONE = -1,
    //GET_STAR_SYSTEM = 0,
    GET_UPDATES = 1,
    GET_WIFI_CREDENTIALS = 2,
    TRACKER_DATA = 128,
    PING_SLAVE = 255
};
