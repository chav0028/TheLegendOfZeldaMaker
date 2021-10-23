//
//  BaseObject.h
//  GameDev2D
//
//  Created by Bradley Flood on 2013-07-10.
//  Copyright (c) 2013 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__GDObject__
#define __GameDev2D__GDObject__

#include "FrameworkConfig.h"
#include "../Platforms/PlatformMacros.h"


using namespace std;

namespace GameDev2D
{
    //Verbosity log levels
    enum LogVerbosity
    {
        VerbosityLevel_None       = 0,
        VerbosityLevel_Debug      = 1,
        VerbosityLevel_Input      = 2,
        VerbosityLevel_Graphics   = 4,
        VerbosityLevel_Shaders    = 8,
        VerbosityLevel_Audio      = 16,
        VerbosityLevel_Animation  = 32,
        VerbosityLevel_UI         = 64,
        VerbosityLevel_Events     = 128,
        VerbosityLevel_Profiling  = 256,
        VerbosityLevel_All        = VerbosityLevel_Debug | VerbosityLevel_Input | VerbosityLevel_Graphics | VerbosityLevel_Shaders | VerbosityLevel_Audio | VerbosityLevel_Animation | VerbosityLevel_UI | VerbosityLevel_Events | VerbosityLevel_Profiling
    };

    //This is the BaseObject for all classes in this framework, it provides a consistent
    //base for all objects in GameDev2D. Currently it can be used for determining the
    //type of an object. Logging (with different verbosity levels) to the output window
    //and to a file. Error logging (also to the output window and to a file) and profiling.
    class BaseObject
    {
    public:
        //The type of the object is based in during contruction, it can NOT be changed.
        //The type is used to identify what type of object this is
        BaseObject(const string& type);
        virtual ~BaseObject();
        
        //Returns the type of object
        string GetType();
        
        //Returns a description of the object, by default its the type of object
        //and its address, but inheriting classes can override this method.
        virtual string GetDescription();
        
        //Definition of a method that can be delayed by either a GameObject or a Scene
        typedef void (BaseObject::*DelayedMethod)();
        
    protected:
        //Used to Log a message with a variable amount of arguments, the
        //verbosity level for these logs is debug (VerbosityLevel_Debug).
        //If the LOG_TO_FILE to file preproc is enabled then this method
        //will log to a file in addition to the output window.
        void Log(const char* message, ...);
        
        //Same as the Log() method above, except you can specify the
        //verbosity of the log messages.
        void Log(unsigned int verbosity, const char* message, ...);
        
        //Used to specically log errors, it has no verbosity level, and can
        //throw an exception. If the LOG_ERRORS_TO_FILE preproc is enabled
        //then this method will log to a file in addition to the output window
        void Error(bool throwException, const char* message, ...);
        
        //Methods used to calculate the length of time a particular operation takes.
        //Place the BeginProfile() method at the start of the operation and the
        //EndProfile() method at the end of the operation, it will determine in
        //seconds how long the operation took.
        #if DEBUG || _DEBUG
        void BeginProfile(const char* profile = nullptr);
        double EndProfile();
        #endif
        
    private:
        //Conveniance method used to log messages
        void OutputLog(unsigned int verbosity, const char* label, const char* aOutput, va_list aArgumentsList);
        
        //Member variable used to hold the type of the object
        string m_Type;
        
        //Profiling member variables
        #if DEBUG || _DEBUG
        string m_Profile;
        unsigned long long m_ProfileStart;
        #endif
    };
}

#endif /* defined(__GameDev2D__GDObject__) */
