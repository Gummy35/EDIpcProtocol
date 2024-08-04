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

struct KeyEvent {
    uint8_t key;
    uint8_t pressed;
    uint8_t count;
};

enum class UPDATE_CATEGORY: uint8_t {
    UC_KEYPAD = 1,
    UC_GAME_FLAGS = 2,
    UC_STATUS = 4,
    UC_TARGET = 8,
    UC_LOCATION = 16,
    UC_LOADOUT = 32,
    UC_INFOS = 64,
};

enum class COM_REQUEST_TYPE: uint8_t {
    CRT_NONE = 0,
    //GET_STAR_SYSTEM = 0,
    CRT_GET_UPDATES = 1,    
    CRT_GET_STATUS = 5,
    CRT_GET_TARGET = 9,
    CRT_GET_LOCATION = 17,
    CRT_GET_NAVROUTE = 18,
    CRT_GET_SYSTEM_POLICY = 19,
    CRT_GET_LOADOUT = 33,
    CRT_GET_INFOS = 65,
    CRT_SEND_TRACKER_DATA = 128,
    CRT_SEND_KEY_DATA = 129,
    CRT_SEND_REBOOT = 130,
    CRT_MOCK = 254,
    CRT_GET_PING_SLAVE = 255
};
