//
//  Platform_iOS.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-03-10.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#include "Platform_iOS.h"
#include "ServiceLocator.h"
#include "ResizeEvent.h"
#include "OrientationChangedEvent.h"
#include <sys/sysctl.h>
#include <sys/utsname.h>
#include <sys/types.h>
#include <mach/mach.h>
#include <mach/mach_host.h>
#include <mach/mach_time.h>
#import "GameView.h"
#import <QuartzCore/QuartzCore.h>
#import <CoreMotion/CoreMotion.h>
#import <AudioToolbox/AudioServices.h>


namespace GameDev2D
{
    Platform_iOS::Platform_iOS(void* aWindow, int aFrameRate) : PlatformLayer("Platform_iOS", aFrameRate),
        m_GameView(nullptr),
        m_SupportedOrientationMask(IOS_SUPPORTED_ORIENTATIONS)
    {
        //Get the time the app started up
        m_InitTime = mach_absolute_time();
        
        //Create the GameView and set it as the window's root view controller
        m_GameView = [[GameView alloc] initWithNibName:@"GameView" bundle:[NSBundle mainBundle]];
        [(UIWindow*)aWindow setRootViewController:(UIViewController*)m_GameView];
        [(UIWindow*)aWindow makeKeyAndVisible];
        
        //This will prevent the iOS device's screen from dimming
        [[UIApplication sharedApplication] setIdleTimerDisabled:YES];
    }

    Platform_iOS::~Platform_iOS()
    {
        //Invalidate the game view and release it
        [(GameView*)m_GameView invalidate];
        [(GameView*)m_GameView release];
    }
    
    PlatformType Platform_iOS::GetPlatformType()
    {
        return PlatformType_iOS;
    }
    
    bool Platform_iOS::Update()
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

    void Platform_iOS::Draw()
    {
        //First, ensure the OpenGL context is set properly
        if([(GameView*)m_GameView setCurrentContext] == true)
        {
            if(m_IsSuspended == false)
            {
                ServiceLocator::GetGraphics()->Clear();
            }
            
            //Draw the services
            ServiceLocator::DrawServices();

            if(m_IsSuspended == false)
            {
                //Lastly present the render buffer
                [(GameView*)m_GameView flushDrawBuffer];
            }
        }
    }
    
    void Platform_iOS::Resume()
    {
        PlatformLayer::Resume();
        [(GameView*)m_GameView resume];
    }
    
    void Platform_iOS::Suspend()
    {
        PlatformLayer::Suspend();
        [(GameView*)m_GameView suspend];
    }
    
    float Platform_iOS::GetScale()
    {
        #if IOS_SCALE_TO_RETINA_SIZE
        return [[UIScreen mainScreen] scale];
        #else
        return PlatformLayer::GetScale();
        #endif
    }

    void Platform_iOS::HandleViewportResize(unsigned int aWidth, unsigned int aHeight)
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
    
    void Platform_iOS::SetVideoModeInfo(VideoModeInfo* aVideoModeInfo)
    {
    	m_VideoModeChangeInfo.needsChange = true;
        m_VideoModeChangeInfo.width = aVideoModeInfo->width;
        m_VideoModeChangeInfo.height = aVideoModeInfo->height;
    }
    
    void Platform_iOS::ApplyVideoModeChanges(VideoModeInfo* aVideoModeInfo)
    {
        HandleViewportResize(aVideoModeInfo->width, aVideoModeInfo->height);
    }
    
    void Platform_iOS::SetSupportedOrientationMask(unsigned int aOrientationMask)
    {
        m_SupportedOrientationMask = aOrientationMask;
    }
    
    unsigned int Platform_iOS::GetSupportedOrientationMask()
    {
        return m_SupportedOrientationMask;
    }
    
    void Platform_iOS::SetRenderBufferStorage()
    {
        [(GameView*)m_GameView setRenderBufferStorage];
    }

    void Platform_iOS::OrientationChanged(Orientation aOrientation)
    {
        //Dispatch an orientation changed event
        DispatchEvent(new OrientationChangedEvent(aOrientation));
    }

    bool Platform_iOS::IsFullscreen()
    {
        return true;
    }
    
    bool Platform_iOS::IsSimulator()
    {
        #if TARGET_IPHONE_SIMULATOR
        return true;
        #else
        return false;
        #endif
    }
    
    bool Platform_iOS::HasMouseInput()
    {
        return false;
    }
    
    bool Platform_iOS::HasKeyboardInput()
    {
        return false;
    }
    
    bool Platform_iOS::HasTouchInput()
    {
        return true;
    }
    
    bool Platform_iOS::HasControllerInput()
    {
        //TODO: Handle iOS gamepad input support
        return false;
    }
    
    bool Platform_iOS::HasAccelerometerInput()
    {
        //The simulator doesn't support accelerometer input
        if(IsSimulator() == true)
        {
            return false;
        }
        
        //Handle iOS Accelerometer input support
        return [[(GameView*)m_GameView motionManager] isAccelerometerAvailable];
    }
    
    bool Platform_iOS::HasGyroscopeInput()
    {
        //The simulator doesn't support gyroscope input
        if(IsSimulator() == true)
        {
            return false;
        }
    
        //Handle iOS Gyroscope input support
        return [[(GameView*)m_GameView motionManager] isGyroAvailable];
    }
    
    bool Platform_iOS::IsMultipleTouchEnabled()
    {
        if(HasTouchInput() == true)
        {
            return [(GameView*)m_GameView isMultipleTouchEnabled];
        }
        return false;
    }
    
    void Platform_iOS::SetMultipleTouchEnabled(bool aEnabled)
    {
        if(HasTouchInput() == true)
        {
            [(GameView*)m_GameView setMultipleTouchEnabled:aEnabled];
            DispatchEvent(new Event(aEnabled == true ? MULTI_TOUCH_ENABLED_EVENT : MULTI_TOUCH_DISABLED_EVENT));
        }
    }
    
    bool Platform_iOS::IsAccelerometerEnabled()
    {
        if(HasAccelerometerInput() == true)
        {
            return [[(GameView*)m_GameView motionManager] isAccelerometerActive];
        }
        return false;
    }
    
    void Platform_iOS::SetAccelerometerEnabled(bool aEnabled)
    {
        if(HasAccelerometerInput() == true)
        {
            if(aEnabled == true)
            {
                [[(GameView*)m_GameView motionManager] startAccelerometerUpdatesToQueue:[NSOperationQueue currentQueue] withHandler:^(CMAccelerometerData* accelerometerData, NSError* error)
                {
                    if(error == nil)
                    {
                        ServiceLocator::GetInputManager()->HandleAccelerometerData(accelerometerData.acceleration.x, accelerometerData.acceleration.y, accelerometerData.acceleration.z);
                    }
                    else
                    {
                        Error(false, "%s", [error localizedFailureReason].UTF8String);
                    }
                }];
                
                //Dispatch the accelerometer event
                DispatchEvent(new Event(ACCELEROMETER_ENABLED_EVENT));
            }
            else
            {
                [[(GameView*)m_GameView motionManager] stopAccelerometerUpdates];
                DispatchEvent(new Event(ACCELEROMETER_DISABLED_EVENT));
            }
        }
    }
    
    void Platform_iOS::SetAccelerometerUpdateInterval(double aInterval)
    {
        if(HasAccelerometerInput() == true)
        {
            return [[(GameView*)m_GameView motionManager] setAccelerometerUpdateInterval:aInterval];
        }
    }
    
    bool Platform_iOS::IsGyroscopeEnabled()
    {
        if(HasGyroscopeInput() == true)
        {
            return [[(GameView*)m_GameView motionManager] isGyroActive];
        }
        return false;
    }
    
    void Platform_iOS::SetGyroscopeEnabled(bool aEnabled)
    {
        if(HasGyroscopeInput() == true)
        {
            if(aEnabled == true)
            {
                [[(GameView*)m_GameView motionManager] startGyroUpdatesToQueue:[NSOperationQueue currentQueue] withHandler:^(CMGyroData* gyroData, NSError* error)
                {
                    if(error == nil)
                    {
                        ServiceLocator::GetInputManager()->HandleGyroscopeRotation([gyroData rotationRate].x, [gyroData rotationRate].y, [gyroData rotationRate].z);
                    }
                    else
                    {
                        Error(false, "%s", [error localizedFailureReason].UTF8String);
                    }
                }];
                
                //Dispatch the gyroscope event
                DispatchEvent(new Event(GYROSCOPE_ENABLED_EVENT));
            }
            else
            {
                [[(GameView*)m_GameView motionManager] stopGyroUpdates];
                DispatchEvent(new Event(GYROSCOPE_DISABLED_EVENT));
            }
        }
    }
    
    void Platform_iOS::SetGyroscopeUpdateInterval(double aInterval)
    {
        if(HasGyroscopeInput() == true)
        {
            return [[(GameView*)m_GameView motionManager] setGyroUpdateInterval:aInterval];
        }
    }
    
    string Platform_iOS::GetWorkingDirectory()
    {
        NSArray *searchPaths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
        NSString *documentsPath = [searchPaths objectAtIndex:0];
        return string([documentsPath UTF8String]);
    }
    
    string Platform_iOS::GetApplicationDirectory()
    {
        NSString* path = [[NSBundle mainBundle] resourcePath];
        return string([path UTF8String]);
    }
    
    string Platform_iOS::GetPathForResourceInDirectory(const char* aFileName, const char* aFileType, const char* aDirectory)
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
    
    bool Platform_iOS::DoesFileExistAtPath(const string& aPath)
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
    
    void Platform_iOS::LowMemoryWarning()
    {
        //Dispatch event - low memory warning
        DispatchEvent(new Event(LOW_MEMORY_WARNING_EVENT));
    }
    
    void Platform_iOS::Vibrate()
    {
        AudioServicesPlaySystemSound(kSystemSoundID_Vibrate);
    }
    
    int Platform_iOS::PresentNativeDialogBox(const char* aTitle, const char* aMessage, NativeDialogType aType)
    {
        //Create the NSString objects for the title and message
        NSString* title = [[NSString alloc] initWithUTF8String:aTitle];
        NSString* message = [[NSString alloc] initWithUTF8String:aMessage];
        
        //Create the alert box and set the title and message
        UIAlertView* alertView = [[UIAlertView alloc] init];
        [alertView setAlertViewStyle:UIAlertViewStyleDefault];
        [alertView setTitle:title];
        [alertView setMessage:message];
        
        //Set the buttons based on the type of alert
        switch (aType)
        {
          case NativeDialogOk:
            [alertView addButtonWithTitle:@"Ok"];
            break;
            
            case NativeDialogOkCancel:
            [alertView addButtonWithTitle:@"Ok"];
            [alertView addButtonWithTitle:@"Cancel"];
            break;
            
            case NativeDialogYesNo:
            [alertView addButtonWithTitle:@"Yes"];
            [alertView addButtonWithTitle:@"No"];
            break;
        }

        //Show the alertview
        [alertView show];
        
        //Release the NSString objects
        [title release];
        [message release];
        
        //Return -1, since we don't have a response
        return -1;
    }

    void Platform_iOS::PlatformName(string& aName)
    {
        aName = string([[[UIDevice currentDevice] systemName] cStringUsingEncoding:NSUTF8StringEncoding]);
    }
    
    void Platform_iOS::PlatformModel(string& aModel)
    {
        NSString *type;
        #if TARGET_IPHONE_SIMULATOR
        type = @"Simulator";
        #else
        struct utsname systemInfo;
        uname(&systemInfo);
        NSString *result = [[NSString alloc] initWithCString:systemInfo.machine encoding:NSUTF8StringEncoding];
        if ([result isEqualToString:@"iPod3,1"])        type = @"iPod Touch 3";
        if ([result isEqualToString:@"iPod4,1"])        type = @"iPod Touch 4";
        if ([result isEqualToString:@"iPod5,1"])        type = @"iPod Touch 5";
        if ([result isEqualToString:@"iPhone2,1"])      type = @"iPhone 3Gs";
        if ([result isEqualToString:@"iPhone3,1"])      type = @"iPhone 4";
        if ([result isEqualToString:@"iPhone4,1"])      type = @"iPhone 4s";
        if ([result isEqualToString:@"iPhone5,1"]   ||
            [result isEqualToString:@"iPhone5,2"])      type = @"iPhone 5";
        if ([result isEqualToString:@"iPad2,1"]     ||
            [result isEqualToString:@"iPad2,2"]     ||
            [result isEqualToString:@"iPad2,3"])        type = @"iPad 2";
        if ([result isEqualToString:@"iPad3,1"]     ||
            [result isEqualToString:@"iPad3,2"]     ||
            [result isEqualToString:@"iPad3,3"])        type = @"iPad 3";
        if ([result isEqualToString:@"iPad3,4"]     ||
            [result isEqualToString:@"iPad3,5"]     ||
            [result isEqualToString:@"iPad3,6"])         type = @"iPad 4";
        if ([result isEqualToString:@"iPad4,1"]     ||
            [result isEqualToString:@"iPad4,2"])         type = @"iPad Air";
        if ([result isEqualToString:@"iPad2,5"]     ||
            [result isEqualToString:@"iPad2,6"]     ||
            [result isEqualToString:@"iPad2,7"])        type = @"iPad Mini";
        if ([result isEqualToString:@"iPad4,4"]     ||
            [result isEqualToString:@"iPad4,5"])        type = @"iPad Mini Retina";
        if ([result isEqualToString:@"iPhone6,1"]   ||
            [result isEqualToString:@"iPhone6,2"])      type = @"iPhone 5s";
        if ([result isEqualToString:@"iPhone5,3"]   ||
            [result isEqualToString:@"iPhone5,4"])      type = @"iPhone 5c";
        
        //Release the result string
        [result release];
        #endif
        
        aModel = string([type UTF8String]);
    }

    void Platform_iOS::PlatformVersion(string& aVersion)
    {
        aVersion = string([[[UIDevice currentDevice] systemVersion] cStringUsingEncoding:NSUTF8StringEncoding]);
    }
    
    bool Platform_iOS::IsMemoryTrackingEnabled()
    {
        #if TRACK_MEMORY_USAGE && DEBUG
        return !IsSimulator();
        #else
        return false;
        #endif
    }
    
    unsigned long long Platform_iOS::MemoryInstalled()
    {
        unsigned long long physicalMemory = [NSProcessInfo processInfo].physicalMemory;
        return physicalMemory;
    }
    
    unsigned long long Platform_iOS::DiskSpaceUsed()
    {
        return DiskSpaceTotal() - DiskSpaceFree();
    }
    
    unsigned long long Platform_iOS::DiskSpaceFree()
    {
        long long freeSpace = [[[[NSFileManager defaultManager] attributesOfFileSystemForPath:NSHomeDirectory() error:nil] objectForKey:NSFileSystemFreeSize] longLongValue];
        return freeSpace;
    }
    
    unsigned long long Platform_iOS::DiskSpaceTotal()
    {
        long long diskSpace = [[[[NSFileManager defaultManager] attributesOfFileSystemForPath:NSHomeDirectory() error:nil] objectForKey:NSFileSystemSize] longLongValue];
        return diskSpace;
    }
    
    unsigned int Platform_iOS::CpuCount()
    {
        static unsigned int numberOfCPUs = 0;
        if(numberOfCPUs == 0)
        {
            size_t length = sizeof(numberOfCPUs);
            sysctlbyname ("hw.ncpu", &numberOfCPUs, &length, NULL, 0);
        }
        return numberOfCPUs;
    }
    
    unsigned int Platform_iOS::GetTicks()
    {
    	uint64_t time = mach_absolute_time();	
        double conversion = 0.0;
        
        mach_timebase_info_data_t info;
        mach_timebase_info( &info );
        conversion = 1e-9 * (double) info.numer / (double) info.denom;	
        
        return (((double)(time - m_InitTime)) * conversion) * 1000.0f;
    }
}