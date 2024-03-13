#include "Logger.h"

std::string getTime()
{
    const char *time_fmt = "%Y-%m-%d %H:%M:%S";
    time_t t = time(nullptr);
    char time_str[64];
    strftime(time_str, sizeof(time_str), time_fmt, localtime(&t));
    return std::string(time_str);
}