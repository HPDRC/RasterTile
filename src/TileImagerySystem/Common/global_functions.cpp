#include "global_functions.h"
#include "global_config.h"
#include "CSemaphore.h"
#include <time.h>
#include "CConfig.h"
#include <Windows.h>

CSemaphore g_logSem(1, 1);

void __Log(char *msg)
{
    if (CConfig::LOG_PATH == NULL) return;

    // generate time string
    char timeString[30];
    time_t timer = time(NULL);
    tm* now = localtime(&timer);
    sprintf(timeString, "%02d/%02d %02d:%02d:%02d ", now->tm_mon+1, now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec);

    // prepare length
    int msgLen = (int)strlen(msg);
    int timeStringLen = (int)strlen(timeString);

    // write to file
    g_logSem.Lock();
    FILE *f = fopen(CConfig::LOG_PATH, "a");
    if (f == NULL)
    {
        g_logSem.Unlock();
        return;
    }
    fwrite(timeString, 1, timeStringLen, f);
    fwrite(msg, 1, msgLen, f);
    fwrite("\n", 1, 1, f);
    fclose(f);
    g_logSem.Unlock();
}

void Log(char *format, ...)
{
    char msg[1024];
    va_list argList;
    va_start(argList, format);
    vsprintf(msg, format, argList);
    va_end(argList);

    __Log(msg);
}

void LogThrow(char *format, ...)
{
    char msg[1024];
    va_list argList;
    va_start(argList, format);
    vsprintf(msg, format, argList);
    va_end(argList);

    __Log(msg);
    throw format;
}
