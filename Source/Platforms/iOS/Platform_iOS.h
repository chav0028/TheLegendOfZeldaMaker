//
//  Platform_iOS.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-03-10.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__Platform_iOS__
#define __GameDev2D__Platform_iOS__

#include "PlatformLayer.h"


namespace GameDev2D
{
    //The Platform_iOS class inherits from the PlatformLayer class. This class provides an iOS specific platform
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
    class Platform_iOS : public PlatformLayer
    {
    public:
        //The constructor takes in iOS View the application is in and the target frame rate.
        Platform_iOS(void* window, int frameRate);
        ~Platform_iOS();
        
        //Returns the iOS platform type
        PlatformType GetPlatformType();
        
        //Methods used to Update and Draw the application, inherited from PlatformLayer
        bool Update();
        void Draw();
        
        //Handles resume and suspend events from OSX, inherited from PlatformLayer
        void Resume();
        void Suspend();
        
        //Returns the current scale of the window, this can be 1.0f or 2.0f since iOS supports
        //retina displays, inherited from PlatformLayer
        float GetScale();
        
        //Dispatches an event when the orientation changes
        void OrientationChanged(Orientation orientation);
        
        //Sets the current video mode, this will set a flag to trigger a resize event, or fullscreen
        //change at the start of the next Update(), inherited from PlatformLayer
        void SetVideoModeInfo(VideoModeInfo* videoModeInfo);
        
        //Set the supported orientation mask, these are the orientations that the application supports
        void SetSupportedOrientationMask(unsigned int orientationMask);
        
        //Returns the supported orientation mask
        unsigned int GetSupportedOrientationMask();
        
        //Returns true, iOS only supports fullscreen applications
        bool IsFullscreen();
        
        //Returns wether the iOS device is a simulator or not
        bool IsSimulator();
        
        //Platform specific methods to determine what input is available, inherited from PlatformLayer
        bool HasMouseInput();
        bool HasKeyboardInput();
        bool HasTouchInput();
        bool HasControllerInput();
        bool HasAccelerometerInput();
        bool HasGyroscopeInput();
        
        //Returns wether multi touch is enabled, by default only the screen only accepts 1 finger touch at a
        //time, with multi touch enalbed in allows up to 5 fingers on iPhone, 10+ on the iPad.
        bool IsMultipleTouchEnabled();
        
        //Sets wether multi touch is enabled
        void SetMultipleTouchEnabled(bool enabled);
        
        //Returns wether the accelerometer is enabled
        bool IsAccelerometerEnabled();
        
        //Sets wether the accelerometer is enabled or not
        void SetAccelerometerEnabled(bool enabled);
        
        //Sets the interval to update the accelerometer data
        void SetAccelerometerUpdateInterval(double interval);
        
        //Returns wether the gyroscope is enabled
        bool IsGyroscopeEnabled();
        
        //Sets wether the gyroscope is enabled or not
        void SetGyroscopeEnabled(bool enabled);
        
        //Sets the interval to update the gyroscope data
        void SetGyroscopeUpdateInterval(double interval);
        
        //Returns a path to the directory where files can be written to, inherited from PlatformLayer
        string GetWorkingDirectory();
        
        //Returns a path to the directory that the application is in, inherited from PlatformLayer
        string GetApplicationDirectory();
        
        //Returns a path for a resource in a specific directory, inherited from PlatformLayer
        string GetPathForResourceInDirectory(const char* fileName, const char* fileType, const char* directory);
        
        //Returns wether the file exists at the path, inherited from PlatformLayer
        bool DoesFileExistAtPath(const string& path);
        
        //Dispatches a low memory warning, if you receive this event you should unload any un-needed resources
        void LowMemoryWarning();
        
        //Vibrates the iOS device
        void Vibrate();
        
        //Presents a platform specific native dialog box, inherited from PlatformLayer - currently unfinished
        int PresentNativeDialogBox(const char* title, const char* message, NativeDialogType type);

        //Hard drive specific methods, inherited from PlatformLayer
        void PlatformName(string& name);
        void PlatformModel(string& model);
        void PlatformVersion(string& version);
      
        //Returns wether memory tracking is enabled, which means is the library MemTrack tracking
        //dynamic memory allocations, inherited from PlatformLayer
        bool IsMemoryTrackingEnabled();
        
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
        //iOS requires specific render buffer storage, inherited from PlatformLayer
        void SetRenderBufferStorage();
        
        //This method applies the changes set in the SetVideoModeInfo() method at the start of the
        //Update() method. Inherited from PlatformLayer
        void ApplyVideoModeChanges(VideoModeInfo* videoModeInfo);
        
        //This handles the viewport and back buffer resizing when the platform's window is resized.
        //Inherited from PlatformLayer.
        void HandleViewportResize(unsigned int width, unsigned int height);
    
    private:
        //Member variables
        void* m_GameView;
        unsigned int m_SupportedOrientationMask;
        unsigned long long m_InitTime;
        VideoModeInfo m_VideoModeChangeInfo;
    };
}

#endif /* defined(__GameDev2D__Platform_iOS__) */
