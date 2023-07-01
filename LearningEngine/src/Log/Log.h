#pragma once

#include "Core/Base.h"

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

class LE_LOG
{
	public:
		static void Init();

		static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static Ref<spdlog::logger> s_CoreLogger;
		static Ref<spdlog::logger> s_ClientLogger;

};

// Core log macros
#define LE_CORE_TRACE(...)    ::LE_LOG::GetCoreLogger()->trace(__VA_ARGS__)
#define LE_CORE_INFO(...)     ::LE_LOG::GetCoreLogger()->info(__VA_ARGS__)
#define LE_CORE_WARN(...)     ::LE_LOG::GetCoreLogger()->warn(__VA_ARGS__)
#define LE_CORE_ERROR(...)    ::LE_LOG::GetCoreLogger()->error(__VA_ARGS__)
#define LE_CORE_CRITICAL(...) ::LE_LOG::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define LE_TRACE(...)         ::LE_LOG::GetClientLogger()->trace(__VA_ARGS__)
#define LE_INFO(...)          ::LE_LOG::GetClientLogger()->info(__VA_ARGS__)
#define LE_WARN(...)          ::LE_LOG::GetClientLogger()->warn(__VA_ARGS__)
#define LE_ERROR(...)         ::LE_LOG::GetClientLogger()->error(__VA_ARGS__)
#define LE_CRITICAL(...)      ::LE_LOG::GetClientLogger()->critical(__VA_ARGS__)

