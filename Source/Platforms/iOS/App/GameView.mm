//
//  GameView.mm
//  GameDevFramework
//
//  Created by Bradley Flood on 12-08-30.
//  Copyright (c) 2012 Algonquin College. All rights reserved.
//

#import "GameView.h"
#import "OpenGLView.h"
#import <QuartzCore/QuartzCore.h>
#import <CoreMotion/CoreMotion.h>
#include "ServiceLocator.h"
#include "Platform_iOS.h"
#include "InputManager.h"


//Hidden methods
@interface GameView()

-(void)gameLoop;
-(CGPoint)scaleTouchLocation:(CGPoint)touchLocation;
-(void)resize:(NSNotification*)notification;

@end


@implementation GameView

@synthesize openGLView = m_OpenGLView;

- (void)dealloc
{
    [m_OpenGLView release];
    [m_DisplayLink release];
    [m_MotionManager release];
    [super dealloc];
}

- (void)viewDidLoad
{
    //Create the display link, this will call gameLoop everytime the iOS device's screen refreshes
    m_DisplayLink = [[CADisplayLink displayLinkWithTarget:self selector:@selector(gameLoop)] retain];
    [m_DisplayLink setFrameInterval:1];
    
    //Allocate an instance of the MotionManager
    m_MotionManager = [[CMMotionManager alloc] init];
    
    //Register for orientation change notifications
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(resize:) name:UIDeviceOrientationDidChangeNotification object:nil];
    
    [super viewDidLoad];
}

- (BOOL)shouldAutorotate
{
    return YES;
}

- (NSUInteger)supportedInterfaceOrientations
{
    //Get the PlatformLayer pointer from the ServiceLocator
    GameDev2D::Platform_iOS* platformLayer = (GameDev2D::Platform_iOS*)GameDev2D::ServiceLocator::GetPlatformLayer();
    
    //Safety check the platformLayer pointer
    if(platformLayer != nullptr)
    {
        NSUInteger supportedOrientations = 0;
    
        //Is the Portrait orientation supported?
        if((platformLayer->GetSupportedOrientationMask() & GameDev2D::OrientationPortrait) != 0)
        {
            supportedOrientations |= UIInterfaceOrientationMaskPortrait | UIInterfaceOrientationMaskPortraitUpsideDown;
        }
        
        //Is the Landscape orientation supported?
        if((platformLayer->GetSupportedOrientationMask() & GameDev2D::OrientationLandscape) != 0)
        {
            supportedOrientations |= UIInterfaceOrientationMaskLandscape;
        }
        
        //Return the supported orientations
        return supportedOrientations;
    }
    
    return UIInterfaceOrientationMaskAll;
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    for(UITouch* touch in touches)
    {
        CGPoint location = [self scaleTouchLocation:[touch locationInView:[self view]]];
        CGPoint previous = [self scaleTouchLocation:[touch previousLocationInView:[self view]]];
        GameDev2D::ServiceLocator::GetInputManager()->HandleTouchBegan(location.x, location.y, previous.x, previous.y);
    }
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    for(UITouch* touch in touches)
    {
        CGPoint location = [self scaleTouchLocation:[touch locationInView:[self view]]];
        CGPoint previous = [self scaleTouchLocation:[touch previousLocationInView:[self view]]];
        GameDev2D::ServiceLocator::GetInputManager()->HandleTouchMoved(location.x, location.y, previous.x, previous.y);
    }
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    for(UITouch* touch in touches)
    {
        CGPoint location = [self scaleTouchLocation:[touch locationInView:[self view]]];
        CGPoint previous = [self scaleTouchLocation:[touch previousLocationInView:[self view]]];
        GameDev2D::ServiceLocator::GetInputManager()->HandleTouchEnded(location.x, location.y, previous.x, previous.y);
    }
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
    for(UITouch* touch in touches)
    {
        CGPoint location = [self scaleTouchLocation:[touch locationInView:[self view]]];
        CGPoint previous = [self scaleTouchLocation:[touch previousLocationInView:[self view]]];
        GameDev2D::ServiceLocator::GetInputManager()->HandleTouchCancelled(location.x, location.y, previous.x, previous.y);
    }
}

- (CGPoint)scaleTouchLocation:(CGPoint)touchLocation
{
    float scale = GameDev2D::ServiceLocator::GetPlatformLayer()->GetScale();
    return CGPointMake(touchLocation.x * scale, touchLocation.y * scale);
}

- (void)resume
{
    [m_DisplayLink addToRunLoop:[NSRunLoop mainRunLoop] forMode:NSDefaultRunLoopMode];
}

- (void)suspend
{
    [m_DisplayLink removeFromRunLoop:[NSRunLoop mainRunLoop] forMode:NSDefaultRunLoopMode];
}

- (void)invalidate
{
    [m_DisplayLink invalidate];
}

- (BOOL)setCurrentContext
{
    return [m_OpenGLView setCurrentContext];
}

- (void)setRenderBufferStorage
{
    [m_OpenGLView setRenderBufferStorage];
}

- (void)flushDrawBuffer
{
    [m_OpenGLView presentRenderBuffer];
}

- (void)setMultipleTouchEnabled:(BOOL)aEnabled
{
    [[self view] setMultipleTouchEnabled:aEnabled];
    [m_OpenGLView setMultipleTouchEnabled:aEnabled];
}

- (BOOL)isMultipleTouchEnabled
{
    return [[self view] isMultipleTouchEnabled] && [m_OpenGLView isMultipleTouchEnabled];
}

- (CMMotionManager*)motionManager
{
    return m_MotionManager;
}

- (void)gameLoop
{
    //Get the PlatformLayer pointer from the ServiceLocator
    GameDev2D::Platform_iOS* platformLayer = (GameDev2D::Platform_iOS*)GameDev2D::ServiceLocator::GetPlatformLayer();
    
    //Safety check the platformLayer pointer
    if(platformLayer != nullptr)
    {
        //If the Game Loop returns false, remove and invalidate the display link
        if(platformLayer->GameLoop() == false)
        {
            //Suspend the DisplayLink (Removes it from the run loop)
            [self suspend];
            
            //Invalidate the game loop
            [self invalidate];
        }
    }
}

- (void)resize:(NSNotification*)notification
{
    //Is the device's orientation landscape?
    bool isOrientationLandscape = UIDeviceOrientationIsLandscape([[UIDevice currentDevice] orientation]);

    //Get the PlatformLayer pointer from the ServiceLocator
    GameDev2D::Platform_iOS* platformLayer = (GameDev2D::Platform_iOS*)GameDev2D::ServiceLocator::GetPlatformLayer();
    
    //Safety check the platformLayer pointer
    if(platformLayer != nullptr)
    {
        //Notify the playformLayer that the orientation has changed
        platformLayer->OrientationChanged(isOrientationLandscape ? GameDev2D::OrientationLandscape : GameDev2D::OrientationPortrait);
    
        //Determine if Portrait orientation is supported
        if((platformLayer->GetSupportedOrientationMask() & GameDev2D::OrientationPortrait) == 0 && isOrientationLandscape == false)
        {
            isOrientationLandscape = true;
        }
        
        //Determine if Landscape orientation is supported
        if((platformLayer->GetSupportedOrientationMask() & GameDev2D::OrientationLandscape) == 0 && isOrientationLandscape == true)
        {
            isOrientationLandscape = false;
        }
    
        //Get the view's width and height
        CGSize viewSize = [[self view] frame].size;
        
        //Set the video mode's width and height
        GameDev2D::VideoModeInfo videoModeInfo;
        videoModeInfo.width = viewSize.width;
        videoModeInfo.height = viewSize.height;
        
        //Ensure the width and height match the orientation
        if(isOrientationLandscape == true)
        {
            videoModeInfo.width = viewSize.width > viewSize.height ? viewSize.width : viewSize.height;
            videoModeInfo.height = viewSize.width > viewSize.height ? viewSize.height : viewSize.width;
        }
        
        //Scale the width and height (retina devices)
        float scale = GameDev2D::ServiceLocator::GetPlatformLayer()->GetScale();
        videoModeInfo.width *= scale;
        videoModeInfo.height *= scale;

        //Set the platformLayer's updated video mode
        platformLayer->SetVideoModeInfo(&videoModeInfo);
    }
}

@end
