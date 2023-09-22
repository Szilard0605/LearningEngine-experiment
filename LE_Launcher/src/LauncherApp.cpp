#include "LearningEngine.h"

#include "LauncherLayer.h"

class Editor : public Application
{
public:
	Editor()
		: Application("LearningEngine Launcher", 1280, 720)
	{
		Application::Init();

		PushLayer(new LauncherLayer());

	}

	~Editor()
	{

	}
};

/*#ifndef _DEBUG
int _stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
#else
int main()
#endif*/
int main()
{

	Editor editor;
	editor.Start();

	return 0;
}