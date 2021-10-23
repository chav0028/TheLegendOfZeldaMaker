//
//  RenderTarget.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-03-15.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__RenderTarget__
#define __GameDev2D__RenderTarget__

#include "../../Core/BaseObject.h"
#include "../OpenGL.h"
#include "FrameBuffer.h"
#include "Color.h"
#include "png.h"


namespace GameDev2D
{
    //Forward declarations
    class Texture;
    class TextureFrame;
    
    //The RenderTarget class allows you to render to a texture OR a seperate storage buffer (iOS).
    //This can allow you to batch render calls together into one texture to save on draw calls.
    class RenderTarget : public BaseObject
    {
    public:
        //Create a RenderTarget for a given size
        RenderTarget(unsigned int width, unsigned int height);
        ~RenderTarget();
        
        //Returns the width and height of the RenderTarget
        int GetWidth();
        int GetHeight();
        
        //Sets the clear color for the RenderTarget
        void SetClearColor(Color clearColor);
        
        //Returns the clear color for the RenderTarget
        Color GetClearColor();
        
        //Returns the TextureFrame associated with the RenderTarget, may be null.
        //On iOS there must be a RenderTarget that render into a special iOS storage
        //buffer, so in that case this method will return null.
        TextureFrame* GetTextureFrame();
        
        //Binds the RenderTarget
        void Bind();
        
        //Unbinds the RenderTarget
        void Unbind();
        
        //Returns wether the RenderTarget is bound
        bool IsBound();
        
        //Methods used to create and destroy the frame buffer
        void CreateFrameBuffer();
        void DestroyFrameBuffer();

    protected:
        //Called by the Graphics class, iOS must have a specific RenderTarget to draw to
        RenderTarget();
        
        //Sets wether the RenderTarget is active or not
        void SetIsActive(bool isActive);

        //Member variables
        FrameBuffer m_FrameBuffer;
        Color m_ClearColor;
        TextureFrame* m_TextureFrame;
        bool m_IsBound;
        
        //Friend class, Graphics needs to access the protected methods
        friend class Graphics;
    };
}

#endif /* defined(__GameDev2D__RenderTarget__) */
