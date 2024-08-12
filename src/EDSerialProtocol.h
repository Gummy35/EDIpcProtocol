#pragma once

#ifndef ESP32

#include <EDIpcProtocol.h>
#include <EDIpcProtocolSlave.h>
#include <EDGameVariables.h>

#define SERIAL_COMMAND_SILENT 'S'
#define SERIAL_COMMAND_HELLO 'H'
#define SERIAL_COMMAND_VERBOSE 'V'
#define SERIAL_COMMAND_SENDINFOS 'I'
#define SERIAL_COMMAND_LOCATION 'l'
#define SERIAL_COMMAND_GAMEINFOS 'G'
#define SERIAL_COMMAND_GAMEFLAGS 'F'
#define SERIAL_COMMAND_LOADOUT 'M'
#define SERIAL_COMMAND_NAVROUTE 'N'
#define SERIAL_COMMAND_ALERTS 'A'

class EDSerialProtocol {

public:
    EDSerialProtocol(EDIpcProtocolSlave *IpcProtocolInstance);
    void Begin();
    void ReadLine(char *res, int maxsize, char separator);
    void ReadSeparator();
    void Handle();

private:
    EDIpcProtocolSlave* _ipcProtocolInstance;
    boolean _outputUI = false;
    boolean _isStartup = true;

};

#endif