//
//  PlatformLayer.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-03-10.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__PlatformLayer__
#define __GameDev2D__PlatformLayer__

#include "FrameworkConfig.h"
#include "../Core/BaseObject.h"
#include "../Events/EventDispatcher.h"
#include "../Platforms/PlatformLayer.h"


using namespace std;

namespace GameDev2D
{
    //Forward Declarations
    struct VideoModeInfo;
    class Scene;

    //Supported platforms
    enum PlatformType
    {
        PlatformType_Unknown = 0,
        PlatformType_iOS,
        PlatformType_OSX,
        PlatformType_Win32,
    };
    
    //Supported orientations (only iOS)
    enum Orientation
    {
        OrientationUnknown = 0,
        OrientationPortrait,
        OrientationLandscape
    };

    //Supported dialog types
    enum NativeDialogType
    {
        NativeDialogOk = 0,
        NativeDialogOkCancel,
        NativeDialogYesNo
    };
    
    //The PlatformLayer class is an abstract layer for all platforms this framework supports. It helps standardize the
    //game loop, input, file io, window creation, sizing, dialog boxes and resume, suspend and shutdown event handling.
    //This class dispatches the following events:
    // SHUTDOWN_EVENT
    // SUSPEND_EVENT
    // RESUME_EVENT
    // LOW_MEMORY_WARNING_EVENT (iOS Only)
    // ORIENTATION_CHANGED_EVENT (iOS Only)
    // RESIZE_EVENT
    // FULLSCREEN_EVENT
    class PlatformLayer : public BaseObject, public EventDispatcher
    {
    public:
        //Constructs a Platform layer with a target frame rate
        PlatformLayer(const string& platformType, int frameRate);
        virtual ~PlatformLayer();
        
        //Initializes the platform layer, must be called directly after the constructor. Can be overridden.
        virtual void Init();
        
        //Set's the root scene, that is the initial Scene that is displayed
        void SetRootScene(Scene* rootScene);
        
        //Returns the Platform type, abstract, must be implemented by an inheriting class
        virtual PlatformType GetPlatformType() = 0;
        
        //The Update() and Draw() methods are abstract and must be implemented by an inheriting class
        virtual bool Update() = 0;
		virtual void Draw() = 0;
        
        //The GameLoop() methodis responsible for calling the Update() and Draw() methods
        //if the application isn't in a suspended state
        bool GameLoop();
        
        //The Resume() and Suspend() methods are called when the application gains or loses focus,
        //they will each, respectively, dispatch Resume and Suspend events. Can be overridden.
        virtual void Resume();
        virtual void Suspend();
        
        //The Shutdown() method is called when the Platform receives a notification that it will be
        //terminated. The Shutdown() method will dispatch a shutdown event.
        void Shutdown();
        
        //Returns the current width and height of our platform's window. Can be overridden.
        virtual int GetWidth();
        virtual int GetHeight();
        
        //Returns the scale of the platform's window, by default this returns 1.0f, but on iOS and
        //OSX this can return 2.0f for retina displays. Can be overridden.
        virtual float GetScale();
        
        //Call this method to resize the platform's window. Can be overridden.
        virtual void ResizeTo(unsigned int width, unsigned int height); 
        
        //Returns the last Update() method's delta tick count
        unsigned int GetDeltaTicks();
        
        //Returns the last Update() method's delta time (in seconds)
        double GetDeltaTime();
        
        //Returns how much time has elapsed since the start of the application (in seconds)
        double GetElapsedTime();
        
        //Returns the current number of frames per second
        unsigned int GetFramesPerSecond();
        
        //Call this to set the platform's target frame rate
        void SetTargetFramerate(unsigned int framerate);
        
        //Returns wether the application is in fullscreen mode or not. Can be overridden.
        virtual bool IsFullscreen();
        
        //Sets the application to be in fullscreen mode or not. Can be overridden.
        virtual void SetIsFullscreen(bool isFullscreen);

        //Returns wether the application is suspended or not. Can be overridden.
        virtual bool IsSuspended();
        
        //Sets the current video mode, this will set a flag to trigger a resize event, or fullscreen
        //change at the start of the next Update(). Abstract, must be overridden by an inheriting class
        virtual void SetVideoModeInfo(VideoModeInfo* videoModeInfo) = 0;
        
        //Platform specific methods to determine what input is available, abstract methods
        //that must be overridden by an inheriting class
        virtual bool HasMouseInput() = 0;
        virtual bool HasKeyboardInput() = 0;
        virtual bool HasTouchInput() = 0;
        virtual bool HasControllerInput() = 0;
        virtual bool HasAccelerometerInput() = 0;
        virtual bool HasGyroscopeInput() = 0;
        
        //Multiple touch specific methods, abstract, must be implemented by an inheriting class
        virtual bool IsMultipleTouchEnabled() = 0;
        virtual void SetMultipleTouchEnabled(bool enabled) = 0;
        
        //Accelermoter specific methods, abstract, must be implemented by an inheriting class
        virtual bool IsAccelerometerEnabled() = 0;
        virtual void SetAccelerometerEnabled(bool enabled) = 0;
        virtual void SetAccelerometerUpdateInterval(double interval) = 0;
        
        //Gyroscope specific methods, abstract, must be implemented by an inheriting class
        virtual bool IsGyroscopeEnabled() = 0;
        virtual void SetGyroscopeEnabled(bool enabled) = 0;
        virtual void SetGyroscopeUpdateInterval(double interval) = 0;
        
        //Returns a path to the directory where files can be written to.
        //Abstract, must be implemented by inheriting classes
        virtual string GetWorkingDirectory() = 0;
        
        //Returns a path to the directory that the application is in.
        //Abstract, must be implemented by inheriting classes
        virtual string GetApplicationDirectory() = 0;
        
        //Returns a path for a resource in a specific directory, abstract, must be implemented by an inheriting class
        virtual string GetPathForResourceInDirectory(const char* fileName, const char* fileType, const char* directory) = 0;
        
        //Returns wether the file exists at the path, abstract, must be implemented by an inheriting class
        virtual bool DoesFileExistAtPath(const string& path) = 0;
        
        //Presents a platform specific native dialog box, abstract, must be implemented by an inheriting class
        virtual int PresentNativeDialogBox(const char* title, const char* message, NativeDialogType type) = 0;
 
        //Returns platform specific details and the operating system name and version, and hardware model (if available).
        //Abstract, must be implemented by inheriting classes
        virtual void PlatformName(string& name) = 0;
        virtual void PlatformModel(string& model) = 0;
        virtual void PlatformVersion(string& version) = 0;
        
        //Returns wether memory tracking is enabled, which means is the library MemTrack tracking
        //dynamic memory allocations, can be overridden
        virtual bool IsMemoryTrackingEnabled();
        
        //Returns how much dynamic memory MemTrack has tracked allocations for. Is the above method
        //returns false, then this method will return zero. This method returns memory in bytes.
        virtual unsigned int MemoryAllocated();
        
        //Formats the memory returned in the above method and formats it in a string and formats to the nearest KB, MB, GB
        virtual void MemoryAllocatedFormatted(string& formatted);
        
        //Returns how much memory (RAM) is installed on the machine, abstract, must be implemented by an inheriting class
        virtual unsigned long long MemoryInstalled() = 0;
        
        //Hard drive specific methods, abstract, must be implemented by an inheriting class
        virtual unsigned long long DiskSpaceUsed() = 0;
        virtual unsigned long long DiskSpaceFree() = 0;
        virtual unsigned long long DiskSpaceTotal() = 0;
        
        //Returns the number of available CPUs on the machine, abstract, must be implemented by an inheriting class
        virtual unsigned int CpuCount() = 0;
        
    protected:
        //Initializes the log files, if there are existing log files, they are wiped.
        void InitLogFiles();
        
        //Logs platform specific details about OS, CPU, RAM, HDD, GPU
        virtual void LogPlatformDetails();
        
        //This method applies the changes set in the SetVideoModeInfo() method at the start of the Update() method.
        //Abstract, must be implemented by an inheriting class
        virtual void ApplyVideoModeChanges(VideoModeInfo* videoModeInfo) = 0;
        
        //This handles the viewport and back buffer resizing when the platform's window is resized.
        //Abstract, must be implemented by an inheriting class
        virtual void HandleViewportResize(unsigned int width, unsigned int height) = 0;
        
        //Some platforms require a specific render buffer storage, this method, can be overridden by
        //those platforms (iOS), by default it does nothing.
        virtual void SetRenderBufferStorage();
        
        //Sleeps the thread to help achieve the frames per second target
        virtual void DoSleep();
        
        //Returns the number of ticks since the start of the application, abstract, must be implemented by an inheriting class
        virtual unsigned int GetTicks() = 0;
        
        //Tracks the frames per seconds and Updates the application's services
        void Tick();
    
        //The RenderTarget needs to access the
        //protected method SetRenderBufferStorage()
        friend class RenderTarget;

        //The GameWindow needs to access the protected
        //method GetTicks()
        friend class GameWindow;
        
        //Member variables
        unsigned int m_Width;
        unsigned int m_Height;
        bool m_IsSuspended;
        bool m_IsRunning;
        int m_Frames;
        unsigned int m_RefreshInterval;
        unsigned int m_FramesPerSecond;
        unsigned int m_DeltaTicks;
        unsigned int m_FrameRate;
        unsigned int m_FrameTicks;
		unsigned int m_LastFrameTicks;
        unsigned int m_LastSleepFrameTicks;
		unsigned int m_LastFPSTicks;
    };
    
    //The video mode struct keeps track of the width and height, and if they need to be changed
    struct VideoModeInfo
    {
		unsigned int width;
		unsigned int height;
		bool needsChange;
	};
}

#endif /* defined(__GameDev2D__PlatformLayer__) */
