//
//  AppDelegate.m
//  GameDev2D - OSX
//
//  Created by Bradley Flood on 2014-04-22.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#import "AppDelegate.h"
#include "Game.h"
#include "Splash.h"
#include "MainMenu.h"
#include "LevelEditor.h"
#include "Examples.h"


@implementation AppDelegate

@synthesize window = m_Window;
@synthesize gameView = m_GameView;

- (void)dealloc
{
    //Release the gameview and window
    [m_GameView release];
    [m_Window release];
    [super dealloc];
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    //Create the platform layer (OSX) and set the game view
    using namespace GameDev2D;
    {
        m_Platform_OSX = new Platform_OSX(m_GameView, OSX_WINDOW_TITLE, OSX_TARGET_FPS);
        m_Platform_OSX->Init(OSX_DEFAULT_WIDTH, OSX_DEFAULT_HEIGHT, OSX_DEFAULT_FULLSCREEN, OSX_DEFAULT_VERTICAL_SYNC);
        m_Platform_OSX->SetRootScene(new ROOT_SCENE());
    }
}

- (void)applicationWillBecomeActive:(NSNotification *)notification
{
    //Notify the Platform Layer about the resume event
    if(m_Platform_OSX != nullptr)
    {
        m_Platform_OSX->Resume();
    }
}

- (void)applicationWillResignActive:(NSNotification *)notification
{
    //Notify the Platform Layer about the suspend event
    if(m_Platform_OSX != nullptr)
    {
        m_Platform_OSX->Suspend();
    }
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)application
{
    return YES;
}

- (void)applicationWillTerminate:(NSNotification *)notification
{
    //Notify the Platform Layer about the shutdown event
    if(m_Platform_OSX != nullptr)
    {
        m_Platform_OSX->Shutdown();
    }
    
    //Delete the Platform Layer object
    SafeDelete(m_Platform_OSX);
    
    //Setting the delegate to nil will call the dealloc method
    [[NSApplication sharedApplication] setDelegate:nil];
}

@end
