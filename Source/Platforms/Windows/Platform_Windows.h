#ifndef PLATFORM_WINDOWS_H
#define PLATFORM_WINDOWS_H

#include "../PlatformLayer.h"


namespace GameDev2D
{
    //Inheritis from the VideoModeInfo struct and adds a fullscreen flag
    struct VideoModeInfo_Win32 : public VideoModeInfo
    {
		bool fullScreen;
    };

    //Forward declarations
    class GameWindow;

    //The Platform_OSX class inherits from the PlatformLayer class. This class provides an OSX specific platform
    //implementation for the game loop, input, file io, window creation, sizing, dialog boxes and resume, suspend
    //and shutdown event handling.
    //This class dispatches the following events:
    // SHUTDOWN_EVENT
    // SUSPEND_EVENT
    // RESUME_EVENT
    // LOW_MEMORY_WARNING_EVENT (iOS Only)
    // ORIENTATION_CHANGED_EVENT (iOS Only)
    // RESIZE_EVENT
    // FULLSCREEN_EVENT
    class Platform_Windows : public PlatformLayer
    {
    public:
        //The constructor takes in the Window the application is in, the window title and the target frame rate.
        Platform_Windows(GameWindow* gameWindow, const char* windowTitle, int frameRate);
        ~Platform_Windows();

        //Returns the Windows platform type
        PlatformType GetPlatformType();
        
        //Intiailizes the Platform_OSX layer to a initial size, fullscreen, and vertical sync settings.
        void Init(unsigned int width, unsigned int height, bool fullscreen, bool verticalSync);

        //Methods used to Update and Draw the application, inherited from PlatformLayer
        bool Update();
        void Draw();

        //Set the window's title
        void SetWindowTitle(const char* title);

        //Sets the current video mode, this will set a flag to trigger a resize event, or fullscreen
        //change at the start of the next Update(), inherited from PlatformLayer
        void SetVideoModeInfo(VideoModeInfo* videoModeInfo);
        
        //Call this method to resize the platform's window, inherited from PlatformLayer
        void ResizeTo(unsigned int width, unsigned int height);

        //Returns wether the application is in fullscreen mode or not, inherited from PlatformLayer
        bool IsFullscreen();

        //Sets the application to be in fullscreen mode or not, inherited from PlatformLayer
        void SetIsFullscreen(bool isFullscreen);
        
        //Returns wether the application has vertical sync enabled or not
        bool IsVerticalSyncEnabled();

        //Sets wether the application should have vertical sync enabled or not
        void EnabledVerticalSync(bool isEnabled);

        //Platform specific methods to determine what input is available, inherited from PlatformLayer
        bool HasMouseInput();
        bool HasKeyboardInput();
        bool HasTouchInput();
        bool HasControllerInput();
        bool HasAccelerometerInput();
        bool HasGyroscopeInput();
        
        //Returns wether the mouse cursor is visible
        bool IsMouseCursorVisible();

        //Methods to show and hide the mouse cursor
        void ShowMouseCursor();
        void HideMouseCursor();
        
        //Sets the current position of the mouse cursor
        void SetMouseCursorPosition(int x, int y);
        
        //Touch input is supported on Windows, BUT I haven't added support for it yet, methods do not do anything
        bool IsMultipleTouchEnabled();
        void SetMultipleTouchEnabled(bool enabled);
        
        //Accelerometer input is NOT supported on OSX, methods do not do anything
        bool IsAccelerometerEnabled();
        void SetAccelerometerEnabled(bool enabled);
        void SetAccelerometerUpdateInterval(double interval);
        
        //Gyroscope input is NOT supported on OSX, methods do not do anything
        bool IsGyroscopeEnabled();
        void SetGyroscopeEnabled(bool enabled);
        void SetGyroscopeUpdateInterval(double interval);
        
        //Returns a path to the directory where files can be written to, inherited from PlatformLayer
        string GetWorkingDirectory();

        //Returns a path to the directory that the application is in, inherited from PlatformLayer
        string GetApplicationDirectory();
        
        //Returns a path for a resource in a specific directory, inherited from PlatformLayer
        string GetPathForResourceInDirectory(const char* fileName, const char* fileType, const char* directory);
        
        //Returns wether the file exists at the path, inherited from PlatformLayer
        bool DoesFileExistAtPath(const string& path);
        
        //Presents a platform specific native dialog box, inherited from PlatformLayer
        int PresentNativeDialogBox(const char* title, const char* message, NativeDialogType type);

        //Returns platform specific details and the operating system name and version, and
        //hardware model (if available). Inherited from PlatformLayer
        void PlatformName(string& name);
        void PlatformModel(string& model);
        void PlatformVersion(string& version);
      
        //Returns how much memory (RAM) is installed on the machine, inherited from PlatformLayer
        unsigned long long MemoryInstalled();
        
        //Hard drive specific methods, inherited from PlatformLayer
        unsigned long long DiskSpaceUsed();
        unsigned long long DiskSpaceFree();
        unsigned long long DiskSpaceTotal();

        //Returns the number of available CPUs on the machine, inherited from PlatformLayer
        unsigned int CpuCount();
        
        //Returns the number of ticks since the start of the application, inherited from PlatformLayer
        unsigned int GetTicks();
    
    protected:
        //This method applies the changes set in the SetVideoModeInfo() method at the start of the
        //Update() method. Inherited from PlatformLayer
        void ApplyVideoModeChanges(VideoModeInfo* videoModeInfo);
        
        //This handles the viewport and back buffer resizing when the platform's window is resized.
        //Inherited from PlatformLayer.
        void HandleViewportResize(unsigned int width, unsigned int height);

        //The GameWindow needs to access protected methods
        friend class GameWindow;

    private:
        //Member variables
        GameWindow* m_GameWindow;
        bool m_IsFullScreen;
        bool m_IsMouseCursorVisible;
        double m_PerformanceCounterFrequency;
        VideoModeInfo_Win32 m_VideoModeChangeInfo;
    };
}

#endif