#include "WindowsFileDialog.h"


#include "Core/Application.h"


#include <windows.h>

#include "glfw3.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include "glfw3native.h"

#include <filesystem>

std::string WindowsFileDialog::OpenFile(const char* filters)
{
	OPENFILENAMEA ofn;
	CHAR szFile[260] = { 0 };
	CHAR currentDir[256] = { 0 };

	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = glfwGetWin32Window(Application::GetInstance()->GetWindow()->GetNativeWindow());
	ofn.lpstrFile = szFile;
	if (GetCurrentDirectoryA(256, currentDir))
		ofn.lpstrInitialDir = currentDir;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = (LPCSTR)filters;
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;;

	ofn.lpstrDefExt = strchr(filters, '\0') + 1;

	if (GetOpenFileNameA(&ofn) == TRUE) 
	{
		return std::string(ofn.lpstrFile);
	}


	return std::string("None");
}
