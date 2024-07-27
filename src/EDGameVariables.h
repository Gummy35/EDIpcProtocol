#pragma once

class EDGameVariablesClass {

public:
    EDGameVariablesClass();

    char LocationSystemName[21];
    char LocationStationName[21];
    char InfosCommanderName[21];
    char InfosShipName[21];
};

extern EDGameVariablesClass EDGameVariables;