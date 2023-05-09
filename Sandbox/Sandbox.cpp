
#include <LearningEngine.h>

#include <stdio.h>
#include <windows.h>

#include "PhysicsTest.h"

class Sandbox : public Application
{
public:
	Sandbox()
		: Application("Sandbox", 1280, 720)
	{
		Application::Init();

		//PushLayer(new Sandbox2D());
		PushLayer(new PhysicsTest());
	}

	~Sandbox() 
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

	Sandbox game;
	game.Start();
	
	return 0;
}