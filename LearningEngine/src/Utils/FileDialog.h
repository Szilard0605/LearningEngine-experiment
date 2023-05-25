#pragma once

#include <string>

namespace Utils
{

	class FileDialog
	{
	public:
		static std::string OpenFile(const char* filters);
	};
}
