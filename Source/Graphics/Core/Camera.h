//
//  Camera.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-03-12.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__Camera__
#define __GameDev2D__Camera__

#include "../../Core/BaseObject.h"
#include "../../Events/EventHandler.h"
#include "../../Animation/Random.h"
#include "../../Animation/Tween.h"
#include "../../Animation/Easing.h"


using namespace glm;

namespace GameDev2D
{
    //2D Orthographic camera, used by every scene in the game. A scene can have more than one camera,
    //but currently the Graphics service only supports one active camera at a time. The camera positions,
    //orientation, and zoom can be set to animate over a duration, an easing function can be applied,
    //can be revered and repeated. A camera shake can also be applied to the camera view.
    class Camera : public BaseObject, public EventHandler
    {
    public:
        Camera();
        ~Camera();
        
        //Update method
        void Update(double delta);
        
        //EventHandler method
        void HandleEvent(Event* event);
        
        //Getter methods for the projection and view matrices
        mat4 GetProjectionMatrix();
        mat4 GetViewMatrix();
        
        //Gets the view width and height, factors in the current zoom level
        float GetViewWidth();
        float GetViewHeight();
        
        //Get the aspect ratio
        float GetAspectRatio();
        
        //Getter methods for the camera position and orientation
        vec2 GetPosition();
        float GetOrientation();
        
        //Sets the position of the camera, can be set to animate over a duration, an easing function can be applied, can be revered and repeated
        void SetPositionX(float positionX, double duration = 0.0, EasingFunction easingFunction = Linear::Interpolation, bool reverse = false, int repeatCount = 0);
        void SetPositionY(float positionY, double duration = 0.0, EasingFunction easingFunction = Linear::Interpolation, bool reverse = false, int repeatCount = 0);
        void SetPosition(float positionX, float positionY, double duration = 0.0, EasingFunction easingFunction = Linear::Interpolation, bool reverse = false, int repeatCount = 0);
        void SetPosition(vec2 position, double duration = 0.0, EasingFunction easingFunction = Linear::Interpolation, bool reverse = false, int repeatCount = 0);
        
        //Sets the orientation of the camera, can be set to animate over a duration, an easing function can be applied, can be revered and repeated
        void SetOrientation(float orientation, double duration = 0.0, EasingFunction easingFunction = Linear::Interpolation, bool reverse = false, int repeatCount = 0);
        
        //Sets the transform (position and orientation of the camera, can be set to animate over a duration, an easing function can be applied, can be revered and repeated
        void SetTransform(vec2 position, float orientation, double duration = 0.0, EasingFunction easingFunction = Linear::Interpolation, bool reverse = false, int repeatCount = 0);
        
        //Returns the current zoom level of the camera
        float GetZoom();
        
        //Sets the zoom level of the camera, can be set to animate over a duration, an easing function can be applied, can be revered and repeated
        void SetZoom(float zoom, double duration = 0.0, EasingFunction easingFunction = Linear::Interpolation, bool reverse = false, int repeatCount = 0);
        
        //Shakes the camera for a magnitude over a certain duration
        void Shake(float magnitude, double duration);
        
        //Resets the projection and view matrices
        void ResetProjectionMatrix();
        void ResetViewMatrix();
        
    protected:
        //Function pointer for the Camera tween methods
        typedef void (Camera::*CameraTweenSetMethod)(float value, double duration, EasingFunction easingFunction, bool reverse, int repeatCount);
        
        //Conveniance method to update the Camera's tweens
        void UpdateTween(double delta, Tween* tween, CameraTweenSetMethod tweenSetMethod);
    
        //Conveniance method to randomize a camera shake
        float RandomShake(float magnitude);
        
        //Conveniance method to log the matrix data
        void LogMatrix(mat4 matrix, const char* name = nullptr);
    
        //Member variables
        mat4 m_ProjectionMatrix;
        mat4 m_ViewMatrix;
        vec2 m_Position;
        float m_Orientation;
        float m_Zoom;
        bool m_ProjectionMatrixDirty;
        bool m_ViewMatrixDirty;
        
        //Tween member variables
        Tween m_TweenPositionX;
        Tween m_TweenPositionY;
        Tween m_TweenOrientation;
        Tween m_TweenZoom;
        
        //Camera shake member variables
        Random m_ShakeRandom;
        bool m_ShakeEnabled;
        float m_ShakeMagnitude;
        double m_ShakeDuration;
        double m_ShakeTimer;
        vec2 m_ShakeStartingPosition;
        vec2 m_ShakeOffset;
    };
}

#endif /* defined(__GameDev2D__Camera__) */
