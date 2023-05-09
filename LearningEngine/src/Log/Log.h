#pragma once

#include <string>



#define LE_LOG_INFO(a) LE_LOG::ConsoleLogInfo(a)
#define LE_LOG_ERROR(a) LE_LOG::ConsoleLogError(a)


// TODO: Editor log 


class LE_LOG
{
	public:
		static void Init();
		static void ConsoleLogInfo(std::string info);
		static void ConsoleLogError(std::string info);

};

