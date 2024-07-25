#include <Logger.h>

LoggerClass::LoggerClass()
{
    _logfunc = nullptr;
}

void LoggerClass::SetLogger(std::function<void(const char *logString)> log)
{
    _logfunc = log;
}

void LoggerClass::Log(const char *logString)
{
    if (_logfunc != nullptr)
        _logfunc(logString);
}


LoggerClass Logger;