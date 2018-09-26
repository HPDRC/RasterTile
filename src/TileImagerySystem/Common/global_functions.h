#pragma once
#include <string>
using namespace std;

void Log(char *format, ...);

void LogThrow(char *format, ...);

// for now, only log assert fail events
inline void Assert(bool assert, char *msg)
{
    if (assert == false)
        Log("Assert failed: %s", msg);
}