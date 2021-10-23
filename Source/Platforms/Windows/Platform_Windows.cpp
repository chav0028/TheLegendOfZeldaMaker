
#include "Platform_Windows.h"
#include "App/GameWindow.h"
#include "../../Services/ServiceLocator.h"
#include "../../Events/Platform/ResizeEvent.h"
#include "../../Events/Platform/FullscreenEvent.h"



namespace GameDev2D
{
    Platform_Windows::Platform_Windows(GameWindow* aGameWindow, const char* aWindowTitle, int aFrameRate) : PlatformLayer("Platform_Windows", aFrameRate),
        m_GameWindow(aGameWindow),
        m_IsFullScreen(false),
        m_IsMouseCursorVisible(false)
    {
        //Set the window's title
        SetWindowTitle(aWindowTitle);

        //Cache the performance counter frequency
        LARGE_INTEGER largeInt;
	    QueryPerformanceFrequency(&largeInt);
	    m_PerformanceCounterFrequency = double(largeInt.QuadPart) / 1000.0;
    }

    Platform_Windows::~Platform_Windows()
    {

    }
    
    PlatformType Platform_Windows::GetPlatformType()
    {
        return PlatformType_Win32;
    }
        
    void Platform_Windows::Init(unsigned int aWidth, unsigned int aHeight, bool aFullscreen, bool aVerticalSync)
    {
        //Initialize the base class
        PlatformLayer::Init();

        //Set the window's desired width and height, and fullscreen settings
        VideoModeInfo_Win32 videoModeInfo;
        videoModeInfo.width = aWidth;
        videoModeInfo.height = aHeight;
        videoModeInfo.fullScreen = aFullscreen;
        ApplyVideoModeChanges(&videoModeInfo);

        //Lastly apply the vertical sync settings
        EnabledVerticalSync(aVerticalSync);
    }

    bool Platform_Windows::Update()
    {
        //If the game isn't running anymore, return false
        if(m_IsRunning == false)
        {
            return false;
        }
        
        //Sleep the game timer
        DoSleep();
        
        //Handle video mode changes
        if(m_VideoModeChangeInfo.needsChange == true)
        {
            ApplyVideoModeChanges(&m_VideoModeChangeInfo);
            m_VideoModeChangeInfo.needsChange = false;
        }

        //Safety check the game window and handle controller input
        if(m_GameWindow != nullptr)
        {
            m_GameWindow->HandleControllerInput();
        }

        //Calculate the tick (milleseconds since last update) and update the game
        Tick();
        
        //Return wether the game is still running or not
        return m_IsRunning;
    }

    void Platform_Windows::Draw()
    {
        //If the application isn't suspended, clear the OpenGL view
        if(m_IsSuspended == false)
        {
            ServiceLocator::GetGraphics()->Clear();
        }

        //Draw the services
        ServiceLocator::DrawServices();

        //If the application isn't suspended, flush the opengl buffer
        if(m_IsSuspended == false)
        {
            //Lastly flush the draw buffer
            if(m_GameWindow != nullptr)
            {
                m_GameWindow->SwapDrawBuffer();
            }
        }
    }

    void Platform_Windows::SetWindowTitle(const char* aTitle)
    {
        SetWindowTextA(m_GameWindow->GetWindowHandle(), aTitle);
    }

    void Platform_Windows::ResizeTo(unsigned int aWidth, unsigned int aHeight)
    {
        VideoModeInfo_Win32 videoModeInfo;
        videoModeInfo.width = aWidth;
        videoModeInfo.height = aHeight;
        videoModeInfo.fullScreen = m_IsFullScreen;
        SetVideoModeInfo(&videoModeInfo);
    }
    
    void Platform_Windows::HandleViewportResize(unsigned int aWidth, unsigned int aHeight)
    {
        //Safety check that the width and height have changed
        if(aWidth != m_Width || aHeight != m_Height)
        {
            //Set the width and height
            m_Width = aWidth;
            m_Height = aHeight;
            
            //Resize the Graphics service
            ServiceLocator::GetGraphics()->Resize(m_Width, m_Height);
            
            //Dispatch a Resize event
            DispatchEvent(new ResizeEvent(vec2(m_Width, m_Height)));
        }
    }
    
    void Platform_Windows::SetVideoModeInfo(VideoModeInfo* aVideoModeInfo)
    {
        VideoModeInfo_Win32* videoModeInfoWin32 = (VideoModeInfo_Win32*)aVideoModeInfo;
    	m_VideoModeChangeInfo.needsChange = true;
        m_VideoModeChangeInfo.width = videoModeInfoWin32->width;
        m_VideoModeChangeInfo.height = videoModeInfoWin32->height;
        m_VideoModeChangeInfo.fullScreen = videoModeInfoWin32->fullScreen;
    }

    void Platform_Windows::ApplyVideoModeChanges(VideoModeInfo* aVideoModeInfo)
    {
        //Cast the video mode info struct to the win32 video mode info struct
        VideoModeInfo_Win32* videoModeInfoWin32 = (VideoModeInfo_Win32*)aVideoModeInfo;

        //Will we need to dispatch a fullscreen event
        bool dispatchFullscreenEvent = m_IsFullScreen != videoModeInfoWin32->fullScreen;

        //Is fullscreen enabled?
        if(videoModeInfoWin32->fullScreen == true)
        {  
            //Set the fullscreen flag to true
            m_IsFullScreen = true;

            //Update the window style for fullscreen display
            SetWindowLongPtr(m_GameWindow->GetWindowHandle(), GWL_STYLE, WS_SYSMENU | WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE);

            //Set the fullscreen display settings
            DEVMODE displaySettings;
            EnumDisplaySettings (NULL, 0, &displaySettings);
            displaySettings.dmPelsWidth = videoModeInfoWin32->width;
            displaySettings.dmPelsHeight = videoModeInfoWin32->height;
            displaySettings.dmBitsPerPel = 32;
            displaySettings.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

            //Enable fullscreen mode, if the assert below is hit, the resolution passed in is NOT supported in fullscreen mode
            bool success = ChangeDisplaySettings(&displaySettings, CDS_FULLSCREEN) == DISP_CHANGE_SUCCESSFUL;
            assert(success == true);

            //Move the window into position
            MoveWindow(m_GameWindow->GetWindowHandle(), 0, 0, videoModeInfoWin32->width, videoModeInfoWin32->height, TRUE);
        }
        else
        {
            //Exit fullscreen mode
            bool success = ChangeDisplaySettings(0, 0) == DISP_CHANGE_SUCCESSFUL;
            assert(success == true);

            //Set the fullscreen flag to false
            m_IsFullScreen = false;

            //Set the window rect
            RECT rect;
            rect.left = 0;
            rect.top = 0;
            rect.right = videoModeInfoWin32->width;
            rect.bottom = videoModeInfoWin32->height;

            //Calculate the position of the window, centered on screen
            int monitorResolutionX = GetSystemMetrics(SM_CXSCREEN);
            int monitorResolutionY = GetSystemMetrics(SM_CYSCREEN);
            int x = (monitorResolutionX - videoModeInfoWin32->width) / 2;
            int y = (monitorResolutionY - videoModeInfoWin32->height) / 2;

            //Restore the window's 'windowed' style, then set the window's rect and position on screen
            SetWindowLongPtr(m_GameWindow->GetWindowHandle(), GWL_STYLE, WS_CAPTION | WS_POPUPWINDOW | WS_VISIBLE);
            AdjustWindowRect(&rect, WS_CAPTION | WS_POPUPWINDOW, FALSE);
            MoveWindow(m_GameWindow->GetWindowHandle(), x, y, rect.right-rect.left, rect.bottom-rect.top, TRUE);
        }
        
        //Dispatch a fullscreen event
        if(dispatchFullscreenEvent == true)
        {
            DispatchEvent(new FullscreenEvent(m_IsFullScreen));
        }
    }

    void Platform_Windows::SetIsFullscreen(bool aIsFullscreen)
    {
        VideoModeInfo_Win32 videoModeInfo;
        videoModeInfo.width = m_Width;
        videoModeInfo.height = m_Height;
        videoModeInfo.fullScreen = aIsFullscreen;
        SetVideoModeInfo(&videoModeInfo);
    }

    bool Platform_Windows::IsFullscreen()
    {
        return m_IsFullScreen;
    }
    
    void Platform_Windows::EnabledVerticalSync(bool aIsEnabled)
    {
        if(m_GameWindow != nullptr)
        {
            return m_GameWindow->EnableVerticalSync(aIsEnabled);
        }
    }
    
    bool Platform_Windows::IsVerticalSyncEnabled()
    {
        if(m_GameWindow != nullptr)
        {
            return m_GameWindow->IsVerticalSyncEnabled();
        }
        return false;
    }
    
    bool Platform_Windows::HasMouseInput()
    {
        return true;
    }
    
    bool Platform_Windows::HasKeyboardInput()
    {
        return true;
    }
    
    bool Platform_Windows::HasTouchInput()
    {
        //TODO:
        return false;
    }
    
    bool Platform_Windows::HasControllerInput()
    {
        return ServiceLocator::GetInputManager()->GetNumberOfControllers() > 0;
    }
    
    bool Platform_Windows::HasAccelerometerInput()
    {
        return false;
    }
    
    bool Platform_Windows::HasGyroscopeInput()
    {
        return false;
    }
    
    bool Platform_Windows::IsMouseCursorVisible()
    {
        return m_IsMouseCursorVisible;
    }
    
    void Platform_Windows::ShowMouseCursor()
    {
        ShowCursor(true);
        m_IsMouseCursorVisible = true;
    }
    
    void Platform_Windows::HideMouseCursor()
    {
        ShowCursor(false);
        m_IsMouseCursorVisible = false;
    }
    
    void Platform_Windows::SetMouseCursorPosition(int aX, int aY)
    {
        if(m_GameWindow != nullptr)
        {
	        POINT point;
	        point.x = aX;
	        point.y = aY;
            ClientToScreen(m_GameWindow->GetWindowHandle(), &point);
	        SetCursorPos(point.x, point.y);
        }
    }
    
    bool Platform_Windows::IsMultipleTouchEnabled()
    {
        //TODO:
        return false;
    }
    
    void Platform_Windows::SetMultipleTouchEnabled(bool aEnabled)
    {
        //TODO:
    }
    
    bool Platform_Windows::IsAccelerometerEnabled()
    {
        return false;
    }
    
    void Platform_Windows::SetAccelerometerEnabled(bool aEnabled)
    {
        //Unused on Win32
    }
    
    void Platform_Windows::SetAccelerometerUpdateInterval(double aInterval)
    {
        //Unused on Win32
    }
    
    bool Platform_Windows::IsGyroscopeEnabled()
    {
        return false;
    }
    
    void Platform_Windows::SetGyroscopeEnabled(bool aEnabled)
    {
        //Unused on Win32
    }
    
    void Platform_Windows::SetGyroscopeUpdateInterval(double aInterval)
    {
        //Unused on Win32
    }
    
    string Platform_Windows::GetWorkingDirectory()
    {
        return GetApplicationDirectory();
    }
    
    string Platform_Windows::GetApplicationDirectory()
    {
        char currentDir[MAX_PATH];
        currentDir, GetModuleFileNameA( NULL, currentDir, MAX_PATH );
        PathRemoveFileSpecA(currentDir);
        return string(currentDir);
    }
    
    string Platform_Windows::GetPathForResourceInDirectory(const char* aFileName, const char* aFileType, const char* aDirectory)
    {
        string path = string(GetApplicationDirectory());
        path += "\\Assets\\";
        path += string(aDirectory);
        path += "\\";
        path += string(aFileName);
        path += ".";
        path += string(aFileType);
        return path;
    }
    
    bool Platform_Windows::DoesFileExistAtPath(const string& aPath)
    {
        DWORD fileAttributes = GetFileAttributesA(aPath.c_str());
        return (fileAttributes != INVALID_FILE_ATTRIBUTES && !(fileAttributes & FILE_ATTRIBUTE_DIRECTORY));
    }

    int Platform_Windows::PresentNativeDialogBox(const char* aTitle, const char* aMessage, NativeDialogType aType)
    {   
        //Determine the type of windows dialog box to display
        unsigned int type = 0;
        switch (aType)
        {
        case GameDev2D::NativeDialogOk:
            type = MB_OK;
            break;
        case GameDev2D::NativeDialogOkCancel:
            type = MB_OKCANCEL;
            break;
        case GameDev2D::NativeDialogYesNo:
            type = MB_YESNO;
            break;
        default:
            break;
        }

        //Get the result of the dialog box
        int result = MessageBoxA(m_GameWindow->GetWindowHandle(), aMessage, aTitle, type);

        switch (aType)
        {
        case GameDev2D::NativeDialogOk:
            if (result == IDOK)
            {
                return 0;
            }
            break;
        case GameDev2D::NativeDialogOkCancel:
            if (result == IDOK)
            {
                return 0;
            }
            else if (result == IDCANCEL)
            {
                return 1;
            }
            break;
        case GameDev2D::NativeDialogYesNo:
            if (result == IDYES)
            {
                return 0;
            }
            else if (result == IDNO)
            {
                return 1;
            }
            break;
            break;
        default:
            break;
        }

        //Return the result
        return 0;
    }

    void Platform_Windows::PlatformName(string& aName)
    {
        aName = "Windows";
    }
    
    void Platform_Windows::PlatformModel(string& aModel)
    {
        aModel = "";
    }

    void Platform_Windows::PlatformVersion(string& aVersion)
    {
        //Setup and initialize version struct
        OSVERSIONINFOA osVersion;
        ZeroMemory(&osVersion, sizeof(OSVERSIONINFOA));
        osVersion.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);

#pragma warning(push)
#pragma warning(disable : 4996)

        //Get the OS version
        GetVersionExA(&osVersion);

#pragma warning(pop)

        //String stream used to convert the numeric version
        stringstream version;

        //Convert the version to a string
        if(osVersion.dwMajorVersion == 6 && osVersion.dwMinorVersion == 3)
        {
            version << "8.1";
        }
        else if(osVersion.dwMajorVersion == 6 && osVersion.dwMinorVersion == 2)
        {
            version << "8";
        }
        else if(osVersion.dwMajorVersion == 6 && osVersion.dwMinorVersion == 1)
        {
            version << "7";
        }
        else if(osVersion.dwMajorVersion == 6 && osVersion.dwMinorVersion == 0)
        {
            version << "Vista";
        }
        else if(osVersion.dwMajorVersion == 5 && (osVersion.dwMinorVersion == 1 || osVersion.dwMinorVersion == 2))
        {
            version << "XP";
        }
        else if(osVersion.dwMajorVersion == 5 && osVersion.dwMinorVersion == 0)
        {
            version << "2000";
        }

        //Set the return version
        version << " (Build " << osVersion.dwBuildNumber << ": " << osVersion.szCSDVersion << ")";
        aVersion = string(version.str());
    }
    
    unsigned long long Platform_Windows::MemoryInstalled()
    {
        unsigned long long physicalMemory = 0;
        GetPhysicallyInstalledSystemMemory(&physicalMemory);
        physicalMemory *= 1024;
        return physicalMemory;
    }
    
    unsigned long long Platform_Windows::DiskSpaceUsed()
    {
        return DiskSpaceTotal() - DiskSpaceFree();
    }
    
    unsigned long long Platform_Windows::DiskSpaceFree()
    {
        unsigned __int64 lpFreeBytesAvailableToCaller, lpTotalNumberOfBytes, lpTotalNumberOfFreeBytes;
        GetDiskFreeSpaceExA ("C:", (PULARGE_INTEGER)&lpFreeBytesAvailableToCaller, (PULARGE_INTEGER)&lpTotalNumberOfBytes, (PULARGE_INTEGER)&lpTotalNumberOfFreeBytes);
        return lpTotalNumberOfFreeBytes;
    }
    
    unsigned long long Platform_Windows::DiskSpaceTotal()
    {
        unsigned __int64 lpFreeBytesAvailableToCaller, lpTotalNumberOfBytes, lpTotalNumberOfFreeBytes;
        GetDiskFreeSpaceExA ("C:", (PULARGE_INTEGER)&lpFreeBytesAvailableToCaller, (PULARGE_INTEGER)&lpTotalNumberOfBytes, (PULARGE_INTEGER)&lpTotalNumberOfFreeBytes);
        return lpTotalNumberOfBytes;
    }
    
    unsigned int Platform_Windows::CpuCount()
    {
        SYSTEM_INFO sysinfo;
        GetSystemInfo( &sysinfo );
        unsigned int cpuCount = sysinfo.dwNumberOfProcessors;
        return cpuCount;
    }
    
    unsigned int Platform_Windows::GetTicks()
    {
	    LARGE_INTEGER largeInt;
	    QueryPerformanceCounter(&largeInt);
        unsigned int ticks = (unsigned int)(largeInt.QuadPart / m_PerformanceCounterFrequency);
	    return ticks;
    }
}