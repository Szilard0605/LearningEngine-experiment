#include "Log.h"

#ifdef _WIN32
    #include <windows.h>
#endif

#include "Core/Platform.h"

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
