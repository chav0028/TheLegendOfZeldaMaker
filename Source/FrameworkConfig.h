//
//  Config.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-03-13.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef GameDev2D_Config_h
#define GameDev2D_Config_h

//General Settings
#define ROOT_SCENE Splash

//iOS Platform Settings
#define IOS_TARGET_FPS 60
#define IOS_TARGET_OPENGL_API kEAGLRenderingAPIOpenGLES3
#define IOS_SUPPORTED_ORIENTATIONS GameDev2D::OrientationPortrait
#define IOS_SCALE_TO_RETINA_SIZE 1

//OSX Platform settings
#define OSX_TARGET_FPS 60
#define OSX_WINDOW_TITLE "GameDev2D - OSX"
#define OSX_DEFAULT_WIDTH 1024
#define OSX_DEFAULT_HEIGHT 832
#define OSX_DEFAULT_FULLSCREEN false
#define OSX_DEFAULT_VERTICAL_SYNC true
#define OSX_SCALE_TO_RETINA_SIZE 0

//Windows Platform settings
#define WINDOWS_TARGET_FPS 60
#define WINDOWS_WINDOW_TITLE "GameDev2D - Windows"
#define WINDOWS_DEFAULT_WIDTH 1024
#define WINDOWS_DEFAULT_HEIGHT 832
#define WINDOWS_DEFAULT_FULLSCREEN false
#define WINDOWS_DEFAULT_VERTICAL_SYNC false

//Graphics
#define GRAPHICS_DEFAULT_CLEAR_COLOR Color::WhiteColor()

//Input
#define SIMULATE_TOUCH_EVENTS_WITH_MOUSE 0

//Errors
#define THROW_EXCEPTION_ON_ERROR 1

//Memory
#define TRACK_MEMORY_USAGE 0
#define PURGE_UNUSED_TEXTURES_IMMEDIATELY 0
#define PURGE_UNUSED_SHADERS_IMMEDIATELY 0
#define PURGE_UNUSED_FONTS_IMMEDIATELY 0

//Logging
#define LOG_VERBOSITY_MASK VerbosityLevel_Debug | VerbosityLevel_Events
#define LOG_TO_FILE 0
#define LOG_FILE "/Log.txt"
#define LOG_ERRORS_TO_FILE 0
#define LOG_ERROR_FILE "/Errors.txt"

#define LOG_PROJECTION_MATRIX 0
#define LOG_VIEW_MATRIX 0

#define LOG_TRUE_TYPE_FONT_GLYPH_DATA 0
#define LOG_BITMAP_FONT_GLYPH_DATA 0

//Draw
#define DRAW_DEBUG_UI 1
#define DEBUG_UI_FONT_COLOR Color::WhiteColor()

#define DRAW_JOYSTICK_DATA 0

#define DRAW_TOUCH_DEBUG_DATA 0
#define DRAW_TOUCH_DEBUG_MOVED_PATH 0
#define DRAW_TOUCH_DEBUG_COLOR Color::BlackColor

#define DRAW_SPRITE_RECT 0
#define DRAW_SPRITE_RECT_COLOR Color::RedColor()

#define DRAW_LABEL_RECT 0
#define DRAW_LABEL_RECT_COLOR Color::RedColor()

#define DRAW_POLYGON_ANCHOR_POINT 0
#define DRAW_POLYGON_ANCHOR_POINT_SIZE 12

#endif
