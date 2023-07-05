#include "LearningEngine.h"

#include "EditorLayer.h"

#include "imgui.h"

class Editor : public Application
{
public:
	Editor()
		: Application("LearningEngine Editor", 1920, 1080)
	{
		Application::Init();

		PushLayer(new EditorLayer());
	
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