#pragma once

#include <string>

namespace Utils
{

	class FileDialog
	{
	public:
		static bool OpenFile(const char* filters, std::string& outPath);
	};
}
