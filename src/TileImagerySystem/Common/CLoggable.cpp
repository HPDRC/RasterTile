#include "CLoggable.h"
#include <ctime>
#include <stdarg.h>

CLoggable::CLoggable(bool isLogEnabled)
{
    this->isLogEnabled = isLogEnabled;
    if (isLogEnabled)
        startTime = clock();
}

void CLoggable::ClearLog()
{
    message = "";
}

string CLoggable::GetLog()
{
    return message;
}

void CLoggable::WriteLog(char *format, ...)
{
    if (isLogEnabled)
    {
        char msg[512];
        va_list args;
        va_start(args, format);
        vsprintf(msg, format, args);
        va_end(args);

        char line[612];
        sprintf(line, "%dms: %s\r\n", clock() - startTime, msg);
        message += line;
    }
}