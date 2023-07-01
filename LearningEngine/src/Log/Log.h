#pragma once

#include <iostream>
#include <string>

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
    static void LogMessage(LogType type, LogLevel level, const std::string& message);

};

#define LE_CORE_INFO(message) ::Log::LogMessage(LogType::Core, LogLevel::Info, message)
#define LE_CORE_WARN(message) ::Log::LogMessage(LogType::Core, LogLevel::Warning, message)
#define LE_CORE_ERROR(message) ::Log::LogMessage(LogType::Core, LogLevel::Error, message)

#define LE_EDITOR_INFO(message) ::Log::LogMessage(LogType::Client, LogLevel::Info, message)
#define LE_EDITOR_WARN(message) ::Log::LogMessage(LogType::Client, LogLevel::Warning, message)
#define LE_EDITOR_ERROR(message) ::Log::LogMessage(LogType::Client, LogLevel::Error, message)