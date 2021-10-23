//
//  AppDelegate.h
//  GameDev2D - OSX
//
//  Created by Bradley Flood on 2014-04-22.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "GameViewOSX.h"
#include "Platform_OSX.h"



@interface AppDelegate : NSObject <NSApplicationDelegate>
{
    NSWindow* m_Window;
    GameViewOSX* m_GameView;
    GameDev2D::Platform_OSX* m_Platform_OSX;
}

@property (nonatomic, retain) IBOutlet NSWindow* window;
@property (nonatomic, retain) IBOutlet GameViewOSX* gameView;

@end
