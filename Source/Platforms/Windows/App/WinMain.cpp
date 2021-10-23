#include "FrameworkConfig.h"
#include "GameWindow.h"
#include "OpenGL/OpenGLExtensions.h"
#include "../Platform_Windows.h"
#include "../../../Graphics/OpenGL.h"
#include "../../../../Game/Game.h"
#include "../../../../Game/Menus/Splash.h"
#include "../../../../Examples/Examples.h"



int WINAPI WinMain(HINSTANCE aCurrentInstance, HINSTANCE aPreviousInstance, LPSTR aCommandLine, int aCommandShow)
{
    //Create the Game Window
    GameDev2D::GameWindow* gameWindow = new GameDev2D::GameWindow(aCurrentInstance, aCommandShow);

    //Initialize the OpenGL extensions
    InitOpenGLExtensions();

    //Create an OpenGL 4.1 Context
    gameWindow->CreateOpenGLContext(4, 1);

    //Create the Window's PlatformLayer and initialize it and set the Root Scene
    GameDev2D::Platform_Windows* platform_Windows = new GameDev2D::Platform_Windows(gameWindow, WINDOWS_WINDOW_TITLE, WINDOWS_TARGET_FPS);
    platform_Windows->Init(WINDOWS_DEFAULT_WIDTH, WINDOWS_DEFAULT_HEIGHT, WINDOWS_DEFAULT_FULLSCREEN, WINDOWS_DEFAULT_VERTICAL_SYNC);
    platform_Windows->SetRootScene(new GameDev2D::ROOT_SCENE());

    //Detect the connected game controllers, if any
    gameWindow->DetectControllers();

    //Message handling
    MSG message;
	do 
    {
		if(PeekMessage(&message, NULL, 0,0, PM_REMOVE)) 
        {
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
	} while(platform_Windows->GameLoop() == true);
    
    //Cleanup the windows platform layer and the game window
    delete platform_Windows;
    delete gameWindow;
    
	return message.wParam;
}
