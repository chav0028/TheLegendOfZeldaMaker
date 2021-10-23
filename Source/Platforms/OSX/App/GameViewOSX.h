//
//  GameViewOSX.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-05-06.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <QuartzCore/CVDisplayLink.h>
#import <IOKit/hid/IOHIDLib.h>
#import "OpenGL.h"
#import "KeyCodes.h"


@interface GameViewOSX : NSOpenGLView <NSWindowDelegate>
{
    //Timer for the Game Loop
    NSTimer* m_Timer;
    
    //Used to track when the mouse leaves the window
    NSTrackingArea* m_TrackingArea;
    
    //Controller manager
    IOHIDManagerRef m_HumanInterfaceDeviceManager;
    
    //Keycode map
    GameDev2D::KeyCode m_Keymap[KEY_CODE_COUNT];
    
    //Fullscreen members
    NSSize m_FullscreenSize;
    BOOL m_IsFullscreen;
    
    //Vertical sync flag
    BOOL m_VerticalSync;
}

//Fullscreen methods
- (void)setIsFullScreen:(BOOL)isFullscreen;
- (void)setIsFullScreen:(BOOL)isFullscreen backingSize:(NSSize)backingSize;
- (BOOL)isFullScreen;

//Vertical sync methods
- (void)enableVerticalSync:(BOOL)isEnabled;
- (BOOL)isVerticalSyncEnabled;

//Game Loop methods
- (void)resume;
- (void)suspend;
- (void)invalidate;

//OpenGL methods
- (void)lockDrawContext;
- (void)unlockDrawContext;
- (void)flushDrawBuffer;

@end
