#include "FileDialog.h"

#include "Core/Platform.h"

#include "Platform/Windows/WindowsFileDialog.h"


namespace Utils
{
	std::string FileDialog::OpenFile(const char* filters)
	{
		switch (Platform::GetOperatingSystem())
		{
		case::Platform::OperatingSystem::Windows:
		{
			return WindowsFileDialog::OpenFile(filters);
		}

		default:
			return std::string("Unknown");
		}
		return std::string("Unknown");
	}
}