//
//  AppDelegate.m
//  GameDevFramework
//
//  Created by Bradley Flood on 12-08-27.
//  Copyright (c) 2012 Algonquin College. All rights reserved.
//

#import "AppDelegate.h"
#import "GameView.h"
#include "Game.h"
#include "Examples.h"


@implementation AppDelegate

@synthesize window = m_Window;

- (void)dealloc
{
    //Delete the Platform Layer
    SafeDelete(m_Platform_iOS);

    //Release the window
    [m_Window release];
    
    [super dealloc];
}

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    //Create the window
    m_Window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
    
    //Create the platform layer (iOS) and set the game view
    m_Platform_iOS = new GameDev2D::Platform_iOS(m_Window, IOS_TARGET_FPS);
    m_Platform_iOS->Init();
    m_Platform_iOS->SetRootScene(new GameDev2D::ROOT_SCENE());

    return YES;
}

- (NSUInteger)application:(UIApplication *)application supportedInterfaceOrientationsForWindow:(UIWindow *)window
{
    NSUInteger supportedOrientations = 0;
    unsigned int orientationMask = IOS_SUPPORTED_ORIENTATIONS;
    
    //Get the application's supported orientation mask
    if(m_Platform_iOS != nullptr)
    {
        orientationMask = m_Platform_iOS->GetSupportedOrientationMask();
    }
    
    //Is portrait orientation supported
    if((orientationMask & GameDev2D::OrientationPortrait) != 0)
    {
        supportedOrientations |= UIInterfaceOrientationMaskPortrait | UIInterfaceOrientationMaskPortraitUpsideDown;
    }
    
    //Is landscape orientation supported
    if((orientationMask & GameDev2D::OrientationLandscape) != 0)
    {
        supportedOrientations |= UIInterfaceOrientationMaskLandscape;
    }

    //Return the supported orientation
    return supportedOrientations;
}

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application;
{
    //Notify the Platform Layer about the low memory warning
    if(m_Platform_iOS != nullptr)
    {
        m_Platform_iOS->LowMemoryWarning();
    }
}

- (void)applicationWillResignActive:(UIApplication *)application
{
    //Notify the Platform Layer about the suspend event
    if(m_Platform_iOS != nullptr)
    {
        m_Platform_iOS->Suspend();
    }
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    //Notify the Platform Layer about the resume event
    if(m_Platform_iOS != nullptr)
    {
        m_Platform_iOS->Resume();
    }
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    //Notify the Platform Layer about the shutdown event
    if(m_Platform_iOS != nullptr)
    {
        m_Platform_iOS->Shutdown();
    }
    
    //Setting the delegate to nil will call the dealloc method
    [[UIApplication sharedApplication] setDelegate:nil];
}

@end
