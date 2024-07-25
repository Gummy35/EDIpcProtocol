#include <Arduino.h>

class LoggerClass
{
    public:
        LoggerClass();
        void SetLogger(std::function<void(const char *logString)> log);
        void Log(const char *logString);
    private:        
        std::function<void(const char *logString)> _logfunc;
};

extern LoggerClass Logger;