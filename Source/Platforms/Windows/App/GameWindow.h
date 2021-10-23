#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include "FrameworkConfig.h"
#include "../../../Graphics/OpenGL.h"
#include "../../../Input/KeyCodes.h"


namespace GameDev2D
{
    //Forward declarations
    class ControllerGeneric;

    //The GameWindow handle window creation, event handling and OpenGL context creation
    class GameWindow
    {
    public:
        GameWindow(HINSTANCE currentInstance, int commandShow);
        ~GameWindow();

        //Create an OpenGL context for the requested version
        void CreateOpenGLContext(unsigned int majorVersion, unsigned int minorVersion);

        //Swaps the draw buffer
        void SwapDrawBuffer();

        //Controller methods, windows specific
        void DetectControllers();
        void HandleControllerInput();

        //Vertical sync methods
        void EnableVerticalSync(bool isEnabled);
        bool IsVerticalSyncEnabled();

        //Returns the windows handle
        HWND GetWindowHandle();

    private:
        //Convenicance methods to handle controller button input
        void HandleControllerButtonInput(ControllerGeneric* controller, WORD lastButtonState, WORD buttonState);

        //Windows event handler static method
        static LRESULT CALLBACK EventHandler(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);

        //Keyboard input static methods
        static void InitKeyMap();
        static KeyCode MapKey(WPARAM wParam, LPARAM lParam);

        //Windows specific members
        HWND m_WindowHandle;
        HDC m_DeviceContext;
        HGLRC m_OpenGLContext;

        //Vertical sync flag
        bool m_IsVerticalSyncEnabled;

        //Used to keep track of when the last controller detection was, on windows we 
        //have to continually poll to see if new controllers have been plugged in
        unsigned int m_LastControllerDetection;

        //Keycode map
        static KeyCode m_Keymap[KEY_CODE_COUNT];
    };
}

#endif