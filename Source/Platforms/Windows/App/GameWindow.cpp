
#include "GameWindow.h"
#include "../../../Platforms/PlatformLayer.h"
#include "../../../Platforms/Windows/Platform_Windows.h"
#include "../../../Services/ServiceLocator.h"
#include "../../../Services/InputManager/InputManager.h"
#include "../../../Input/Controller/ControllerData.h"
#include "../../../Input/Controller/ControllerGeneric.h"
#include "../../../../Platforms/Windows/GameDev2D/resource.h"


//#pragma comment(lib, "Xinput9_1_0")
//#pragma comment(lib, "Winmm")


namespace GameDev2D
{
    KeyCode GameWindow::m_Keymap[KEY_CODE_COUNT];

    GameWindow::GameWindow(HINSTANCE aCurrentInstance, int aCommandShow) :
        m_WindowHandle(nullptr),
        m_DeviceContext(nullptr),
        m_OpenGLContext(nullptr),
        m_IsVerticalSyncEnabled(false)
    {
        //Initialize the key input map
        InitKeyMap();

        //Initialize the window class data and register it
        WNDCLASSEX windowClassExtended;
        windowClassExtended.cbSize          = sizeof(WNDCLASSEX);
        windowClassExtended.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
        windowClassExtended.lpfnWndProc	    = &GameWindow::EventHandler;
        windowClassExtended.cbClsExtra		= 0;
        windowClassExtended.cbWndExtra		= 0;
        windowClassExtended.hInstance		= aCurrentInstance;
        windowClassExtended.hIcon			= LoadIcon(aCurrentInstance, MAKEINTRESOURCE(IDI_MYICON));
        windowClassExtended.hCursor		    = LoadCursor(NULL, IDC_ARROW);
        windowClassExtended.hbrBackground	= NULL;
        windowClassExtended.lpszMenuName	= NULL;
        windowClassExtended.lpszClassName	= L"GameDev2D";
        windowClassExtended.hIconSm		    = LoadIcon(aCurrentInstance, MAKEINTRESOURCE(IDI_MYICON));
        RegisterClassEx(&windowClassExtended);

        //Create the window, if the assert is hit, we couldn't create a window
        m_WindowHandle = CreateWindowEx(WS_EX_APPWINDOW, L"GameDev2D", L"", WS_CAPTION | WS_POPUPWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, aCurrentInstance, this);
        assert(m_WindowHandle != nullptr);

        //Get the device context, if the assert is hit, then we couldn't get a device context
        m_DeviceContext = GetDC(m_WindowHandle);
        assert(m_DeviceContext != nullptr);

        //The pixel format descriptor tells Windows how we want our OpenGL context to be setup
        static PIXELFORMATDESCRIPTOR pixelFormatDescriptor =
        {
            sizeof(PIXELFORMATDESCRIPTOR),  // Size Of This Pixel Format Descriptor
            1,                              // Version Number
            PFD_DRAW_TO_WINDOW |            // Format Must Support Window
              PFD_SUPPORT_OPENGL |          // Format Must Support OpenGL
              PFD_DOUBLEBUFFER,             // Must Support Double Buffering
            PFD_TYPE_RGBA,                  // Request An RGBA Format
            32,                             // Select Our Color Depth
            0, 0, 0, 0, 0, 0,               // Color Bits Ignored
            0,                              // No Alpha Buffer
            0,                              // Shift Bit Ignored
            0,                              // No Accumulation Buffer
            0, 0, 0, 0,                     // Accumulation Bits Ignored
            32,                             // Bits for Z-Buffer (Depth Buffer)
            0,                              // Stencil bits
            0,                              // No Auxiliary Buffer
            PFD_MAIN_PLANE,                 // Main Drawing Layer
            0,                              // Reserved
            0, 0, 0                         // Layer Masks Ignored
        };

        //Choose the pixel format, based on the pixel format descriptor. If the assert fails it means the supplied pixel format descriptor is invalid on this system
        unsigned int pixelFormat = ChoosePixelFormat(m_DeviceContext, &pixelFormatDescriptor);
        assert(pixelFormat != 0);

        //Set the pixel format, if the assert is hit, it means the pixel format supplied is invalid on this system
        BOOL success = SetPixelFormat(m_DeviceContext, pixelFormat, &pixelFormatDescriptor);
        assert(success == TRUE);

        //Create an opengl context, this context will be version 2.1, and is temporary. 
        //If the assert is hit, it means creating an OpenGL context failed
        m_OpenGLContext = wglCreateContext(m_DeviceContext);
        assert(m_OpenGLContext != nullptr);
    
        //Make the current opengl context current
        wglMakeCurrent(m_DeviceContext, m_OpenGLContext);

        //Show the window and update it
        ShowWindow(m_WindowHandle, aCommandShow);
        UpdateWindow(m_WindowHandle);
    }

    GameWindow::~GameWindow()
    {

    }

    void GameWindow::CreateOpenGLContext(unsigned int aMajorVersion, unsigned int aMinorVersion)
    {
        //Set attributes with the OpenGL version we want to target
        int attributes[] = 
        {
		    WGL_CONTEXT_MAJOR_VERSION_ARB, aMajorVersion, // Set the MAJOR version
		    WGL_CONTEXT_MINOR_VERSION_ARB, aMinorVersion, // Set the MINOR version
		    0
	    };

        //If this assert is hit, it means your computer's video card doesn't support the version of OpenGL we want to target
        assert(wglCreateContextAttribsARB != nullptr);

        //Create a new opengl context with the attributes above
        HGLRC openGLContext = wglCreateContextAttribsARB(m_DeviceContext, NULL, attributes);
		
        //Remove the temporary context from being active and delete the temporary OpenGL 2.1 context
        wglMakeCurrent(NULL, NULL); 
		wglDeleteContext(m_OpenGLContext);

        //Set the OpenGL context and make it current
        m_OpenGLContext = openGLContext;
		wglMakeCurrent(m_DeviceContext, m_OpenGLContext);
    }

    void GameWindow::SwapDrawBuffer()
    {
        SwapBuffers(m_DeviceContext);
    }

    void GameWindow::EnableVerticalSync(bool aIsEnabled)
    {
        //Safety check that vertical sync is not already set
        if(m_IsVerticalSyncEnabled != aIsEnabled)
        {
            //
            m_IsVerticalSyncEnabled = aIsEnabled;

            //Ensure the v-sync function actually exists
            if(wglSwapIntervalEXT != nullptr)
            {
		        if(m_IsVerticalSyncEnabled) 
                {
			        wglSwapIntervalEXT(1);
		        } 
                else 
                {
			        wglSwapIntervalEXT(0);
		        }
	        }
            else
            {
                //
                m_IsVerticalSyncEnabled = false;
            }
        }
    }

    bool GameWindow::IsVerticalSyncEnabled()
    {
        return m_IsVerticalSyncEnabled;
    }

    HWND GameWindow::GetWindowHandle()
    {
        return m_WindowHandle;
    }

    void GameWindow::DetectControllers()
    {
        //Cache the last controller detection time stamp
        m_LastControllerDetection = ServiceLocator::GetPlatformLayer()->GetTicks();

        //Local variables used in the for loop below
        bool duplicate = false;
        XINPUT_STATE state;

        //Cycle through available controller ports
	    for(unsigned int controllerPort = 0; controllerPort < XUSER_MAX_COUNT; controllerPort++) 
        {
            //Zero out the memory of the state struct
            ZeroMemory(&state, sizeof(XINPUT_STATE));
 
            //Can we get the state of a controller on this port
            if(XInputGetState(controllerPort, &state) == ERROR_SUCCESS)
            {
			    duplicate = false;
			    for(unsigned int i = 0; i < GameDev2D::ServiceLocator::GetInputManager()->GetNumberOfControllers(); i++) 
                {    
                    //Check if this port already has a registered contoller
                    if(GameDev2D::ServiceLocator::GetInputManager()->GetControllerForIndex(i) != nullptr)
                    {
                        if((uintptr_t)GameDev2D::ServiceLocator::GetInputManager()->GetControllerForIndex(i)->GetDevice() == (uintptr_t)controllerPort) 
                        {
					        duplicate = true;
					        break;
				        }
                    }
			    }

                //If its a duplicate, continue to the next port
			    if(duplicate == true) 
                {
				    continue;
			    }

                //Used to get the vendor id and the product ids
                int vendorId = -1;
                int productId = -1;

                //Determine the vendor id and product id
                JOYCAPS caps;
                if(joyGetDevCaps(controllerPort, &caps, sizeof(JOYCAPS)) == JOYERR_NOERROR) 
                {
                    vendorId = caps.wMid;
                    productId = caps.wPid;
                }
            
                //Create a ControllerData object, pass it the device, vendor id and product id
                GameDev2D::ControllerData* controllerData = new GameDev2D::ControllerData((void*)controllerPort, vendorId, productId);
                assert(controllerData != nullptr);

                //Pass the controller data onto the input manager, it will
                GameDev2D::ControllerGeneric* controller = GameDev2D::ServiceLocator::GetInputManager()->HandleMatchedController(controllerData);
                assert(controller != nullptr);

                //Set the controller data's initial state
                controllerData->SetStateInfo(state);

                //Add all the controller buttons
                controllerData->AddButton(XINPUT_GAMEPAD_DPAD_UP);
                controllerData->AddButton(XINPUT_GAMEPAD_DPAD_DOWN);
                controllerData->AddButton(XINPUT_GAMEPAD_DPAD_LEFT);
                controllerData->AddButton(XINPUT_GAMEPAD_DPAD_RIGHT);
                controllerData->AddButton(XINPUT_GAMEPAD_START);
                controllerData->AddButton(XINPUT_GAMEPAD_BACK);
                controllerData->AddButton(XINPUT_GAMEPAD_LEFT_THUMB);
                controllerData->AddButton(XINPUT_GAMEPAD_RIGHT_THUMB);
                controllerData->AddButton(XINPUT_GAMEPAD_LEFT_SHOULDER);
                controllerData->AddButton(XINPUT_GAMEPAD_RIGHT_SHOULDER);
                controllerData->AddButton(XINPUT_GAMEPAD_A);
                controllerData->AddButton(XINPUT_GAMEPAD_B);
                controllerData->AddButton(XINPUT_GAMEPAD_X);
                controllerData->AddButton(XINPUT_GAMEPAD_Y);

                //Add the analog trugger
                controllerData->AddAnalog(VK_PAD_LTRIGGER, 0, 255, 0);
                controllerData->AddAnalog(VK_PAD_RTRIGGER, 0, 255, 0);

                //Add the left analog stick
                controllerData->AddAnalog(VK_PAD_LTHUMB_UP, -32768, 32767, 0);
                controllerData->AddAnalog(VK_PAD_LTHUMB_LEFT, -32768, 32767, 0);

                //Add the right analog stick
                controllerData->AddAnalog(VK_PAD_RTHUMB_UP, -32768, 32767, 0);
                controllerData->AddAnalog(VK_PAD_RTHUMB_LEFT, -32768, 32767, 0);
            }
        }
    }

    void GameWindow::HandleControllerInput()
    {
        //If three seconds have passed, check to see if new controllers have been plugged in
       	if(ServiceLocator::GetPlatformLayer()->GetTicks() > m_LastControllerDetection + 3000) 
        {
            DetectControllers();
	    }  

        //Local variable used in the for loop below
        XINPUT_STATE state;
	
        //Cycle through all the connected controllers and check their input state
        unsigned int deviceIndex = 0;
	    for(unsigned int deviceIndex = 0; deviceIndex < GameDev2D::ServiceLocator::GetInputManager()->GetNumberOfControllers(); deviceIndex++) 
        {
            //Get the controller object and the device identifer from the index
            ControllerGeneric* controller = GameDev2D::ServiceLocator::GetInputManager()->GetControllerForIndex(deviceIndex);
            UINT controllerPort = (UINT)controller->GetDevice();
		
            //Zero the memory of the state struct
            ZeroMemory(&state, sizeof(XINPUT_STATE));

            //Can we get the controller state for this port
            if(XInputGetState(controllerPort, &state) == ERROR_SUCCESS)
            {
                //Handle the button input
                if(state.Gamepad.wButtons != controller->GetControllerData()->GetStateInfo().Gamepad.wButtons)
                {
                    HandleControllerButtonInput(controller, controller->GetControllerData()->GetStateInfo().Gamepad.wButtons, state.Gamepad.wButtons);
                }

                //Handle left trigger input
                if(state.Gamepad.bLeftTrigger != controller->GetControllerData()->GetStateInfo().Gamepad.bLeftTrigger)
                {
                    GameDev2D::ServiceLocator::GetInputManager()->HandleControllerInput(controller, VK_PAD_LTRIGGER, state.Gamepad.bLeftTrigger);
                }

                //Handle right trigger input
                if(state.Gamepad.bRightTrigger != controller->GetControllerData()->GetStateInfo().Gamepad.bRightTrigger)
                {
                    GameDev2D::ServiceLocator::GetInputManager()->HandleControllerInput(controller, VK_PAD_RTRIGGER, state.Gamepad.bRightTrigger);
                }

                //Handle left analog stick x-axis input
                if(state.Gamepad.sThumbLX != controller->GetControllerData()->GetStateInfo().Gamepad.sThumbLX)
                {
                    GameDev2D::ServiceLocator::GetInputManager()->HandleControllerInput(controller, VK_PAD_LTHUMB_LEFT, state.Gamepad.sThumbLX);
                }

                //Handle left analog stick y-axis input
                if(state.Gamepad.sThumbLY != controller->GetControllerData()->GetStateInfo().Gamepad.sThumbLY)
                {
                    GameDev2D::ServiceLocator::GetInputManager()->HandleControllerInput(controller, VK_PAD_LTHUMB_UP, state.Gamepad.sThumbLY);
                }

                //Handle right analog stick x-axis input
                if(state.Gamepad.sThumbRX != controller->GetControllerData()->GetStateInfo().Gamepad.sThumbRX)
                {
                    GameDev2D::ServiceLocator::GetInputManager()->HandleControllerInput(controller, VK_PAD_RTHUMB_LEFT, state.Gamepad.sThumbRX);
                }

                //Handle right analog stick y-axis input
                if(state.Gamepad.sThumbRY != controller->GetControllerData()->GetStateInfo().Gamepad.sThumbRY)
                {
                    GameDev2D::ServiceLocator::GetInputManager()->HandleControllerInput(controller, VK_PAD_RTHUMB_UP, state.Gamepad.sThumbRY);
                }

                //Update the controllers state info
                controller->GetControllerData()->SetStateInfo(state);
            }
            else
            {
                //Notify the Input manager of the unplugged controller
                GameDev2D::ServiceLocator::GetInputManager()->HandleRemovedController((void*)controllerPort);	
            }
        }
    }

    void GameWindow::HandleControllerButtonInput(ControllerGeneric* aController, WORD aLastButtonState, WORD aButtonState)
    {
        for(unsigned int i = 0; i < aController->GetControllerData()->GetNumberOfButtonBindings(); i++) 
        {
            unsigned int binding = aController->GetControllerData()->GetButtonBindingForIndex(i);
            if((aLastButtonState ^ aButtonState) & binding)
            {
			    if(!!(aButtonState & binding)) 
                {
                    GameDev2D::ServiceLocator::GetInputManager()->HandleControllerInput(aController, binding, 1);
			    }
                else 
                {
                    GameDev2D::ServiceLocator::GetInputManager()->HandleControllerInput(aController, binding, 0);
			    }
		    }
	    }
    }

    LRESULT GameWindow::EventHandler(HWND aHandle, UINT aMessage, WPARAM aWParam, LPARAM aLParam)
    {
	    bool useDefault = false;

        //Safety check that the platform layer isn't null
        if(ServiceLocator::GetPlatformLayer() == nullptr)
        {
		    return DefWindowProc(aHandle, aMessage, aWParam, aLParam);
        }

        //Handle the windows message
	    switch (aMessage)
	    {
	        case WM_SIZE:
            {
                if(ServiceLocator::GetPlatformLayer() != nullptr) 
                {
                    int width = LOWORD(aLParam); 
		            int height = HIWORD(aLParam);
                    ((Platform_Windows*)ServiceLocator::GetPlatformLayer())->HandleViewportResize(width, height);
		        }
            }
	        break;

            case WM_SETFOCUS:
            {
                ServiceLocator::GetPlatformLayer()->Resume();
            }
            break;

            case WM_KILLFOCUS:
            {
                ServiceLocator::GetPlatformLayer()->Suspend();
            }
            break;

            case WM_MOVING:
            {
                if(ServiceLocator::GetPlatformLayer()->IsSuspended() == false)
                {
                    ServiceLocator::GetPlatformLayer()->Suspend();
                }
            }
            break;

            case WM_EXITSIZEMOVE:
            {
                ServiceLocator::GetPlatformLayer()->Resume();
            }
            break;

            case WM_SYSCOMMAND:
            {
                //Is the screensaver trying to start? Or is a monitor trying to enter Powersave?
                //Prevent From Happening by returning zero
                switch(aWParam)
                {
                    case SC_SCREENSAVE:             
                    case SC_MONITORPOWER:
                        return 0;
                    default:
                        return DefWindowProc(aHandle, aMessage, aWParam, aLParam);
                }
            }
            break;

	        case WM_MOUSEMOVE:
            {
                int height = ServiceLocator::GetPlatformLayer()->GetHeight();
                int positionX = GET_X_LPARAM(aLParam);
				int positionY = height - GET_Y_LPARAM(aLParam);
                ServiceLocator::GetInputManager()->HandleMouseMoved((float)positionX, (float)positionY, 0, 0);
            }
	        break;

	        case WM_MOUSEWHEEL:
            {
                ServiceLocator::GetInputManager()->HandleScrollWheel(GET_WHEEL_DELTA_WPARAM(aWParam));
            }
	        break;

	        case WM_LBUTTONDOWN:
            {
                int height = ServiceLocator::GetPlatformLayer()->GetHeight();
                int positionX = GET_X_LPARAM(aLParam);
				int positionY = height - GET_Y_LPARAM(aLParam);
                ServiceLocator::GetInputManager()->HandleLeftMouseDown((float)positionX, (float)positionY);
            }
	        break;

	        case WM_LBUTTONUP:
            {
                int height = ServiceLocator::GetPlatformLayer()->GetHeight();
                int positionX = GET_X_LPARAM(aLParam);
				int positionY = height - GET_Y_LPARAM(aLParam);
                ServiceLocator::GetInputManager()->HandleLeftMouseUp((float)positionX, (float)positionY);
            }
	        break;

	        case WM_RBUTTONDOWN:
            {
                int height = ServiceLocator::GetPlatformLayer()->GetHeight();
                int positionX = GET_X_LPARAM(aLParam);
				int positionY = height - GET_Y_LPARAM(aLParam);
                ServiceLocator::GetInputManager()->HandleRightMouseDown((float)positionX, (float)positionY);
            }
	        break;

	        case WM_RBUTTONUP:
            {
                int height = ServiceLocator::GetPlatformLayer()->GetHeight();
                int positionX = GET_X_LPARAM(aLParam);
				int positionY = height - GET_Y_LPARAM(aLParam);
                ServiceLocator::GetInputManager()->HandleRightMouseUp((float)positionX, (float)positionY);
            }
	        break;

	        case WM_TOUCH:
            {
                //TODO: Handle touch input for windows
            }
	        break;

	        case WM_MBUTTONDOWN:
            {
                int height = ServiceLocator::GetPlatformLayer()->GetHeight();
                int positionX = GET_X_LPARAM(aLParam);
				int positionY = height - GET_Y_LPARAM(aLParam);
                ServiceLocator::GetInputManager()->HandleCenterMouseDown((float)positionX, (float)positionY);
            }
	        break;

	        case WM_MBUTTONUP:
            {
                int height = ServiceLocator::GetPlatformLayer()->GetHeight();
                int positionX = GET_X_LPARAM(aLParam);
				int positionY = height - GET_Y_LPARAM(aLParam);
                ServiceLocator::GetInputManager()->HandleCenterMouseUp((float)positionX, (float)positionY);
            }
	        break;

	        case WM_KEYDOWN:
	        case WM_SYSKEYDOWN:
            {
                ServiceLocator::GetInputManager()->HandleKeyDown(MapKey(aWParam, aLParam));
            }
	        break;

	        case WM_KEYUP:
	        case WM_SYSKEYUP:
            {
                ServiceLocator::GetInputManager()->HandleKeyUp(MapKey(aWParam, aLParam));
            }
	        break;

	        case WM_CLOSE:
            {
                ServiceLocator::GetPlatformLayer()->Shutdown();
                useDefault = true;
            }
	        break;

	        case WM_DESTROY:
            {
		        PostQuitMessage(0);
            }
		    break;

	        default:
		        useDefault = true;
		        break;
	    }
	
	    if (useDefault)
        {
		    return DefWindowProc(aHandle, aMessage, aWParam, aLParam);
        }
	    else
        {
		    return 0;
        }
    }
    
    void GameWindow::InitKeyMap()
    {
        for (int i = 0; i < KEY_CODE_COUNT; i++)
        {
		    m_Keymap[i] = KEY_CODE_UNKNOWN;
        }

	    m_Keymap[VK_BACK] = KEY_CODE_BACKSPACE;
	    m_Keymap[VK_TAB] = KEY_CODE_TAB;
	    m_Keymap[VK_CLEAR] = KEY_CODE_CLEAR;
	    m_Keymap[VK_RETURN] = KEY_CODE_RETURN;
	    m_Keymap[VK_PAUSE] = KEY_CODE_PAUSE;
	    m_Keymap[VK_ESCAPE] = KEY_CODE_ESCAPE;
	    m_Keymap[VK_SPACE] = KEY_CODE_SPACE;
	    m_Keymap[0xDE] = KEY_CODE_QUOTE;
	    m_Keymap[0xBC] = KEY_CODE_COMMA;
	    m_Keymap[0xBD] = KEY_CODE_MINUS;
	    m_Keymap[0xBE] = KEY_CODE_PERIOD;
	    m_Keymap[0xBF] = KEY_CODE_SLASH;
	    m_Keymap['0'] = KEY_CODE_0;
	    m_Keymap['1'] = KEY_CODE_1;
	    m_Keymap['2'] = KEY_CODE_2;
	    m_Keymap['3'] = KEY_CODE_3;
	    m_Keymap['4'] = KEY_CODE_4;
	    m_Keymap['5'] = KEY_CODE_5;
	    m_Keymap['6'] = KEY_CODE_6;
	    m_Keymap['7'] = KEY_CODE_7;
	    m_Keymap['8'] = KEY_CODE_8;
	    m_Keymap['9'] = KEY_CODE_9;
	    m_Keymap[0xBA] = KEY_CODE_SEMICOLON;
	    m_Keymap[0xBB] = KEY_CODE_EQUALS;
	    m_Keymap[0xDB] = KEY_CODE_LEFT_BRACKET;
	    m_Keymap[0xDC] = KEY_CODE_BACKSLASH;
	    m_Keymap[VK_OEM_102] = KEY_CODE_LESS;
	    m_Keymap[0xDD] = KEY_CODE_RIGHT_BRACKET;
	    m_Keymap[0xC0] = KEY_CODE_BACKQUOTE;
	    m_Keymap[0xDF] = KEY_CODE_BACKQUOTE;
	    m_Keymap['A'] = KEY_CODE_A;
	    m_Keymap['B'] = KEY_CODE_B;
	    m_Keymap['C'] = KEY_CODE_C;
	    m_Keymap['D'] = KEY_CODE_D;
	    m_Keymap['E'] = KEY_CODE_E;
	    m_Keymap['F'] = KEY_CODE_F;
	    m_Keymap['G'] = KEY_CODE_G;
	    m_Keymap['H'] = KEY_CODE_H;
	    m_Keymap['I'] = KEY_CODE_I;
	    m_Keymap['J'] = KEY_CODE_J;
	    m_Keymap['K'] = KEY_CODE_K;
	    m_Keymap['L'] = KEY_CODE_L;
	    m_Keymap['M'] = KEY_CODE_M;
	    m_Keymap['N'] = KEY_CODE_N;
	    m_Keymap['O'] = KEY_CODE_O;
	    m_Keymap['P'] = KEY_CODE_P;
	    m_Keymap['Q'] = KEY_CODE_Q;
	    m_Keymap['R'] = KEY_CODE_R;
	    m_Keymap['S'] = KEY_CODE_S;
	    m_Keymap['T'] = KEY_CODE_T;
	    m_Keymap['U'] = KEY_CODE_U;
	    m_Keymap['V'] = KEY_CODE_V;
	    m_Keymap['W'] = KEY_CODE_W;
	    m_Keymap['X'] = KEY_CODE_X;
	    m_Keymap['Y'] = KEY_CODE_Y;
	    m_Keymap['Z'] = KEY_CODE_Z;
	    m_Keymap[VK_DELETE] = KEY_CODE_DELETE;

	    m_Keymap[VK_NUMPAD0] = KEY_CODE_KP0;
	    m_Keymap[VK_NUMPAD1] = KEY_CODE_KP1;
	    m_Keymap[VK_NUMPAD2] = KEY_CODE_KP2;
	    m_Keymap[VK_NUMPAD3] = KEY_CODE_KP3;
	    m_Keymap[VK_NUMPAD4] = KEY_CODE_KP4;
	    m_Keymap[VK_NUMPAD5] = KEY_CODE_KP5;
	    m_Keymap[VK_NUMPAD6] = KEY_CODE_KP6;
	    m_Keymap[VK_NUMPAD7] = KEY_CODE_KP7;
	    m_Keymap[VK_NUMPAD8] = KEY_CODE_KP8;
	    m_Keymap[VK_NUMPAD9] = KEY_CODE_KP9;
	    m_Keymap[VK_DECIMAL] = KEY_CODE_KP_PERIOD;
	    m_Keymap[VK_DIVIDE] = KEY_CODE_KP_DIVIDE;
	    m_Keymap[VK_MULTIPLY] = KEY_CODE_KP_MULTIPLY;
	    m_Keymap[VK_SUBTRACT] = KEY_CODE_KP_MINUS;
	    m_Keymap[VK_ADD] = KEY_CODE_KP_PLUS;

	    m_Keymap[VK_UP] = KEY_CODE_UP;
	    m_Keymap[VK_DOWN] = KEY_CODE_DOWN;
	    m_Keymap[VK_RIGHT] = KEY_CODE_RIGHT;
	    m_Keymap[VK_LEFT] = KEY_CODE_LEFT;
	    m_Keymap[VK_INSERT] = KEY_CODE_INSERT;
	    m_Keymap[VK_HOME] = KEY_CODE_HOME;
	    m_Keymap[VK_END] = KEY_CODE_END;
	    m_Keymap[VK_PRIOR] = KEY_CODE_PAGEUP;
	    m_Keymap[VK_NEXT] = KEY_CODE_PAGEDOWN;

	    m_Keymap[VK_F1] = KEY_CODE_F1;
	    m_Keymap[VK_F2] = KEY_CODE_F2;
	    m_Keymap[VK_F3] = KEY_CODE_F3;
	    m_Keymap[VK_F4] = KEY_CODE_F4;
	    m_Keymap[VK_F5] = KEY_CODE_F5;
	    m_Keymap[VK_F6] = KEY_CODE_F6;
	    m_Keymap[VK_F7] = KEY_CODE_F7;
	    m_Keymap[VK_F8] = KEY_CODE_F8;
	    m_Keymap[VK_F9] = KEY_CODE_F9;
	    m_Keymap[VK_F10] = KEY_CODE_F10;
	    m_Keymap[VK_F11] = KEY_CODE_F11;
	    m_Keymap[VK_F12] = KEY_CODE_F12;
	    m_Keymap[VK_F13] = KEY_CODE_F13;
	    m_Keymap[VK_F14] = KEY_CODE_F14;
	    m_Keymap[VK_F15] = KEY_CODE_F15;

	    m_Keymap[VK_NUMLOCK] = KEY_CODE_NUMLOCK;
	    m_Keymap[VK_CAPITAL] = KEY_CODE_CAPSLOCK;
	    m_Keymap[VK_SCROLL] = KEY_CODE_SCROLLOCK;
	    m_Keymap[VK_RSHIFT] = KEY_CODE_RIGHT_SHIFT;
	    m_Keymap[VK_LSHIFT] = KEY_CODE_LEFT_SHIFT;
	    m_Keymap[VK_RCONTROL] = KEY_CODE_RIGHT_CTRL;
	    m_Keymap[VK_LCONTROL] = KEY_CODE_LEFT_CTRL;
	    m_Keymap[VK_RMENU] = KEY_CODE_RIGHT_ALT;
	    m_Keymap[VK_LMENU] = KEY_CODE_LEFT_ALT;
	    m_Keymap[VK_RWIN] = KEY_CODE_RIGHT_COMMAND;
	    m_Keymap[VK_LWIN] = KEY_CODE_LEFT_COMMAND;
    }

    KeyCode GameWindow::MapKey(WPARAM aWParam, LPARAM aLParam)
    {
        switch (aWParam) 
        {
				case VK_CONTROL:
					if(aLParam & (1<<24))
                    {
						aWParam = VK_RCONTROL;
                    }
					else
                    {
						aWParam = VK_LCONTROL;
                    }
					break;
				case VK_SHIFT:
					//Use MapVirtualKey to determine whether it's LSHIFT or RSHIFT by scancode.
					UINT scancode = (aLParam & 0x00ff0000) >> 16;
					aWParam = MapVirtualKey(scancode, MAPVK_VSC_TO_VK_EX);
					break;
	    }
	    return m_Keymap[(unsigned int)aWParam];
    }
}