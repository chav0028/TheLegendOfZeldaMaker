//
//  BaseObject.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2013-07-10.
//  Copyright (c) 2013 Algonquin College. All rights reserved.
//

#include "BaseObject.h"
#include "../Platforms/PlatformLayer.h"
#include "../Services/ServiceLocator.h"


#if TARGET_OS_IPHONE || TARGET_OS_MAC
#include <mach/mach_time.h>
#endif

namespace GameDev2D
{
    BaseObject::BaseObject(const string& aType) :
        m_Type(aType)
    {

    }
    
    BaseObject::~BaseObject()
    {

    }
    
    string BaseObject::GetType()
    {
        return m_Type;
    }
    
    string BaseObject::GetDescription()
    {
        stringstream description;
        description << m_Type << " at:" << this;
        return description.str();
    }
    
    void BaseObject::Log(const char* aMessage, ...)
    {
        va_list arguments;
        va_start(arguments, aMessage);
        OutputLog(VerbosityLevel_Debug, GetType().c_str(), aMessage, arguments);
        va_end(arguments);
    }
    
    void BaseObject::Log(unsigned int aVerbosity, const char* aMessage, ...)
    {
        va_list arguments;
        va_start(arguments, aMessage);
        OutputLog(aVerbosity, GetType().c_str(), aMessage, arguments);
        va_end(arguments);
    }
    
    void BaseObject::OutputLog(unsigned int aVerbosity, const char* aLabel, const char* aOutput, va_list aArgumentsList)
    {
#if DEBUG || _DEBUG
        if((aVerbosity & (LOG_VERBOSITY_MASK)) != 0)
        {
            #if _WIN32
            char labelBuffer[128];
            sprintf_s(labelBuffer, 128, "[%p][%s] - ", this, aLabel);
            OutputDebugStringA(labelBuffer);

            char outputBuffer[1024];
            vsnprintf(outputBuffer, 1024, aOutput, aArgumentsList);
            OutputDebugStringA(outputBuffer);
            OutputDebugStringA("\n");
            #else
            printf("[%p][%s] - ", this, aLabel);
            vprintf(aOutput, aArgumentsList);
            printf("\n");
            #endif
            
            #if LOG_TO_FILE
            //Get the path for the log file
            string path = string(ServiceLocator::GetPlatformLayer()->GetWorkingDirectory());
            path += LOG_FILE;
            
            //Open the file at the path, if the assert is hit it means the log file couldn't be opened
            FILE* logFile;
            logFile = fopen(path.c_str(), "a+");
            assert(logFile != nullptr);

            //Write the log to the file
            if(logFile != NULL)
            {
                fprintf(logFile, "[%p][%s] - ", this, aLabel);
                vfprintf(logFile, aOutput, aArgumentsList);
                fprintf(logFile, "\n");
                fclose(logFile);
            }
            #endif
        }
#endif
    }
    
    void BaseObject::Error(bool aThrowException, const char* aMessage, ...)
    {
#if DEBUG || _DEBUG
        va_list arguments;
        va_start(arguments, aMessage);
        printf("[ERROR] - [%p][%s] - ", this, GetType().c_str());
        vprintf(aMessage, arguments);
        printf("\n");
        
        #if LOG_ERRORS_TO_FILE
        //Get the path for the error log file
        string path = string(ServiceLocator::GetPlatformLayer()->GetWorkingDirectory());
        path += LOG_ERROR_FILE;
        
        //Open the file at the path, if the assert is hit it means the log file couldn't be opened
        FILE* errorFile;
        errorFile = fopen(path.c_str(), "a+");
        assert(errorFile != nullptr);

        //Write the error to the log file
        if(errorFile != nullptr)
        {
            fprintf(errorFile, "[ERROR] - [%p][%s] - ", this, GetType().c_str());
            vfprintf(errorFile, aMessage, arguments);
            fprintf(errorFile, "\n");
            fclose(errorFile);
        }
        #endif
        
        //Should we throw an exception?
        #if THROW_EXCEPTION_ON_ERROR
        if(aThrowException == true)
        {
            char outputBuffer[1024];
            vsnprintf(outputBuffer, 1024, aMessage, arguments);
        
            throw std::runtime_error(outputBuffer);
        }
        
        //End the variable argument list
        va_end(arguments);
        #endif
#endif
    }
    
    #if DEBUG || _DEBUG
        void BaseObject::BeginProfile(const char* aProfile)
        {
            //Safety check the profile string, if there is one log it
            if(aProfile != nullptr)
            {
                m_Profile = string(aProfile);
                Log(VerbosityLevel_Profiling, "%s profile began", aProfile);
            }
            else
            {
                Log(VerbosityLevel_Profiling, "Profile began");
            }

            #if __APPLE__
            //Cache the profiling start time
            m_ProfileStart = mach_absolute_time();
            #elif _WIN32
            //Get the performance counter frequency
            LARGE_INTEGER largeInt;
	        QueryPerformanceFrequency(&largeInt);
	        double performanceCounterFrequency = double(largeInt.QuadPart) / 1000.0;

            //Cache the profiling start time
	        QueryPerformanceCounter(&largeInt);
            m_ProfileStart = (unsigned long long)(largeInt.QuadPart / performanceCounterFrequency);
            #endif
        }
    
        double BaseObject::EndProfile()
        {
            double duration = 0.0;

            #if __APPLE__
            //Get the end time of the profile
            unsigned long long profileEnd = mach_absolute_time();
            
            //Convert it
            double conversion = 0.0;
            mach_timebase_info_data_t info;
            mach_timebase_info( &info );
            conversion = 1e-9 * (double) info.numer / (double) info.denom;	
            
            //determine the entire duration
            duration = (((double)(profileEnd - m_ProfileStart)) * conversion);
            #elif _WIN32
            //Get the performance counter frequency
            LARGE_INTEGER largeInt;
	        QueryPerformanceFrequency(&largeInt);
	        double performanceCounterFrequency = double(largeInt.QuadPart) / 1000.0;

            //Cache the profiling start time
	        QueryPerformanceCounter(&largeInt);
            unsigned long long profileEnd = (unsigned long long)(largeInt.QuadPart / performanceCounterFrequency);

            //determine the entire duration
            duration = (((double)(profileEnd - m_ProfileStart)) / 1000.0);
            #endif

            //Log the profile results
            if(m_Profile != "")
            {
                Log(VerbosityLevel_Profiling, "%s profile ended - duration: %f", m_Profile.c_str(), duration);
            }
            else
            {
                Log(VerbosityLevel_Profiling, "Profile ended - duration: %f", duration);
            }
            
            return duration;
        }
    #endif
    
}
