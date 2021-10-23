//
//  OpenGL.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-03-07.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef GameDev2D_OpenGL_h
#define GameDev2D_OpenGL_h

#if __APPLE__
    #include "TargetConditionals.h"

    #if TARGET_OS_IPHONE
        #include <OpenGLES/ES3/gl.h>
        #include <OpenGLES/ES3/glext.h>
    #elif TARGET_OS_MAC
        #import <OpenGL/OpenGL.h>
        #if CGL_VERSION_1_3
            #import <OpenGL/gl3.h>
            #import <OpenGL/gl3ext.h>
        #else
            #error "This operating system doesn't support OpenGL 3.0, you must be on OSX Lion OR higher"
        #endif
    
    #endif
#elif _WIN32
    //#include <Windows.h>
    //#include <gl/GL.h>
    //#include <gl/GLU.h>
    #include "../Platforms/Windows/App/OpenGL/OpenGLExtensions.h"
#endif


static inline const char * GetGLErrorString(GLenum error)
{
	const char *str;
	switch( error )
	{
		case GL_NO_ERROR:
			str = "GL_NO_ERROR";
			break;
		case GL_INVALID_ENUM:
			str = "GL_INVALID_ENUM";
			break;
		case GL_INVALID_VALUE:
			str = "GL_INVALID_VALUE";
			break;
		case GL_INVALID_OPERATION:
			str = "GL_INVALID_OPERATION";
			break;		
#if defined __gl_h_ || defined __gl3_h_
		case GL_OUT_OF_MEMORY:
			str = "GL_OUT_OF_MEMORY";
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			str = "GL_INVALID_FRAMEBUFFER_OPERATION";
			break;
#endif
#if defined __gl_h_
		case GL_STACK_OVERFLOW:
			str = "GL_STACK_OVERFLOW";
			break;
		case GL_STACK_UNDERFLOW:
			str = "GL_STACK_UNDERFLOW";
			break;
		case GL_TABLE_TOO_LARGE:
			str = "GL_TABLE_TOO_LARGE";
			break;
#endif
		default:
			str = "(ERROR: Unknown Error Enum)";
			break;
	}
	return str;
}


#endif
