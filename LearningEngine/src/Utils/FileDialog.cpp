#include "FileDialog.h"

#include "Core/Platform.h"

#include "Platform/Windows/WindowsFileDialog.h"


namespace Utils
{
	bool FileDialog::OpenFile(const char* filters, std::string& outPath)
	{
		switch (Platform::GetOperatingSystem())
		{
		case::Platform::OperatingSystem::Windows:
		{
			return WindowsFileDialog::OpenFile(filters, outPath);
		}

		default:
			return false;
		}
		return false;
	}
}