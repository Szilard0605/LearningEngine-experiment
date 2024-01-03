#pragma once

#include <iostream>
#include <string>
#include <stdarg.h>

enum class LogType 
{
    Core,
    Client
};

enum class LogLevel 
{
    Info,
    Warning,
    Error
};

class Log 
{
public:
    static void Init();
    static void LogMessage(LogType type, LogLevel level, std::string format, ...);

};

#define LE_CORE_INFO(message, ...) ::Log::LogMessage(LogType::Core, LogLevel::Info, message, __VA_ARGS__)
#define LE_CORE_WARN(message, ...) ::Log::LogMessage(LogType::Core, LogLevel::Warning, message, __VA_ARGS__)
#define LE_CORE_ERROR(message, ...) ::Log::LogMessage(LogType::Core, LogLevel::Error, message, __VA_ARGS__)

#define LE_CLIENT_INFO(message, ...) ::Log::LogMessage(LogType::Client, LogLevel::Info, message, __VA_ARGS__)
#define LE_CLIENT_WARN(message, ...) ::Log::LogMessage(LogType::Client, LogLevel::Warning, message, __VA_ARGS__)
#define LE_CLIENT_ERROR(message, ...) ::Log::LogMessage(LogType::Client, LogLevel::Error, message, __VA_ARGS__)