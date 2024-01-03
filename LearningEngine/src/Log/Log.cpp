#include "Log.h"

#ifdef _WIN32
    #include <windows.h>
#endif

#include "Core/Platform.h"

#include <cstdarg> // For variadic functions

void Log::Init()
{

    /* Note (Szilard): You need to enable this on windows, 
                       because the executable console window does
                       not enable ANSI escape sequences by default */                 
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(hConsole, &mode);
    SetConsoleMode(hConsole, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif
}

static void PrintFormattedMessage(LogLevel level, const char* prefix, const char* format, va_list args);

void Log::LogMessage(LogType type, LogLevel level, std::string format, ...)
 {
    va_list args;
    va_start(args, format.c_str());

    const char* prefix = type == LogType::Core ? "Core" : "Client";

    PrintFormattedMessage(level, prefix, format.c_str(), args);

    va_end(args);

    /*switch (type)
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
    }*/
}


void PrintFormattedMessage(LogLevel level, const char* prefix, const char* format, va_list args)
{
    switch (level)
    {
        case LogLevel::Info:
        {
            std::cout << "\033[32m[" << prefix << " Info]:\033[0m ";
            break;
        }
        case LogLevel::Warning:
        {
            std::cout << "\033[33m[" << prefix << " Warning]:\033[0m ";
            break;
        }
        case LogLevel::Error:
        {
            std::cout << "\033[31m[" << prefix << " Core Error]:\033[0m ";
            break;
        }
    }
    vprintf(format, args);
    std::cout << std::endl;
}