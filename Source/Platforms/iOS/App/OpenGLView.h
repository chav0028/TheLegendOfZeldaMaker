//
//  OpenGLView.h
//  GameDevFramework
//
//  Created by Brad Flood on 11-10-16.
//  Copyright 2011 Algonquin College. All rights reserved.
//

#import <OpenGLES/EAGL.h>
#include "OpenGL.h"


@class EAGLContext;

@interface OpenGLView : UIView
{
    EAGLContext* m_OpenGLContext;
    EAGLRenderingAPI m_RenderingAPI;
}

- (EAGLRenderingAPI)renderingAPI;
- (BOOL)setCurrentContext;
- (void)setRenderBufferStorage;
- (void)presentRenderBuffer;

@end
