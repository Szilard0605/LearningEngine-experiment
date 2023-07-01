#include "Log.h"

void Log::LogMessage(LogType type, LogLevel level, const std::string& message) 
 {
    switch (type)
    {
        case LogType::Core:
        {
            switch (level)
            {
                case LogLevel::Info:
                {
                    std::cout << "\033[32m[Core Info]:\033[0m " << message << std::endl;
                    break;
                }
                case LogLevel::Warning:
                {
                    std::cout << "\033[33m[Core Warning]:\033[0m " << message << std::endl;
                    break;
                }
                case LogLevel::Error:
                {
                    std::cout << "\033[31m[Core Error]:\033[0m " << message << std::endl;
                    break;
                }
                default:
                    break;
            }
            break;
        }

        case LogType::Client:
        {
            switch (level)
            {
                case LogLevel::Info:
                {
                    std::cout << "\033[32m[Client Info]:\033[0m " << message << std::endl;
                    break;
                }
                case LogLevel::Warning:
                {
                    std::cout << "\033[33m[Client Warning]:\033[0m " << message << std::endl;
                    break;
                }
                case LogLevel::Error:
                {
                    std::cout << "\033[31m[Client Error]:\033[0m " << message << std::endl;
                    break;
                }
                default:
                    break;
            }
            break;
        }
    }
}
