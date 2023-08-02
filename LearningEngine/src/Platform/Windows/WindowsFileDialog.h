#pragma once

#include <string>

class WindowsFileDialog
{
public:
	static bool OpenFile(const char* filters, std::string& outPath);
	static std::string SaveFile(const char* filters);
};

