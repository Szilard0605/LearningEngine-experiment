#include "Log.h"

#include "spdlog/spdlog.h"

void LE_LOG::Init()
{
	spdlog::set_pattern("%H:%M:%S %^[%l]%$ %v");
}

void LE_LOG::ConsoleLogInfo(std::string info)
{
	spdlog::info(info.c_str());
}

void LE_LOG::ConsoleLogError(std::string info)
{
	spdlog::error(info.c_str());
}
