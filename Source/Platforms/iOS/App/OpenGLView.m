//
//  OpenGLView.cpp
//  GameDevFramework
//
//  Created by Brad Flood on 11-10-16.
//  Copyright 2011 Algonquin College. All rights reserved.
//

#import "OpenGLView.h"
#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/EAGLDrawable.h>


@implementation OpenGLView

+ (Class)layerClass
{
    return [CAEAGLLayer class];
}

- (id)initWithCoder:(NSCoder*)coder
{
    if ((self = [super initWithCoder:coder]))
	{
        CAEAGLLayer* openGLLayer = (CAEAGLLayer *)[self layer];
        
        //Get the device's content scale factor.
        CGFloat contentScale = 1.0f;
        
        #if IOS_SCALE_TO_RETINA_SIZE
        if([UIScreen instancesRespondToSelector:@selector(scale)])
        {
            contentScale = [[UIScreen mainScreen] scale];
        }
        #endif
        
        //Set the layer's content scale.
        [openGLLayer setContentsScale:contentScale];
        
        //Set the layer's opacoty and drawable properties.
        [openGLLayer setOpaque:YES];
        [openGLLayer setDrawableProperties:[NSDictionary dictionaryWithObjectsAndKeys:[NSNumber numberWithBool:YES], kEAGLDrawablePropertyRetainedBacking, kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil]];
        
        //Set the view's scale factor.
		[self setContentScaleFactor:contentScale];
        
        //Create the OpenGL context, initializing it with desired rendering API
        m_RenderingAPI = IOS_TARGET_OPENGL_API;
        
        //Create the opengl context with the target rendering api
        m_OpenGLContext = [[EAGLContext alloc] initWithAPI:m_RenderingAPI];
        
        //If the context is nil, that means the rendering api is NOT supported, if the target was ES 3.0 then lets try 2.0
        if(m_OpenGLContext == nil && m_RenderingAPI == kEAGLRenderingAPIOpenGLES3)
        {
            //Set ES 2.0 rendering api
            m_RenderingAPI = kEAGLRenderingAPIOpenGLES2;
            
            //Try to create the opengl context with ES 2.0 rendering api
            m_OpenGLContext = [[EAGLContext alloc] initWithAPI:m_RenderingAPI];
        }
        
        //Failed to create an OpenGL context, throw an assert
        assert(m_OpenGLContext != nil);
        
        //Safety check that OpenGL context and set the current context
        if(m_OpenGLContext != nil)
        {
            [EAGLContext setCurrentContext:m_OpenGLContext];
        }
    }
    return self;
}

- (void)dealloc
{
    [m_OpenGLContext release];
    [super dealloc];
}

- (EAGLRenderingAPI)renderingAPI
{
    return m_RenderingAPI;
}

- (BOOL)setCurrentContext
{
    return [EAGLContext setCurrentContext:m_OpenGLContext];
}

- (void)setRenderBufferStorage
{
    [m_OpenGLContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer*)[self layer]];
}

- (void)presentRenderBuffer
{
    //Display the the renderbuffer.
    if(m_OpenGLContext != nil)
    {
        [m_OpenGLContext presentRenderbuffer:GL_RENDERBUFFER];
    }
}

@end