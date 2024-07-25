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

enum UPDATE_CATEGORY {
    KEYPAD = 1,
    GAME_FLAGS = 2,
    GAME_INFO = 4,
};

enum COM_REQUEST_TYPE {
    NONE = -1,
    //GET_STAR_SYSTEM = 0,
    GET_UPDATES = 1,
    TRACKER_DATA = 128,
    KEY_DATA = 129,
    PING_SLAVE = 255
};
