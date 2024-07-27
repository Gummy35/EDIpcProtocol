#pragma once
#include <Arduino.h>

class EDGameVariablesClass {

public:
    EDGameVariablesClass();

    char LocationSystemName[21];
    char LocationStationName[21];
    char InfosCommanderName[21];
    char InfosShipName[21];
    char StatusLegal[21];
    uint32_t flags1;
    uint32_t flags2;   
};

extern EDGameVariablesClass EDGameVariables;