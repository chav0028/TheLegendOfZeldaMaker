//
//  PlatformLayer.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-03-10.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//


#include "PlatformLayer.h"
#include "../Services/ServiceLocator.h"
#include "../../Game/Game.h"
#if __APPLE__
#include <unistd.h>
#endif


namespace GameDev2D
{
    PlatformLayer::PlatformLayer(const string& aPlatformType, int aFrameRate) : BaseObject(aPlatformType), EventDispatcher(),
        m_Width(0),
        m_Height(0),
        m_IsSuspended(false),
        m_IsRunning(true),
        m_Frames(0),
        m_RefreshInterval(0),
        m_FramesPerSecond(0),
        m_DeltaTicks(0),
        m_FrameRate(0),
        m_FrameTicks(0),
		m_LastFrameTicks(0),
        m_LastSleepFrameTicks(0),
		m_LastFPSTicks(0)
    {
        //Set the target framerate
        SetTargetFramerate(aFrameRate);
        
        //Set ServiceLocator's PlatformLayer pointer
        ServiceLocator::SetPlatformLayer(this);
    }
    
    PlatformLayer::~PlatformLayer()
    {    
        //Remove all the game services
        ServiceLocator::RemoveAllServices();
    }

    void PlatformLayer::Init()
    {
        //Initialize the log files
        InitLogFiles();
        
        //Log the platform details (CPU, RAM, GPU and hardware info)
        LogPlatformDetails();
        
        //Initialize the Game services
        ServiceLocator::LoadDefaultServices();
    }
    
    void PlatformLayer::InitLogFiles()
    {
        #if LOG_TO_FILE
        {
            //Get the Path for the Working Directory and append the Log filename to it
            string path = string(GetWorkingDirectory());
            path += LOG_FILE;
                    
            //Open the log file, wiping any existing data in the file
            FILE* logFile;
            logFile = fopen(path.c_str(), "w");

            //Make sure the file opened, if it did close the file
            if(logFile != NULL)
            {
                fprintf(logFile, "[%s] - Allocated at: %p\n", GetType().c_str(), this);
                fclose(logFile);
            }
        }
        #endif
        
        #if LOG_ERRORS_TO_FILE
        {
            //Get the Path for the Working Directory and append the Error log filename to it
            string path = string(GetWorkingDirectory());
            path += LOG_ERROR_FILE;
                    
            //Open the error log file, wiping any existing data in the file
            FILE* errorFile;
            errorFile = fopen(path.c_str(), "w");

            //Make sure the file opened, if it did close the file
            if(errorFile != NULL)
            {
                fclose(errorFile);
            }
        }
        #endif
    }
    
    void PlatformLayer::LogPlatformDetails()
    {
        //Local variables
        string name;
        string model;
        string version;

        //Get the platform name, model and operating system version
        PlatformName(name);
        PlatformModel(model);
        PlatformVersion(version);

        //Log platform information
        Log(VerbosityLevel_Debug, "Platform: %s %s", name.c_str(), version.c_str());
        if(model != "") 
        {
            Log(VerbosityLevel_Debug, "Model: %s", model.c_str());
        }
        Log(VerbosityLevel_Debug, "CPU cores: %u", CpuCount());
        Log(VerbosityLevel_Debug, "Memory installed: %.2f-MB", ((float)MemoryInstalled() / 1024.0f / 1024.0f));
        Log(VerbosityLevel_Debug, "Disk space total: %.2f-GB", ((float)DiskSpaceTotal() / 1024.0f / 1024.0f / 1024.0f));
        Log(VerbosityLevel_Debug, "Disk space used: %.2f-GB", ((float)DiskSpaceUsed() / 1024.0f / 1024.0f / 1024.0f));
        Log(VerbosityLevel_Debug, "Disk space free: %.2f-GB", ((float)DiskSpaceFree() / 1024.0f / 1024.0f / 1024.0f));
        Log(VerbosityLevel_Debug, "GPU: %s", (const char*)glGetString(GL_RENDERER));
        Log(VerbosityLevel_Debug, "OpenGL API: %s", (const char*)glGetString(GL_VERSION));
    }
    
    void PlatformLayer::SetRootScene(Scene* aRootScene)
    {
        //Safety check the scene pointer
        if(aRootScene != nullptr)
        {
            //Push the Game scene
            ServiceLocator::GetSceneManager()->SetRootScene(aRootScene);
        }
    }

    bool PlatformLayer::GameLoop()
    {
        if(m_IsSuspended == false)
        {
            //Update the frame
            bool isRunning = Update();
            
            //Draw the frame
            Draw();
            
            //Return wether the game loop still running
            return isRunning;
        }
        
        return m_IsRunning;
    }
    
    void PlatformLayer::Resume()
    {
        //Set the isSuspended flag to false
        m_IsSuspended = false;

        //Reset the last frame ticks, to avoid a large delta time after the app suspension
        m_LastFrameTicks = GetTicks();
        
        //Dispatch a Resume Event
        DispatchEvent(new Event(RESUME_EVENT));
    }
    
    void PlatformLayer::Suspend()
    {
        //Set the isSuspended flag to true
        m_IsSuspended = true;
        
        //Dispatch a Suspend Event
        DispatchEvent(new Event(SUSPEND_EVENT));
    }
    
    void PlatformLayer::Shutdown()
    {
        if(m_IsRunning == true)
        {
            //Set the isRunning flag to false
            m_IsRunning = false;
            
            //Dispatch a Shutdown Event
            DispatchEvent(new Event(SHUTDOWN_EVENT));
        }
    }
    
    int PlatformLayer::GetWidth()
    {
        return m_Width;
    }
    
    int PlatformLayer::GetHeight()
    {
        return m_Height;
    }
    
    float PlatformLayer::GetScale()
    {
        return 1.0f;
    }
    
    void PlatformLayer::ResizeTo(unsigned int aWidth, unsigned int aHeight)
    {
        //Does nothing, each platform will have to override to implement resizing
    }
    
    unsigned int PlatformLayer::GetDeltaTicks()
    {
        return m_DeltaTicks;
    }
    
    double PlatformLayer::GetDeltaTime()
    {
        return (double)(m_DeltaTicks / 1000.0);
    }
    
    double PlatformLayer::GetElapsedTime()
    {
        return ((double)GetTicks()) / 1000.0;
    }
    
    unsigned int PlatformLayer::GetFramesPerSecond()
    {
        return m_FramesPerSecond;
    }
    
    void PlatformLayer::SetTargetFramerate(unsigned int aFramerate)
    {
        m_FrameRate = aFramerate;
        m_RefreshInterval = 1000 / m_FrameRate;
    }

    bool PlatformLayer::IsFullscreen()
    {
        return false;
    }

    void PlatformLayer::SetIsFullscreen(bool aIsFullscreen)
    {
        //Does nothing, each platform will have to implement to support fullscreen, that is if the platform supports fullscreen
    }
    
    bool PlatformLayer::IsSuspended()
    {
        return m_IsSuspended;
    }
    
    bool PlatformLayer::IsMemoryTrackingEnabled()
    {
        #if TRACK_MEMORY_USAGE && DEBUG
        return true;
        #else
        return false;
        #endif
    }
    
    unsigned int PlatformLayer::MemoryAllocated()
    {
        #if TRACK_MEMORY_USAGE
        return MemTrack::TotalBytesUsed();
        #endif
        
        return 0;
    }
    
    void PlatformLayer::MemoryAllocatedFormatted(string& aFormatted)
    {
        #if TRACK_MEMORY_USAGE
        double bytes = MemoryAllocated();

#if __APPLE__
        vector<string> sizes = {"B", "KB", "MB", "GB"};
#elif _WIN32
        vector<string> sizes;
        sizes.push_back("B");
        sizes.push_back("KB");
        sizes.push_back("MB");
        sizes.push_back("GB");
#endif

        int order = 0;
        
        while(bytes >= 1024 && order + 1 < sizes.size())
        {
            order++;
            bytes = bytes / 1024;
        }
        
        stringstream memoryStream;
        memoryStream << setprecision(4) << bytes << sizes.at(order);
        
        aFormatted = string(memoryStream.str());
        #else
        aFormatted = "Unknown";
        #endif   
    }
    
    void PlatformLayer::SetRenderBufferStorage()
    {
        //By default this method does nothing, platforms that require
        //a custom render buffer storage can override this method
    }
    
    void PlatformLayer::DoSleep()
    {
        //Sleep the main thread to achieve our target framerate
        unsigned int ticks = GetTicks();
		unsigned int ticksSinceLastFrame = ticks - m_LastSleepFrameTicks;
		if(ticksSinceLastFrame <= m_RefreshInterval)
#ifdef _WINDOWS
		Sleep((m_RefreshInterval - ticksSinceLastFrame));
#else
        usleep((m_RefreshInterval - ticksSinceLastFrame) * 1000);
#endif
		m_LastSleepFrameTicks = GetTicks();
    }
    
    void PlatformLayer::Tick()
    {
        //Increment the frames and calculate the delta ticks
        m_Frames++;
		m_FrameTicks = GetTicks();
		m_DeltaTicks = m_FrameTicks - m_LastFrameTicks;
		
        //Clamp the delta ticks
		if(m_DeltaTicks > 1000)
        {
			m_DeltaTicks = 1000;
        }
        
        //Update the Game's services
        ServiceLocator::UpdateServices(GetDeltaTime());
		
        //Calculate the frames per second
		if(m_FrameTicks - m_LastFPSTicks >= 1000)
        {
			m_FramesPerSecond = m_Frames;
			m_Frames = 0;
			m_LastFPSTicks = m_FrameTicks;
		}
		m_LastFrameTicks = m_FrameTicks;
    }
}