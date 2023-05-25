#pragma once

#include <string>

class WindowsFileDialog
{
public:
	static std::string OpenFile(const char* filters);
};

