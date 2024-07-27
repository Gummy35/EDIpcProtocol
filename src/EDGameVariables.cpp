#include <Arduino.h>
#include "EDGameVariables.h"

EDGameVariablesClass::EDGameVariablesClass()
{
    memset(InfosCommanderName, 0, 21);
    memset(InfosShipName, 0, 21);
    memset(LocationSystemName, 0, 21);
    memset(LocationSystemName, 0, 21);
    memset(StatusLegal, 0, 21);
}


EDGameVariablesClass EDGameVariables;