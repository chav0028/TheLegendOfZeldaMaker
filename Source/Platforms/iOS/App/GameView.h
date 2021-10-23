//
//  GameView.h
//  GameDevFramework
//
//  Created by Bradley Flood on 12-08-30.
//  Copyright (c) 2012 Algonquin College. All rights reserved.
//


@class OpenGLView;
@class CADisplayLink;
@class CMMotionManager;

@interface GameView : UIViewController
{
    OpenGLView* m_OpenGLView;
    CADisplayLink* m_DisplayLink;
    CMMotionManager* m_MotionManager;
}

@property (nonatomic, retain) IBOutlet OpenGLView *openGLView;

- (void)resume;
- (void)suspend;
- (void)invalidate;

- (BOOL)setCurrentContext;
- (void)setRenderBufferStorage;
- (void)flushDrawBuffer;

- (void)setMultipleTouchEnabled:(BOOL)enabled;
- (BOOL)isMultipleTouchEnabled;

- (CMMotionManager*)motionManager;

@end
