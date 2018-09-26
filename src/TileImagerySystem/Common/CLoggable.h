#pragma once
#include "global_includes.h"

class CLoggable
{
private:
    string message;
    int startTime;

protected:
    bool isLogEnabled;

public:
    CLoggable(bool isLogEnabled);
    void ClearLog();
    string GetLog();
    void WriteLog(char *format, ...);
};