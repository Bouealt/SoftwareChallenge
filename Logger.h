#ifndef BXC_RTSPSERVER_LOG_H
#define BXC_RTSPSERVER_LOG_H

#include <iostream>
#include <string>
#include <cstdio>
#include <ctime>

// 获取当前时间的字符串表示
std::string getTime();

// 定义日志宏，将信息输出到 stderr
#define LOGI(format, ...)  fprintf(stderr, "[INFO] %s [%s:%d] " format "\n", getTime().c_str(), __FILE__, __LINE__, ##__VA_ARGS__)
#define LOGE(format, ...)  fprintf(stderr, "[ERROR] %s [%s:%d] " format "\n", getTime().c_str(), __FILE__, __LINE__, ##__VA_ARGS__)

#endif // BXC_RTSPSERVER_LOG_H
