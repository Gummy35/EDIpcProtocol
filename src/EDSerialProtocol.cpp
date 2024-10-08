#ifndef ESP32

#include <EDSerialProtocol.h>

const char  infoString [] = "EDControlPanel V1.0.0";

/// @brief Serial command I
void sendInfo()
{
  Serial.print("I\t");
  Serial.println(infoString);
}

EDSerialProtocol::EDSerialProtocol(EDIpcProtocolSlave* IpcProtocolInstance)
{
    _ipcProtocolInstance = IpcProtocolInstance;
    _isStartup = true;
}

void EDSerialProtocol::Begin()
{
    _isStartup = false;
}

void EDSerialProtocol::ReadLine(char * res, int maxsize, char separator)
{
  static char buffer[100 + 1] = {0};
  static size_t size = 0;
  size = Serial.readBytesUntil(separator, buffer, 100);
  buffer[size] = 0;
  if (res != nullptr)
    strncpy(res, buffer, maxsize);
}

void EDSerialProtocol::ReadSeparator()
{
  while (Serial.available())
  {
    char c = Serial.read();
    if ((c == '\t') || (c == 0))
      return;
  }
}

void EDSerialProtocol::Handle()
{
  while (Serial.available() > 0)
  {
    // read the incoming byte:
    byte command = Serial.read();

    // use if ... else instead of switch. Arduino is a pain in the ass with switch statements.

    if (command == SERIAL_COMMAND_SILENT)
    {
      _outputUI = false;
      Serial.println("S"); //silent
    }
    else if (command == SERIAL_COMMAND_HELLO)
    {
      if (_isStartup)
        Serial.println("h");
      else
        Serial.println("H"); // Hello
    }
    else if (command == SERIAL_COMMAND_SHUTDOWN)
    {
      _ipcProtocolInstance->addUpdate(UPDATE_CATEGORY::UC_SHUTDOWN);
      _ipcProtocolInstance->signalMaster();
    }
    else if (command == SERIAL_COMMAND_VERBOSE)
    {
      Serial.println("V"); //verbose
      sendInfo();
      _outputUI = true;
    }
    else if (command == SERIAL_COMMAND_SENDINFOS)
    {
      sendInfo();
    }
    else if (command == SERIAL_COMMAND_LOCATION)
    {
      ReadSeparator();
      ReadLine(EDGameVariables.LocationSystemName, 20, '\t');
      ReadLine(EDGameVariables.LocationStationName, 20, '\t');
      ReadLine(EDGameVariables.LocalAllegiance, 20, '\t');
      ReadLine(EDGameVariables.SystemSecurity, 20, '\0');
      _ipcProtocolInstance->addUpdate(UPDATE_CATEGORY::UC_LOCATION);
      _ipcProtocolInstance->signalMaster();
    }
    else if (command == SERIAL_COMMAND_GAMEINFOS)
    {
      ReadSeparator();
      ReadLine(EDGameVariables.InfosCommanderName, 20, '\t');
      ReadLine(EDGameVariables.InfosShipName, 20, '\0');
      _ipcProtocolInstance->addUpdate(UPDATE_CATEGORY::UC_INFOS);
      _ipcProtocolInstance->clearUpdate(UPDATE_CATEGORY::UC_SHUTDOWN);
      _ipcProtocolInstance->signalMaster();     
    }
    else if (command == SERIAL_COMMAND_GAMEFLAGS)
    {
      Serial.readBytes((uint8_t *)(&EDGameVariables.StatusFlags1), 4);
      Serial.readBytes((uint8_t *)(&EDGameVariables.StatusFlags2), 4);
      Serial.readBytes((uint8_t *)(&EDGameVariables.GuiFocus), 1);
      Serial.readBytes((uint8_t *)(&EDGameVariables.FireGroup), 1);
      ReadLine(EDGameVariables.StatusLegal, 20, '\0');
      if ((EDGameVariables.StatusFlags1 > 0) || (EDGameVariables.StatusFlags2 > 0))
        _ipcProtocolInstance->clearUpdate(UPDATE_CATEGORY::UC_SHUTDOWN);
      _ipcProtocolInstance->addUpdate(UPDATE_CATEGORY::UC_STATUS);
      _ipcProtocolInstance->signalMaster();
    }
    else if (command == SERIAL_COMMAND_LOADOUT)
    {
      Serial.readBytes((uint8_t *)(&EDGameVariables.LoadoutFlags1), 4);
      Serial.readBytes((uint8_t *)(&EDGameVariables.LoadoutFlags2), 4);
      ReadLine(nullptr, 20, '\0');
      _ipcProtocolInstance->addUpdate(UPDATE_CATEGORY::UC_STATUS);
      _ipcProtocolInstance->signalMaster();
    }
    else if (command == SERIAL_COMMAND_NAVROUTE)
    {
      ReadSeparator();
      ReadLine(EDGameVariables.Navroute1, 20, '\t');
      ReadLine(EDGameVariables.Navroute2, 20, '\t');
      ReadLine(EDGameVariables.Navroute3, 20, '\0');
      _ipcProtocolInstance->addUpdate(UPDATE_CATEGORY::UC_LOCATION);
      _ipcProtocolInstance->signalMaster();
    }
    else if (command == SERIAL_COMMAND_ALERTS)
    {
      Serial.readBytes((uint8_t *)(&EDGameVariables.AlertDuration), 4);
      ReadLine(EDGameVariables.AlertMessageTitle, 20, '\t');
      ReadLine(EDGameVariables.AlertMessage1, 20, '\t');
      ReadLine(EDGameVariables.AlertMessage2, 20, '\t');
      ReadLine(EDGameVariables.AlertMessage3, 20, '\0');
      _ipcProtocolInstance->addUpdate(UPDATE_CATEGORY::UC_URGENT_INFO);
      _ipcProtocolInstance->signalMaster();
    }
  }
}

#endif