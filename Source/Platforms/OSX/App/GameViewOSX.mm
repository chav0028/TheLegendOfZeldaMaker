//
//  GameViewOSX.m
//  GameDev2D
//
//  Created by Bradley Flood on 2014-05-06.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#import "GameViewOSX.h"

#include "ServiceLocator.h"
#include "Platform_OSX.h"
#include "InputManager.h"
#include "KeyEvent.h"
#include "ControllerGeneric.h"
#include "ControllerPS3.h"
#include "ControllerXbox360.h"
#include "ControllerData.h"


//Callback methods for human interface devices
static void OnDeviceMatched(void* context, IOReturn result, void* sender, IOHIDDeviceRef device);
static void OnDeviceRemoved(void * context, IOReturn result, void * sender, IOHIDDeviceRef device);
static void OnDeviceValueChanged(void * context, IOReturn result, void * sender, IOHIDValueRef value);

//'Hidden' methods
@interface GameViewOSX ()

- (void)gameLoop;
- (void)initKeyMap;
- (void)initHumanInterfaceDevice;

@end

@implementation GameViewOSX

- (void)dealloc
{
    [m_Timer release];
    [m_TrackingArea release];
    [super dealloc];
}

- (void)awakeFromNib
{
    //
    [[self window] setAcceptsMouseMovedEvents:YES];
    [[self window] makeFirstResponder:self];
    [self setAutoresizesSubviews:YES];

    //Initialize the member variables
    m_FullscreenSize = NSZeroSize;
    m_IsFullscreen = false;
    m_VerticalSync = false;
    
    //
    NSOpenGLPixelFormatAttribute attributes[] =
	{
		NSOpenGLPFADoubleBuffer,
		NSOpenGLPFADepthSize, 32,
		// Must specify the 3.2 Core Profile to use OpenGL 3.2 or higher
		NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion3_2Core,
		0
	};
    
    //Create the OpenGL pixel format object, if the assert gets hit, that means the attributes passed in were not supported on this computer
    NSOpenGLPixelFormat *pixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:attributes];
    assert(pixelFormat != nil);
	   
    //Create the opengl context with the pixel format, if the assert gets hit that means we couldn't create the opengl context with the
    //given pixel format, meaning one of the attributes is not valid on this computer
    NSOpenGLContext* context = [[NSOpenGLContext alloc] initWithFormat:pixelFormat shareContext:nil];
    assert(context != nil);
    
#if DEBUG
	// When we're using a CoreProfile context, crash if we call a legacy OpenGL function
	// This will make it much more obvious where and when such a function call is made so
	// that we can remove such calls.
	// Without this we'd simply get GL_INVALID_OPERATION error for calling legacy functions
	// but it would be more difficult to see where that function was called.
	CGLEnable((CGLContextObj)[context CGLContextObj], kCGLCECrashOnRemovedFunctions);
#endif
	
    //Set the view's pixel format and the opengl context
    [self setPixelFormat:pixelFormat];
    [self setOpenGLContext:context];
    
    //Release the pixel format object, we don't need it anymore
    [pixelFormat release];
    
#if OSX_SCALE_TO_RETINA_SIZE
    [self setWantsBestResolutionOpenGLSurface:YES];
#endif

    //Initialize the key map used for keyboard input
    [self initKeyMap];
    
    //Initialize any plugged in human interface devices
    [self initHumanInterfaceDevice];
    
    //There's a bug on Mavericks where the cursor disappears
    [self performSelector:@selector(fixCursor) withObject:nil afterDelay:0.5];
}

- (void)prepareOpenGL
{
    [super prepareOpenGL];

    //Make the opengl context the current one
    [[self openGLContext] makeCurrentContext];
}

- (void)setIsFullScreen:(BOOL)aIsFullscreen
{
    //Get the frame size
    NSSize size = [self frame].size;
    
    //Scale the size if retina is supported
#if OSX_SCALE_TO_RETINA_SIZE
    size = [self convertSizeToBacking:size];
#endif

    //Set the fullscreen at the given backing size
    [self setIsFullScreen:aIsFullscreen backingSize:size];
}

- (void)setIsFullScreen:(BOOL)aIsFullscreen backingSize:(NSSize)aBackingSize
{
    //Safety check that we aren't already in fullscreen mode
    if(aIsFullscreen != m_IsFullscreen)
    {
        //Set the fullscreen flag
        m_IsFullscreen = aIsFullscreen;

        //Get the context object
        CGLContextObj contextObject = (CGLContextObj)[[self openGLContext] CGLContextObj];

        //Are we are going into fullscreen mode?
        if(m_IsFullscreen == true)
        {
            //Set the fullscreen backing size
            m_FullscreenSize = aBackingSize;

            //Enable the fullscreen backing size
            GLint backingSize[2] = {(GLint)m_FullscreenSize.width, (GLint)m_FullscreenSize.height};
            CGLSetParameter(contextObject, kCGLCPSurfaceBackingSize, backingSize);
            CGLEnable(contextObject, kCGLCESurfaceBackingSize);
        
            //Enter fullscreen mode
            [self enterFullScreenMode:[[self window] screen] withOptions:nil];
            [[self window] becomeFirstResponder];
        }
        else
        {
            //Disable the fullscreen backing size
            CGLDisable(contextObject, kCGLCESurfaceBackingSize);
        
            //Exit fullscreen mode
            [self exitFullScreenModeWithOptions:nil];
        }
    }
}

- (BOOL)isFullScreen
{
    return m_IsFullscreen;
}

- (void)enableVerticalSync:(BOOL)aIsEnabled
{
    //Safety check that vertical sync isn't already enabled
    if(m_VerticalSync != aIsEnabled)
    {
        //Set the vertical sync flag
        m_VerticalSync = aIsEnabled;
        
        //Enable vertical sync
        GLint verticalSync = m_VerticalSync == true ? 1 : 0;
        [[self openGLContext] setValues:&verticalSync forParameter:NSOpenGLCPSwapInterval];
    }
}

- (BOOL)isVerticalSyncEnabled
{
    return m_VerticalSync;
}

- (void)resume
{
    //Safety check that the timer is nil
    if(m_Timer == nil)
    {
        //Create a new timer object, set it the call the gameLoop method
        m_Timer = [[NSTimer timerWithTimeInterval:(1.0f/1000.0f) target:self selector:@selector(gameLoop) userInfo:nil repeats:YES] retain];
        
        //Add the timer to the main run loop
        [[NSRunLoop currentRunLoop] addTimer:m_Timer forMode:NSDefaultRunLoopMode];
        [[NSRunLoop currentRunLoop] addTimer:m_Timer forMode:NSEventTrackingRunLoopMode];
    }
}

- (void)suspend
{
    //Safety check that the timer is valid
    if(m_Timer != nil && [m_Timer isValid] == YES)
    {
        //Invalidate the timer
        [self invalidate];
        
        //Release the timer
        [m_Timer release];
        m_Timer = nil;
    }
}

- (void)invalidate
{
    [m_Timer invalidate];
}

- (void)lockDrawContext
{
    CGLLockContext((CGLContextObj)[[self openGLContext] CGLContextObj]);
}

- (void)unlockDrawContext
{
    CGLUnlockContext((CGLContextObj)[[self openGLContext] CGLContextObj]);
}

- (void)flushDrawBuffer
{
	CGLFlushDrawable((CGLContextObj)[[self openGLContext] CGLContextObj]);
}

- (void)reshape
{
    //Lock the OpenGL draw context
    [self lockDrawContext];
    
    //Safety check that the platform layer isn't null
    if(GameDev2D::ServiceLocator::GetPlatformLayer() != nullptr)
    {
        //Get the frame size
        NSSize size = [self frame].size;
        
        //Scale the size if retina is supported
        #if OSX_SCALE_TO_RETINA_SIZE
        size = [self convertSizeToBacking:size];
        #endif

        //Set the platform's video mode info struct
        GameDev2D::VideoModeInfo_OSX videoModeInfoOSX;
        videoModeInfoOSX.width = m_IsFullscreen == true ? m_FullscreenSize.width : size.width;
        videoModeInfoOSX.height = m_IsFullscreen == true ? m_FullscreenSize.height : size.height;
        videoModeInfoOSX.fullScreen = m_IsFullscreen;
    
        //Set the platform layer's updated video mode
        GameDev2D::ServiceLocator::GetPlatformLayer()->SetVideoModeInfo(&videoModeInfoOSX);
    }
    
	//Unlock the OpenGL draw context
    [self unlockDrawContext];
    
    [super reshape];
}

-(void)updateTrackingAreas
{
    //If the tracking area object isn't nil, remove it from the view and release it
    if(m_TrackingArea != nil)
    {
        [self removeTrackingArea:m_TrackingArea];
        [m_TrackingArea release];
    }

    //Allocate a new tracking area object
    int options = (NSTrackingMouseEnteredAndExited | NSTrackingActiveAlways);
    m_TrackingArea = [[NSTrackingArea alloc] initWithRect:[self bounds] options:options owner:self userInfo:nil];
    
    //Add the tracking area to the view
    [self addTrackingArea:m_TrackingArea];
    [super updateTrackingAreas];
}

- (void)gameLoop
{
    //Get the PlatformLayer pointer from the ServiceLocator
    GameDev2D::Platform_OSX* platformLayer = (GameDev2D::Platform_OSX*)GameDev2D::ServiceLocator::GetPlatformLayer();

    //Make the opengl context the current context
    [[self openGLContext] makeCurrentContext];

    //Safety check the platformLayer pointer
    if(platformLayer != nullptr)
    {
        //If the Game Loop returns false, remove and invalidate the display link
        if(platformLayer->GameLoop() == false)
        {
            //If the game loop returns false that means we should exit the application,
            //the the application to stop
            [[NSApplication sharedApplication] terminate:self];
        }
    }
}

- (void)initKeyMap
{
    using namespace GameDev2D;
    {
		for(int i = 0; i < KEY_CODE_COUNT; i++)
		{
			m_Keymap[i] = KEY_CODE_UNKNOWN;
		}
	
        m_Keymap[0x00] = KEY_CODE_A;
        m_Keymap[0x01] = KEY_CODE_S;
        m_Keymap[0x02] = KEY_CODE_D;
        m_Keymap[0x03] = KEY_CODE_F;
        m_Keymap[0x05] = KEY_CODE_G;
        m_Keymap[0x04] = KEY_CODE_H;
        m_Keymap[0x06] = KEY_CODE_Z;
        m_Keymap[0x07] = KEY_CODE_X;
        m_Keymap[0x08] = KEY_CODE_C;
        m_Keymap[0x09] = KEY_CODE_V;
        m_Keymap[0x0B] = KEY_CODE_B;
        m_Keymap[0x0C] = KEY_CODE_Q;
        m_Keymap[0x0D] = KEY_CODE_W;
        m_Keymap[0x0E] = KEY_CODE_E;
        m_Keymap[0x0F] = KEY_CODE_R;
        m_Keymap[0x10] = KEY_CODE_Y;
        m_Keymap[0x11] = KEY_CODE_T;
        m_Keymap[0x12] = KEY_CODE_1;
        m_Keymap[0x13] = KEY_CODE_2;
        m_Keymap[0x14] = KEY_CODE_3;
        m_Keymap[0x15] = KEY_CODE_4;
        m_Keymap[0x16] = KEY_CODE_6;
        m_Keymap[0x17] = KEY_CODE_5;
        m_Keymap[0x18] = KEY_CODE_EQUALS;
        m_Keymap[0x19] = KEY_CODE_9;
        m_Keymap[0x1A] = KEY_CODE_7;
        m_Keymap[0x1B] = KEY_CODE_MINUS;
        m_Keymap[0x1C] = KEY_CODE_8;
        m_Keymap[0x1D] = KEY_CODE_0;
        m_Keymap[0x1E] = KEY_CODE_RIGHT_BRACKET;
        m_Keymap[0x1F] = KEY_CODE_O;
        m_Keymap[0x20] = KEY_CODE_U;
        m_Keymap[0x21] = KEY_CODE_LEFT_BRACKET;
        m_Keymap[0x22] = KEY_CODE_I;
        m_Keymap[0x23] = KEY_CODE_P;
        m_Keymap[0x25] = KEY_CODE_L;
        m_Keymap[0x26] = KEY_CODE_J;
        m_Keymap[0x27] = KEY_CODE_QUOTE;
        m_Keymap[0x28] = KEY_CODE_K;
        m_Keymap[0x29] = KEY_CODE_SEMICOLON;
        m_Keymap[0x2A] = KEY_CODE_BACKSLASH;
        m_Keymap[0x2B] = KEY_CODE_COMMA;
        m_Keymap[0x2C] = KEY_CODE_SLASH;
        m_Keymap[0x2D] = KEY_CODE_N;
        m_Keymap[0x2E] = KEY_CODE_M;
        m_Keymap[0x2F] = KEY_CODE_PERIOD;
        m_Keymap[0x32] = KEY_CODE_BACKQUOTE;
        m_Keymap[0x41] = KEY_CODE_KP_PERIOD;
        m_Keymap[0x43] = KEY_CODE_KP_MULTIPLY;
        m_Keymap[0x45] = KEY_CODE_KP_PLUS;
        m_Keymap[0x4B] = KEY_CODE_KP_DIVIDE;
        m_Keymap[0x4C] = KEY_CODE_KP_ENTER;
        m_Keymap[0x4E] = KEY_CODE_KP_MINUS;
        m_Keymap[0x51] = KEY_CODE_KP_EQUALS;
        m_Keymap[0x52] = KEY_CODE_KP0;
        m_Keymap[0x53] = KEY_CODE_KP1;
        m_Keymap[0x54] = KEY_CODE_KP2;
        m_Keymap[0x55] = KEY_CODE_KP3;
        m_Keymap[0x56] = KEY_CODE_KP4;
        m_Keymap[0x57] = KEY_CODE_KP5;
        m_Keymap[0x58] = KEY_CODE_KP6;
        m_Keymap[0x59] = KEY_CODE_KP7;
        m_Keymap[0x5B] = KEY_CODE_KP8;
        m_Keymap[0x5C] = KEY_CODE_KP9;
        m_Keymap[0x24] = KEY_CODE_RETURN;
        m_Keymap[0x30] = KEY_CODE_TAB;
        m_Keymap[0x31] = KEY_CODE_SPACE;
        m_Keymap[0x33] = KEY_CODE_BACKSPACE;
        m_Keymap[0x35] = KEY_CODE_ESCAPE;
        m_Keymap[0x37] = KEY_CODE_LEFT_COMMAND;
        m_Keymap[0x36] = KEY_CODE_RIGHT_COMMAND;
        m_Keymap[0x38] = KEY_CODE_LEFT_SHIFT;
        m_Keymap[0x39] = KEY_CODE_CAPSLOCK;
        m_Keymap[0x3A] = KEY_CODE_LEFT_ALT;
        m_Keymap[0x3D] = KEY_CODE_RIGHT_ALT;
        m_Keymap[0x3B] = KEY_CODE_LEFT_CTRL;
        m_Keymap[0x3C] = KEY_CODE_RIGHT_SHIFT;
        m_Keymap[0x3E] = KEY_CODE_RIGHT_CTRL;
        m_Keymap[0x3F] = KEY_CODE_FUNCTION;
        m_Keymap[0x60] = KEY_CODE_F5;
        m_Keymap[0x61] = KEY_CODE_F6;
        m_Keymap[0x62] = KEY_CODE_F7;
        m_Keymap[0x63] = KEY_CODE_F3;
        m_Keymap[0x64] = KEY_CODE_F8;
        m_Keymap[0x65] = KEY_CODE_F9;
        m_Keymap[0x67] = KEY_CODE_F11;
        m_Keymap[0x69] = KEY_CODE_F13;
        m_Keymap[0x6B] = KEY_CODE_F14;
        m_Keymap[0x6D] = KEY_CODE_F10;
        m_Keymap[0x6F] = KEY_CODE_F12;
        m_Keymap[0x71] = KEY_CODE_F15;
        m_Keymap[0x73] = KEY_CODE_HOME;
        m_Keymap[0x74] = KEY_CODE_PAGEUP;
        m_Keymap[0x75] = KEY_CODE_DELETE;
        m_Keymap[0x76] = KEY_CODE_F4;
        m_Keymap[0x77] = KEY_CODE_END;
        m_Keymap[0x78] = KEY_CODE_F2;
        m_Keymap[0x79] = KEY_CODE_PAGEDOWN;
        m_Keymap[0x7A] = KEY_CODE_F1;
        m_Keymap[0x7B] = KEY_CODE_LEFT;
        m_Keymap[0x7C] = KEY_CODE_RIGHT;
        m_Keymap[0x7D] = KEY_CODE_DOWN;
        m_Keymap[0x7E] = KEY_CODE_UP;
    }
}

- (void)initHumanInterfaceDevice
{
    //Safety check that the hid manager isn't already created
    if(m_HumanInterfaceDeviceManager == nil)
    {
        //Local variable we will use later in the method
		CFStringRef keys[2];
		int value;
		CFNumberRef values[2];
		CFDictionaryRef dictionaries[3];
	
        //Create the human interface manager
		m_HumanInterfaceDeviceManager = IOHIDManagerCreate(kCFAllocatorDefault, kIOHIDOptionsTypeNone);
        
        //Open the human interface manager and schedule it with the main run loop
		IOHIDManagerOpen(m_HumanInterfaceDeviceManager, kIOHIDOptionsTypeNone);
		IOHIDManagerScheduleWithRunLoop(m_HumanInterfaceDeviceManager, CFRunLoopGetCurrent(), kCFRunLoopDefaultMode);
		
        //Set the usage keys
		keys[0] = CFSTR(kIOHIDDeviceUsagePageKey);
		keys[1] = CFSTR(kIOHIDDeviceUsageKey);
		
        //Create joystick usage values that match the keys
		value = kHIDPage_GenericDesktop;
		values[0] = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, &value);
		value = kHIDUsage_GD_Joystick;
		values[1] = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, &value);
        
        //Create a dictionary entry with the keys and values
		dictionaries[0] = CFDictionaryCreate(kCFAllocatorDefault, (const void **) keys, (const void **) values, 2, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
		
        //Release the value objects
        CFRelease(values[0]);
		CFRelease(values[1]);
		
        //Create gamepad usage values that match the keys
		value = kHIDPage_GenericDesktop;
		values[0] = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, &value);
        value = kHIDUsage_GD_GamePad;
		values[1] = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, &value);
		
        //Create a dictionary entry with the keys and values
        dictionaries[1] = CFDictionaryCreate(kCFAllocatorDefault, (const void **) keys, (const void **) values, 2, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
		
        //Release the value objects
        CFRelease(values[0]);
		CFRelease(values[1]);
		
        //Create multiaxs controller usage values that match the keys
		value = kHIDPage_GenericDesktop;
		values[0] = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, &value);
		value = kHIDUsage_GD_MultiAxisController;
		values[1] = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, &value);
        
        //Create a dictionary entry with the keys and values
		dictionaries[2] = CFDictionaryCreate(kCFAllocatorDefault, (const void **) keys, (const void **) values, 2, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
		
        //Release the value objects
        CFRelease(values[0]);
		CFRelease(values[1]);
		
        //Create an array for device callbacks for the dictionary entries
		CFArrayRef array = CFArrayCreate(kCFAllocatorDefault, (const void **)dictionaries, 3, &kCFTypeArrayCallBacks);
		
        //Release the dictionary entries
        CFRelease(dictionaries[0]);
		CFRelease(dictionaries[1]);
		CFRelease(dictionaries[2]);
        
        //Set the array for the hid manager
		IOHIDManagerSetDeviceMatchingMultiple(m_HumanInterfaceDeviceManager, array);
        
        //Release the array
		CFRelease(array);
		
        //Register callback methods for when hid are plugged and unplugged
		IOHIDManagerRegisterDeviceMatchingCallback(m_HumanInterfaceDeviceManager, OnDeviceMatched, self);
		IOHIDManagerRegisterDeviceRemovalCallback(m_HumanInterfaceDeviceManager, OnDeviceRemoved, self);
    }
}

- (void)renewGState
{	
	// Called whenever graphics state updated (such as window resize)
	
	// OpenGL rendering is not synchronous with other rendering on the OSX.
	// Therefore, call disableScreenUpdatesUntilFlush so the window server
	// doesn't render non-OpenGL content in the window asynchronously from
	// OpenGL content, which could cause flickering.  (non-OpenGL content
	// includes the title bar and drawing done by the app with other APIs)
	[[self window] disableScreenUpdatesUntilFlush];

	[super renewGState];
}

- (void)drawRect:(NSRect) theRect
{
	//Avoid flickering during resize by drawiing
	[self gameLoop];
}

- (BOOL)acceptsFirstResponder
{
    return YES;
}

- (void)keyDown:(NSEvent*)aEvent
{
    GameDev2D::ServiceLocator::GetInputManager()->HandleKeyDown(m_Keymap[[aEvent keyCode]]);
}

- (void)keyUp:(NSEvent*)aEvent
{
    GameDev2D::ServiceLocator::GetInputManager()->HandleKeyUp(m_Keymap[[aEvent keyCode]]);
}

- (void)flagsChanged:(NSEvent *)aEvent
{
	if([aEvent keyCode] == 0)
    {
		return;
	}
    
    //Get the keycode
    GameDev2D::KeyCode keyCode = m_Keymap[[aEvent keyCode]];
    GameDev2D::KeyEventType keyEventType = GameDev2D::KeyUnknown;

    //Handle the modifier keys
	switch(keyCode)
    {
		case GameDev2D::KEY_CODE_LEFT_ALT:
			if (([aEvent modifierFlags] & 0x80120) == 0x80120)
            {
				keyEventType = GameDev2D::KeyDown;
			}
            else
            {
				keyEventType = GameDev2D::KeyUp;
			}
            break;
            
		case GameDev2D::KEY_CODE_RIGHT_ALT:
			if (([aEvent modifierFlags] & 0x80140) == 0x80140)
            {
				keyEventType = GameDev2D::KeyDown;
			}
            else
            {
				keyEventType = GameDev2D::KeyUp;
			}
            break;
        
		case GameDev2D::KEY_CODE_LEFT_SHIFT:
			if (([aEvent modifierFlags] & 0x20102) == 0x20102)
            {
				keyEventType = GameDev2D::KeyDown;
			}
            else
            {
				keyEventType = GameDev2D::KeyUp;
			}
            break;
            
		case GameDev2D::KEY_CODE_RIGHT_SHIFT:
			if (([aEvent modifierFlags] & 0x20104) == 0x20104)
            {
				keyEventType = GameDev2D::KeyDown;
			}
            else
            {
				keyEventType = GameDev2D::KeyUp;
			}
            break;
            
		case GameDev2D::KEY_CODE_LEFT_CTRL:
			if (([aEvent modifierFlags] & 0x40101) == 0x40101)
            {
				keyEventType = GameDev2D::KeyDown;
			}
            else
            {
				keyEventType = GameDev2D::KeyUp;
			}
            break;
        	
		case GameDev2D::KEY_CODE_RIGHT_CTRL:
			//No right control on my mac
            break;
            
		case GameDev2D::KEY_CODE_LEFT_COMMAND:
			if (([aEvent modifierFlags] & 0x100108) == 0x100108)
            {
				keyEventType = GameDev2D::KeyDown;
			}
            else
            {
				keyEventType = GameDev2D::KeyUp;
			}
		break;
        
		case GameDev2D::KEY_CODE_RIGHT_COMMAND:
			if (([aEvent modifierFlags] & 0x100110) == 0x100110)
            {
				keyEventType = GameDev2D::KeyDown;
			}
            else
            {
				keyEventType = GameDev2D::KeyUp;
			}
		break;
		default:
			//We don't care otherwise
			break;
	}
    
    //Handle the key up or key down event
    if(keyEventType == GameDev2D::KeyUp)
    {
        GameDev2D::ServiceLocator::GetInputManager()->HandleKeyUp(keyCode);
    }
    else if(keyEventType == GameDev2D::KeyDown)
    {
        GameDev2D::ServiceLocator::GetInputManager()->HandleKeyDown(keyCode);
    }
}

- (void)mouseDown:(NSEvent *)aEvent
{
    //Get the mouse's position
	NSPoint position = [self convertPoint:[aEvent locationInWindow] fromView:self];
    
    //Scale the position if retina is supported
    #if OSX_SCALE_TO_RETINA_SIZE
    position = [self convertPointToBacking:position];
    #endif
    
    //Pass the position on to the input manager
    GameDev2D::ServiceLocator::GetInputManager()->HandleLeftMouseDown(position.x, position.y);
}

- (void)mouseUp:(NSEvent *)aEvent
{
    //Get the mouse's current position
	NSPoint position = [self convertPoint:[aEvent locationInWindow] fromView:self];
    
    //Scale the position if retina is supported
    #if OSX_SCALE_TO_RETINA_SIZE
    position = [self convertPointToBacking:position];
    #endif
    
    //Pass the position on to the input manager
    GameDev2D::ServiceLocator::GetInputManager()->HandleLeftMouseUp(position.x, position.y);
}

- (void)rightMouseDown:(NSEvent *)aEvent
{
    //Get the mouse's current position
	NSPoint position = [self convertPoint:[aEvent locationInWindow] fromView:self];
    
    //Scale the position if retina is supported
    #if OSX_SCALE_TO_RETINA_SIZE
    position = [self convertPointToBacking:position];
    #endif
    
    //Pass the position on to the input manager
    GameDev2D::ServiceLocator::GetInputManager()->HandleRightMouseDown(position.x, position.y);
}

- (void)rightMouseUp:(NSEvent *)aEvent
{
    //Get the mouse's current position
	NSPoint position = [self convertPoint:[aEvent locationInWindow] fromView:self];
    
    //Scale the position if retina is supported
    #if OSX_SCALE_TO_RETINA_SIZE
    position = [self convertPointToBacking:position];
    #endif
    
    //Pass the position on to the input manager
    GameDev2D::ServiceLocator::GetInputManager()->HandleRightMouseUp(position.x, position.y);
}

- (void)otherMouseDown:(NSEvent *)aEvent
{
    //Get the mouse's current position
	NSPoint position = [self convertPoint:[aEvent locationInWindow] fromView:self];
    
    //Scale the position if retina is supported
    #if OSX_SCALE_TO_RETINA_SIZE
    position = [self convertPointToBacking:position];
    #endif
    
    //Pass the position on to the input manager
    GameDev2D::ServiceLocator::GetInputManager()->HandleCenterMouseDown(position.x, position.y);
}

- (void)otherMouseUp:(NSEvent *)aEvent
{
    //Get the mouse's current position
	NSPoint position = [self convertPoint:[aEvent locationInWindow] fromView:self];
    
    //Scale the position if retina is supported
    #if OSX_SCALE_TO_RETINA_SIZE
    position = [self convertPointToBacking:position];
    #endif
    
    //Pass the position on to the input manager
    GameDev2D::ServiceLocator::GetInputManager()->HandleCenterMouseUp(position.x, position.y);
}

- (void)mouseMoved:(NSEvent *)aEvent
{
    //Get the mouse's current position and the delta values
	NSPoint position = [self convertPoint:[aEvent locationInWindow] fromView:self];
    float deltaX = [aEvent deltaX];
    float deltaY = [aEvent deltaY];
    
    //Scale the position if retina is supported
    #if OSX_SCALE_TO_RETINA_SIZE
    position = [self convertPointToBacking:position];
    deltaX *= GameDev2D::ServiceLocator::GetPlatformLayer()->GetScale();
    deltaY *= GameDev2D::ServiceLocator::GetPlatformLayer()->GetScale();
    #endif
    
    //Pass the position on to the input manager
    GameDev2D::ServiceLocator::GetInputManager()->HandleMouseMoved(position.x, position.y, deltaX, deltaY);
}

- (void)mouseDragged:(NSEvent *)aEvent
{
    //Get the mouse's current position and the delta values
	NSPoint position = [self convertPoint:[aEvent locationInWindow] fromView:self];
    float deltaX = [aEvent deltaX];
    float deltaY = [aEvent deltaY];
    
    //Scale the position if retina is supported
    #if OSX_SCALE_TO_RETINA_SIZE
    position = [self convertPointToBacking:position];
    deltaX *= GameDev2D::ServiceLocator::GetPlatformLayer()->GetScale();
    deltaY *= GameDev2D::ServiceLocator::GetPlatformLayer()->GetScale();
    #endif
    
    //Pass the position and delta on to the input manager
    GameDev2D::ServiceLocator::GetInputManager()->HandleMouseMoved(position.x, position.y, deltaX, deltaY);
}

- (void)scrollWheel:(NSEvent *)aEvent
{
    float delta = [aEvent deltaY];
    GameDev2D::ServiceLocator::GetInputManager()->HandleScrollWheel(delta);
}

- (void)fixCursor
{
    [[NSCursor crosshairCursor] push];
    [NSCursor pop];
}

- (void)mouseEntered:(NSEvent *)aEvent
{
    GameDev2D::Platform_OSX* platformLayer = (GameDev2D::Platform_OSX*)GameDev2D::ServiceLocator::GetPlatformLayer();
    if(platformLayer->IsMouseCursorVisible() == false)
    {
        CGDisplayHideCursor(kCGDirectMainDisplay);
    }

    [super mouseEntered:aEvent];
}

- (void)mouseExited:(NSEvent *)aEvent
{
    GameDev2D::Platform_OSX* platformLayer = (GameDev2D::Platform_OSX*)GameDev2D::ServiceLocator::GetPlatformLayer();
    if(platformLayer->IsMouseCursorVisible() == false)
    {
        CGDisplayShowCursor(kCGDirectMainDisplay);
    }

    [super mouseExited:aEvent];
}

@end



static Boolean IOHIDDevice_GetNSIntegerProperty(IOHIDDeviceRef inIOHIDDeviceRef, CFStringRef inKey, NSInteger* outValue)
{
	Boolean result = FALSE;
	
	if(inIOHIDDeviceRef)
    {
		CFTypeRef tCFTypeRef = IOHIDDeviceGetProperty(inIOHIDDeviceRef, inKey);
		if(tCFTypeRef)
        {
			if(CFNumberGetTypeID() == CFGetTypeID(tCFTypeRef))
            {
				result = CFNumberGetValue((CFNumberRef) tCFTypeRef, kCFNumberNSIntegerType, outValue);
			}
		}
	}
	return result;
}

NSInteger IOHIDDevice_GetProductID(IOHIDDeviceRef inIOHIDDeviceRef)
{
	NSInteger result = 0;
	(void)IOHIDDevice_GetNSIntegerProperty(inIOHIDDeviceRef, CFSTR(kIOHIDProductIDKey), &result);
	return result;
}

NSInteger IOHIDDevice_GetVendorID(IOHIDDeviceRef inIOHIDDeviceRef)
{
	NSInteger result = 0;
	(void)IOHIDDevice_GetNSIntegerProperty(inIOHIDDeviceRef, CFSTR(kIOHIDVendorIDKey), &result);
	return result;
}

//
static void OnDeviceMatched(void* aContext, IOReturn aResult, void* aSender, IOHIDDeviceRef aDevice)
{
    //Get the vendor id and the product id of the human interface device
    int vendorId = (int)IOHIDDevice_GetVendorID(aDevice);
    int productId = (int)IOHIDDevice_GetProductID(aDevice);
    
    //Create a ControllerData object, pass it the device, vendor id and product id
    GameDev2D::ControllerData* controllerData = new GameDev2D::ControllerData(aDevice, vendorId, productId);
    assert(controllerData != nullptr);
    
    //Pass the controller data onto the input manager, it will
    GameDev2D::ControllerGeneric* controller = GameDev2D::ServiceLocator::GetInputManager()->HandleMatchedController(controllerData);
    assert(controller != nullptr);

    //Cycle through all the elements (buttons and axis) and register them with the controller data object
	CFArrayRef elements = IOHIDDeviceCopyMatchingElements(aDevice, NULL, kIOHIDOptionsTypeNone);
    CFIndex elementIndex;
	for(elementIndex = 0; elementIndex < CFArrayGetCount(elements); elementIndex++)
    {
        //Get the current input element and its type
		IOHIDElementRef element = (IOHIDElementRef) CFArrayGetValueAtIndex(elements, elementIndex);
		IOHIDElementType type = IOHIDElementGetType(element);
		
		//All of the axis elements I've ever detected have been kIOHIDElementTypeInput_Misc,
        //kIOHIDElementTypeInput_Axis is only included for good faith...
		if (type == kIOHIDElementTypeInput_Misc || type == kIOHIDElementTypeInput_Axis)
        {
            //Get the element cookie, the cookie is the input binding for the current element
            IOHIDElementCookie cookie = IOHIDElementGetCookie(element);
            
            //Get the min and max this axis element
            CFIndex min = IOHIDElementGetLogicalMin(element);
            CFIndex max = IOHIDElementGetLogicalMax(element);
            
            //Validate the input binding, for instance the PS3 has a lot of extra input elements,
            //assuming they're mostly from gyro/accelerometer in the controller
            if(controller->IsBindingValid((uint32_t)cookie) == true)
            {
                long value = controller->IsAnalogStick((uint32_t)cookie) == true && controller->GetType() == "ControllerPS3" ? ((max - min) / 2) + min : 0;
                bool platformInverted = false;
                
                //On OSX, the vertical sticks need to be platform inverted, they're fine on windows
                if((controller->GetType() == "ControllerPS3" && ((uint32_t)cookie == GameDev2D::ControllerPS3::AnalogStickLeftVertical() || (uint32_t)cookie == GameDev2D::ControllerPS3::AnalogStickRightVertical())) ||
                   (controller->GetType() == "ControllerXbox360" && ((uint32_t)cookie == GameDev2D::ControllerXbox360::AnalogStickLeftVertical() || (uint32_t)cookie == GameDev2D::ControllerXbox360::AnalogStickRightVertical())))
                {
                    platformInverted = true;
                }
                
                controllerData->AddAnalog((uint32_t)cookie, min, max, value, platformInverted);
            }
		}
        else if (type == kIOHIDElementTypeInput_Button)
        {
            //Get the element cookie, the cookie is the input binding for the current element
            IOHIDElementCookie cookie = IOHIDElementGetCookie(element);
            
            //Validate the input binding
            if(controller->IsBindingValid((uint32_t)cookie) == true)
            {
                controllerData->AddButton((uint32_t)cookie);
            }
		}
	}
    
    //Release the input elements array
	CFRelease(elements);
		  
    //Register the input callback for the device, pass in the controller object as the context,
    //it will be passed as the context for the callback function
    IOHIDDeviceRegisterInputValueCallback(aDevice, OnDeviceValueChanged, controller);
}

static void OnDeviceRemoved(void* aContext, IOReturn aResult, void* aSender, IOHIDDeviceRef aDevice)
{
    //Unregister the input callback function for the device
    IOHIDDeviceRegisterInputValueCallback(aDevice, NULL, NULL);

    //Tell the input manager that the device has been removed
    GameDev2D::ServiceLocator::GetInputManager()->HandleRemovedController(aDevice);
}

static void OnDeviceValueChanged(void* aContext, IOReturn aResult, void* aSender, IOHIDValueRef aValue)
{
    //Get the input element and cookie
    IOHIDElementRef element = IOHIDValueGetElement(aValue);
    IOHIDElementCookie cookie = IOHIDElementGetCookie(element);
    
    //Workaround for a strange crash that occurs with PS3 controller; was getting lengths of 39!
    if(IOHIDValueGetLength(aValue) > 4)
    {
        return;
    }
    
    //Get the logical value
    CFIndex logicalValue = IOHIDValueGetIntegerValue(aValue);

    //Pass the logical value on to the input manager for the cookie (input binding)
    GameDev2D::ServiceLocator::GetInputManager()->HandleControllerInput((GameDev2D::ControllerGeneric*)aContext, (unsigned int)cookie, logicalValue);
}
