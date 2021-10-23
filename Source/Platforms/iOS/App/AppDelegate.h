//
//  AppDelegate.h
//  GameDevFramework
//
//  Created by Bradley Flood on 12-08-27.
//  Copyright (c) 2012 Algonquin College. All rights reserved.
//

#include "Platform_iOS.h"


@interface AppDelegate : UIResponder <UIApplicationDelegate>
{
    UIWindow* m_Window;
    GameDev2D::Platform_iOS* m_Platform_iOS;
}

@property (nonatomic, retain) UIWindow *window;

@end
