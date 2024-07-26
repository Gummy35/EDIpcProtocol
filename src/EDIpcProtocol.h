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
    KEYPAD = 1,
    GAME_FLAGS = 2,
    STATUS = 4,
    TARGET = 8,
    LOCATION = 16,
    LOADOUT = 32
};

enum class COM_REQUEST_TYPE: uint8_t {
    NONE = 0,
    //GET_STAR_SYSTEM = 0,
    GET_UPDATES = 1,
    
    GET_STATUS = 5,
    GET_TARGET = 9,
    GET_LOCATION = 17,
    GET_LOADOUT = 33,

    TRACKER_DATA = 128,
    KEY_DATA = 129,
    PING_SLAVE = 255
};
