//
//  Platform_OSX.cpp
//  GameDevEngine
//
//  Created by Bradley Flood on 2014-03-11.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#include "Platform_OSX.h"
#include "ServiceLocator.h"
#include "Game.h"
#include "ResizeEvent.h"
#include "OrientationChangedEvent.h"
#include "FullscreenEvent.h"

#include "OpenGL.h"
#include <sys/sysctl.h>
#include <sys/utsname.h>
#include <sys/types.h>
#include <mach/mach.h>
#include <mach/mach_host.h>
#include <mach/mach_time.h>
#import "GameViewOSX.h"


namespace GameDev2D
{
    Platform_OSX::Platform_OSX(void* aGameView, const char* aWindowTitle, int aFrameRate) : PlatformLayer("Platform_OSX", aFrameRate),
        m_GameView(aGameView),
        m_IsFullScreen(false),
        m_IsMouseCursorVisible(false)
    {
        //Get the time the app started up
        m_InitTime = mach_absolute_time();
        
        //Set the Window's title
        SetWindowTitle(aWindowTitle);
        
        //Ensure, that by default, the mouse cursor is visible
        ShowMouseCursor();
    }

    Platform_OSX::~Platform_OSX()
    {
        //Invalidate the game view and release it
        [(GameViewOSX*)m_GameView invalidate];
        [(GameViewOSX*)m_GameView release];
    }
    
    PlatformType Platform_OSX::GetPlatformType()
    {
        return PlatformType_OSX;
    }
    
    void Platform_OSX::Init(unsigned int aWidth, unsigned int aHeight, bool aFullscreen, bool aVerticalSync)
    {
        //Initialize the base class
        PlatformLayer::Init();
        
        //Resize the window to the desired width and height, and
        //set the fullscreen and vertical sync settings
        ResizeTo(aWidth, aHeight);
        SetIsFullscreen(aFullscreen);
        EnabledVerticalSync(aVerticalSync);
    }
    
    bool Platform_OSX::Update()
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

        //Calculate the tick (milleseconds since last update) and update the game
        Tick();
        
        //Return wether the game is still running or not
        return m_IsRunning;
    }

    void Platform_OSX::Draw()
    {
        //Lock the OpenGL draw context
        [(GameViewOSX*)m_GameView lockDrawContext];

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
            [(GameViewOSX*)m_GameView flushDrawBuffer];
        }
        
        //Unlock the OpenGL draw context
        [(GameViewOSX*)m_GameView unlockDrawContext];
    }
    
    void Platform_OSX::Resume()
    {
        PlatformLayer::Resume();
        [(GameViewOSX*)m_GameView resume];
    }
    
    void Platform_OSX::Suspend()
    {
        PlatformLayer::Suspend();
        [(GameViewOSX*)m_GameView suspend];
    }
    
    float Platform_OSX::GetScale()
    {
        #if OSX_SCALE_TO_RETINA_SIZE
        return [[(GameViewOSX*)m_GameView window] backingScaleFactor];
        #else
        return PlatformLayer::GetScale();
        #endif
    }

    void Platform_OSX::SetWindowTitle(const char* aTitle)
    {
        NSString* title = [[NSString alloc] initWithUTF8String:aTitle];
        [[(GameViewOSX*)m_GameView window] setTitle:title];
        [title release];
    }

    void Platform_OSX::ResizeTo(unsigned int aWidth, unsigned int aHeight)
    {
        if([(GameViewOSX*)m_GameView isFullScreen] == false)
        {
            //If the app is NOT fullscreen, center the window on screen
            #if OSX_SCALE_TO_RETINA_SIZE
            [[(GameViewOSX*)m_GameView window] setContentSize:NSMakeSize(aWidth / GetScale(), aHeight / GetScale())];
            [[(GameViewOSX*)m_GameView window] center];
            #else
            [[(GameViewOSX*)m_GameView window] setContentSize:NSMakeSize(aWidth, aHeight)];
            [[(GameViewOSX*)m_GameView window] center];
            #endif
        }
        else
        {
            //We need to disable fullscreen mode if we want to resize the window while in fullscreen mode
            [(GameViewOSX*)m_GameView setIsFullScreen:false];
            [(GameViewOSX*)m_GameView setIsFullScreen:true backingSize:NSMakeSize(aWidth, aHeight)];
        }
    }
    
    void Platform_OSX::HandleViewportResize(unsigned int aWidth, unsigned int aHeight)
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
    
    void Platform_OSX::SetVideoModeInfo(VideoModeInfo* aVideoModeInfo)
    {
        VideoModeInfo_OSX* videoModeInfoOSX = (VideoModeInfo_OSX*)aVideoModeInfo;
    	m_VideoModeChangeInfo.needsChange = true;
        m_VideoModeChangeInfo.width = videoModeInfoOSX->width;
        m_VideoModeChangeInfo.height = videoModeInfoOSX->height;
        m_VideoModeChangeInfo.fullScreen = videoModeInfoOSX->fullScreen;
    }

    void Platform_OSX::ApplyVideoModeChanges(VideoModeInfo* aVideoModeInfo)
    {
        VideoModeInfo_OSX* videoModeInfoOSX = (VideoModeInfo_OSX*)aVideoModeInfo;
    
        //Handle the resizing of the window
        HandleViewportResize(videoModeInfoOSX->width, videoModeInfoOSX->height);
    
        //Set the fullscreen setting
        SetIsFullscreen(videoModeInfoOSX->fullScreen);
    }
    
    void Platform_OSX::SetIsFullscreen(bool aIsFullscreen)
    {
        if(aIsFullscreen != IsFullscreen())
        {
            [(GameViewOSX*)m_GameView setIsFullScreen:aIsFullscreen];
            DispatchEvent(new FullscreenEvent(aIsFullscreen));
        }
    }

    bool Platform_OSX::IsFullscreen()
    {
        return m_GameView != nullptr ? [(GameViewOSX*)m_GameView isFullScreen] : false;
    }
    
    void Platform_OSX::EnabledVerticalSync(bool aIsEnabled)
    {
        [(GameViewOSX*)m_GameView enableVerticalSync:aIsEnabled];
    }
    
    bool Platform_OSX::IsVerticalSyncEnabled()
    {
        return [(GameViewOSX*)m_GameView isVerticalSyncEnabled];
    }
    
    bool Platform_OSX::HasMouseInput()
    {
        return true;
    }
    
    bool Platform_OSX::HasKeyboardInput()
    {
        return true;
    }
    
    bool Platform_OSX::HasTouchInput()
    {
        return false;
    }
    
    bool Platform_OSX::HasControllerInput()
    {
        return ServiceLocator::GetInputManager()->GetNumberOfControllers() > 0;
    }
    
    bool Platform_OSX::HasAccelerometerInput()
    {
        return false;
    }
    
    bool Platform_OSX::HasGyroscopeInput()
    {
        return false;
    }
    
    bool Platform_OSX::IsMouseCursorVisible()
    {
        return m_IsMouseCursorVisible;
    }
    
    void Platform_OSX::ShowMouseCursor()
    {
        if(m_IsMouseCursorVisible == false)
        {
            CGDisplayShowCursor(kCGDirectMainDisplay);
            m_IsMouseCursorVisible = true;
        }
    }
    
    void Platform_OSX::HideMouseCursor()
    {
        if(m_IsMouseCursorVisible == true)
        {
            CGDisplayHideCursor(kCGDirectMainDisplay);
            m_IsMouseCursorVisible = false;
        }
    }
    
    void Platform_OSX::SetMouseCursorPosition(int aX, int aY)
    {
        //Get the screen size and the window's position
        NSSize screenSize = [NSScreen mainScreen].frame.size;
        NSPoint windowPosition = [(GameViewOSX*)m_GameView window].frame.origin;
        
        //Scale the size and position if retina scaling is enabled
        #if OSX_SCALE_TO_RETINA_SIZE
        screenSize = [(GameViewOSX*)m_GameView convertSizeToBacking:screenSize];
        windowPosition = [(GameViewOSX*)m_GameView convertPointToBacking:windowPosition];
        aX *= GetScale();
        aY *= GetScale();
        #endif
        
        //Reposition the mouse position based on the window's position
        NSPoint mousePosition = NSMakePoint(windowPosition.x + aX, (-1) * (windowPosition.y - screenSize.height) - m_Height + aY);
		
        //Convert the mouse position back if retina scaling is enabled
        #if OSX_SCALE_TO_RETINA_SIZE
        mousePosition = [(GameViewOSX*)m_GameView convertPointFromBacking:mousePosition];
        #endif

        //Lastly move the cursor to the desired position on screen
        CGDisplayMoveCursorToPoint(kCGDirectMainDisplay, CGPointMake(mousePosition.x, mousePosition.y));
    }
    
    bool Platform_OSX::IsMultipleTouchEnabled()
    {
        return false;
    }
    
    void Platform_OSX::SetMultipleTouchEnabled(bool aEnabled)
    {
        //Unused on OSX
    }
    
    bool Platform_OSX::IsAccelerometerEnabled()
    {
        return false;
    }
    
    void Platform_OSX::SetAccelerometerEnabled(bool aEnabled)
    {
        //Unused on OSX
    }
    
    void Platform_OSX::SetAccelerometerUpdateInterval(double aInterval)
    {
        //Unused on OSX
    }
    
    bool Platform_OSX::IsGyroscopeEnabled()
    {
        return false;
    }
    
    void Platform_OSX::SetGyroscopeEnabled(bool aEnabled)
    {
        //Unused on OSX
    }
    
    void Platform_OSX::SetGyroscopeUpdateInterval(double aInterval)
    {
        //Unused on OSX
    }
    
    string Platform_OSX::GetWorkingDirectory()
    {
        NSArray *searchPaths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
        NSString *documentsPath = [searchPaths objectAtIndex:0];
        return string([documentsPath UTF8String]);
    }
    
    string Platform_OSX::GetApplicationDirectory()
    {
        NSString* path = [[NSBundle mainBundle] resourcePath];
        return string([path UTF8String]);
    }
    
    string Platform_OSX::GetPathForResourceInDirectory(const char* aFileName, const char* aFileType, const char* aDirectory)
    {
        string path = string(GetApplicationDirectory());
        path += "/Assets/";
        path += string(aDirectory);
        path += "/";
        path += string(aFileName);
        path += ".";
        path += string(aFileType);
        return path;
    }
    
    bool Platform_OSX::DoesFileExistAtPath(const string& aPath)
    {
        bool fileExists = false;
    
        //Safety check that the path actually contains something
        if(aPath.length() > 0)
        {
            NSString* path = [[NSString alloc] initWithCString:aPath.c_str() encoding:NSUTF8StringEncoding];
            fileExists = [[NSFileManager defaultManager] fileExistsAtPath:path];
            [path release];
        }
    
        return fileExists;
    }
    
    int Platform_OSX::PresentNativeDialogBox(const char* aTitle, const char* aMessage, NativeDialogType aType)
    {
        //Create the NSString objects for the title and message
        NSString* title = [[NSString alloc] initWithUTF8String:aTitle];
        NSString* message = [[NSString alloc] initWithUTF8String:aMessage];
        
        //Create the alert box and set the title and message
        NSAlert *alert = [[NSAlert alloc] init];
        [alert setAlertStyle:NSInformationalAlertStyle];
        [alert setMessageText:title];
        [alert setInformativeText:message];
        
        //Set the buttons based on the type of alert
        switch (aType)
        {
          case NativeDialogOk:
            [alert addButtonWithTitle:@"Ok"];
            break;
            
            case NativeDialogOkCancel:
            [alert addButtonWithTitle:@"Ok"];
            [alert addButtonWithTitle:@"Cancel"];
            break;
            
            case NativeDialogYesNo:
            [alert addButtonWithTitle:@"Yes"];
            [alert addButtonWithTitle:@"No"];
            break;
        }

        //Get the response and release the alert
        NSModalResponse response = [alert runModal];
        [alert release];
        
        //Also release the NSString objects
        [title release];
        [message release];
        
        //Return the response, 0 = 'Ok' OR 'Yes', 1 = 'Cancel' OR 'No'
        return (int)(response - 1000);
    }

    void Platform_OSX::PlatformName(string& aName)
    {
        NSDictionary *systemVersion = [[NSDictionary alloc] initWithContentsOfFile:@"/System/Library/CoreServices/SystemVersion.plist"];
        NSString *productVersion = [systemVersion objectForKey:@"ProductName"];
        [systemVersion release];
        aName = string([productVersion UTF8String]);
    }
    
    void Platform_OSX::PlatformModel(string& aModel)
    {
        NSString *result = nil;
        size_t length = 0;
        
        //Get the length of the hardware's string
        sysctlbyname("hw.model", NULL, &length, NULL, 0);

        //Safety check the length
        if(length > 0)
        {
            //Get the hardware model string
            char* model = (char*)malloc(length * sizeof(char));
            sysctlbyname("hw.model", model, &length, NULL, 0);
            
            //Create an objc string with the char array
            NSString *modelString = [[NSString alloc] initWithCString:model encoding:NSUTF8StringEncoding];
            
            //Strip non-alpha characters
            result = [[modelString componentsSeparatedByCharactersInSet:[[NSCharacterSet letterCharacterSet] invertedSet]] componentsJoinedByString:@""];
            
            //Release the model string and free the char array
            [modelString release];
            free(model);
        }
        
        //Set the model string
        aModel = string([result UTF8String]);
    }

    void Platform_OSX::PlatformVersion(string& aVersion)
    {
        aVersion = string([[[NSProcessInfo processInfo] operatingSystemVersionString] UTF8String]);
    }
    
    unsigned long long Platform_OSX::MemoryInstalled()
    {
        unsigned long long physicalMemory = [NSProcessInfo processInfo].physicalMemory;
        return physicalMemory;
    }
    
    unsigned long long Platform_OSX::DiskSpaceUsed()
    {
        return DiskSpaceTotal() - DiskSpaceFree();
    }
    
    unsigned long long Platform_OSX::DiskSpaceFree()
    {
        long long freeSpace = [[[[NSFileManager defaultManager] attributesOfFileSystemForPath:NSHomeDirectory() error:nil] objectForKey:NSFileSystemFreeSize] longLongValue];
        return freeSpace;
    }
    
    unsigned long long Platform_OSX::DiskSpaceTotal()
    {
        long long diskSpace = [[[[NSFileManager defaultManager] attributesOfFileSystemForPath:NSHomeDirectory() error:nil] objectForKey:NSFileSystemSize] longLongValue];
        return diskSpace;
    }
    
    unsigned int Platform_OSX::CpuCount()
    {
        unsigned int cpuCount = (unsigned int)[NSProcessInfo processInfo].processorCount;
        return cpuCount;
    }
    
    unsigned int Platform_OSX::GetTicks()
    {
    	uint64_t time = mach_absolute_time();	
        double conversion = 0.0;
        
        mach_timebase_info_data_t info;
        mach_timebase_info( &info );
        conversion = 1e-9 * (double) info.numer / (double) info.denom;	
        
        return (((double)(time - m_InitTime)) * conversion) * 1000.0f;
    }
    
}