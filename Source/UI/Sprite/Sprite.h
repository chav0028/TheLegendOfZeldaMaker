//
//  Sprite.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-03-05.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__Sprite__
#define __GameDev2D__Sprite__

#include "../../Core/GameObject.h"
#include "../../Graphics/Core/Color.h"
#include "../../Events/EventDispatcher.h"
#include "../../Animation/Tween.h"
#include "../../Animation/Easing.h"


namespace GameDev2D
{
    //Forward declarations
    class TextureFrame;
    class Shader;
    
    //The Sprite class inherits from the GameObject class and the EventDispatcher class. It can display a
    //single framed Sprite OR an animated Sprite. It is designed to load animated sprites or sprites from
    //a texture atlas created using the'Texture Packer' software created by Code and Web https://www.codeandweb.com/texturepacker
    //You can add frames after construction from different Textures. The anchor points, and the alpha, can be animated.
    //This class dispatches the following events:
    // SPRITE_DID_FINISH_ANIMATING
    class Sprite : public GameObject, public EventDispatcher
    {
    public:
        //Constructs a sprite with a single frame, can be loaded from a single image
        //or as an optional texture atlas via the atlas key
        Sprite(const string& filename, const string& atlasKey = "");
        
        //Constructs a sprite with a single frame, can be loaded from a single image
        //or as an optional texture atlas via the atlas key. In addition the shader used
        //can be specified, the Sprite takes ownership of the Shader and will remove it
        Sprite(Shader* shader, const string& filename, const string& atlasKey = "");
        
        //Constructs an animated sprite from a json file, it can be set to loop and optionally
        //the frame speed can be set. A lower frame speed means the slower the sprite animates
        Sprite(const string& imageFile, const string& jsonFile, bool doesLoop, float frameSpeed = 15.0f);
        
        //Constructs an animated sprite from a json file, it can be set to loop and optionally
        //the frame speed can be set. A lower frame speed means the slower the sprite animates.
        //In addition the shader used can be specified, the Sprite takes ownership of the Shader
        //and will remove it
        Sprite(Shader* shader, const string& imageFile, const string& jsonFile, bool doesLoop, float frameSpeed = 15.0f);
        virtual ~Sprite();
        
        //Updates the Sprite
        void Update(double delta);
        
        //Draws the Sprite
        void Draw();
        
        //Resets the Sprite, if animated back to the first frame
        void Reset();
        
        //Sets the Shader to render the Sprite with
        void SetShader(Shader* shader);
        
        //Returns the size of the current frame of the Sprite.
        vec2 GetSize();
        float GetWidth();
        float GetHeight();
        
        //Sets the anchor point for the Sprite, can be set to animate over a duration, an easing function can be applied, can be revered and repeated
        void SetAnchorX(float anchorX, double duration = 0.0, EasingFunction easingFunction = Linear::Interpolation, bool reverse = false, int repeatCount = 0);
        void SetAnchorY(float anchorY, double duration = 0.0, EasingFunction easingFunction = Linear::Interpolation, bool reverse = false, int repeatCount = 0);
        void SetAnchorPoint(float anchorX, float anchorY, double duration = 0.0, EasingFunction easingFunction = Linear::Interpolation, bool reverse = false, int repeatCount = 0);
        void SetAnchorPoint(vec2 anchorPoint, double duration = 0.0, EasingFunction easingFunction = Linear::Interpolation, bool reverse = false, int repeatCount = 0);
        
        //Returns the anchor point of the Sprite
        vec2 GetAnchorPoint();
    
        //Sets the alpha for the Sprite, can be set to animate over a duration, an easing function can be applied, can be revered and repeated
        void SetAlpha(float alpha, double duration = 0.0, EasingFunction easingFunction = Linear::Interpolation, bool reverse = false, int repeatCount = 0);
        
        //Returns the alpha for the Sprite
        float GetAlpha();
        
        //Adds a frame to the Sprite, can make a non animated sprite, animated
        void AddFrame(TextureFrame* textureFrame);
        void AddFrame(const string& filename, const string& atlasKey = "");
        
        //Returns if this Sprite is animated, simply checks if there are more than one frame
        bool IsAnimated();
        
        //Returns the frame count
        unsigned long GetFrameCount();
  
        //Returns the frame index
        int GetFrameIndex();
        
        //Sets the frame index
        void SetFrameIndex(unsigned int frameIndex);
 
        //Returns wether this Sprite loops, must be animated
        bool DoesLoop();
        
        //Sets if the Sprite loops, must be animated
        void SetDoesLoop(bool doesLoop);
        
        //Returns the frame speed - must be animated
        float GetFrameSpeed();
        
        //Sets the frame speed - must be animated
        void SetFrameSpeed(float frameSpeed);
        
        //Method used to cancel all running tweens
        void CancelTweens();
    
    protected:
        //Used to Reset the Model Matrix
        void ResetModelMatrix();
        
        //Conveniance method that loads the animated sprite from the json file
        void LoadFrames(const string& imageFile, const string& jsonFile);
        
        //Member variables
        vector<TextureFrame*> m_Frames;
        Shader* m_Shader;
        vec2 m_AnchorPoint;
        Color m_Color;
        bool m_DoesLoop;
        float m_FrameSpeed;
        double m_ElapsedTime;
        int m_FrameIndex;
        Tween m_TweenAnchorX;
        Tween m_TweenAnchorY;
        Tween m_TweenAlpha;
        bool m_DidDispatchFinishedEvent;
    };
}

#endif /* defined(__GameDev2D__Sprite__) */
