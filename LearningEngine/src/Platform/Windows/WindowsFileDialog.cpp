#include "WindowsFileDialog.h"


#include "Core/Application.h"


#include <windows.h>

#include "glfw3.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include "glfw3native.h"

#include <filesystem>

bool WindowsFileDialog::OpenFile(const char* filters, std::string& outPath)
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

	if (GetOpenFileNameA(&ofn)) 
	{
		outPath = ofn.lpstrFile;
		return true;
	}

	outPath = std::string("None");
	return false;
}

std::string WindowsFileDialog::SaveFile(const char* filters)
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

	ofn.lpstrDefExt = "lescene";

	if (GetSaveFileNameA(&ofn))
	{
		return ofn.lpstrFile;
	}
	return std::string();
}
