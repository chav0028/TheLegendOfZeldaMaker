//
//  RenderTarget.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-03-15.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//


#include "RenderTarget.h"
#include "../Textures/Texture.h"
#include "../Textures/TextureFrame.h"
#include "../../Services/ServiceLocator.h"
#include "../../Platforms/PlatformLayer.h"


namespace GameDev2D
{
    RenderTarget::RenderTarget() : BaseObject("RenderTarget"),
        m_TextureFrame(nullptr),
        m_IsBound(false)
    {
        //Initialize the framebuffer
        size_t size = sizeof(m_FrameBuffer);
        memset(&m_FrameBuffer, 0, size);
        
        //Set the clear color
        SetClearColor(GRAPHICS_DEFAULT_CLEAR_COLOR);
    }
    
    RenderTarget::RenderTarget(unsigned int aWidth, unsigned int aHeight) : BaseObject("RenderTarget"),
        m_TextureFrame(nullptr),
        m_IsBound(false)
    {
        //Initialize the framebuffer
        size_t size = sizeof(m_FrameBuffer);
        memset(&m_FrameBuffer, 0, size);
        
        //Set the clear color
        SetClearColor(Color::ClearColor());
        
        //Create the texture frame for the width and height
        m_TextureFrame = new TextureFrame(aWidth, aHeight);
    }
    
    RenderTarget::~RenderTarget()
    {
        //
        SafeDelete(m_TextureFrame);
        
        //
        DestroyFrameBuffer();
    }
    
    int RenderTarget::GetWidth()
    {
        return m_FrameBuffer.width;
    }
    
    int RenderTarget::GetHeight()
    {
        return m_FrameBuffer.height;
    }
    
    void RenderTarget::SetClearColor(Color aClearColor)
    {
        m_ClearColor = aClearColor;
        glClearColor(m_ClearColor.Red(), m_ClearColor.Green(), m_ClearColor.Blue(), m_ClearColor.Alpha());
    }
    
    Color RenderTarget::GetClearColor()
    {
        return m_ClearColor;
    }

    TextureFrame* RenderTarget::GetTextureFrame()
    {
        return m_TextureFrame;
    }
    
    void RenderTarget::Bind()
    {
        //
        glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer.frameBuffer);
        
        //
        if(m_TextureFrame == nullptr)
        {
            glBindRenderbuffer(GL_RENDERBUFFER, m_FrameBuffer.renderBuffer);
        }
        
        //
        SetClearColor(m_ClearColor);
        
        //
        m_IsBound = true;
    }
    
    void RenderTarget::Unbind()
    {
        //
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        
        //
        m_IsBound = false;
    }
    
    bool RenderTarget::IsBound()
    {
        return m_IsBound;
    }
    
    void RenderTarget::CreateFrameBuffer()
    {
        //
        int originalFrameBuffer = 0;
        
        //
        if(m_TextureFrame != nullptr && m_TextureFrame->GetTexture() != nullptr)
        {
            glGetIntegerv(GL_FRAMEBUFFER_BINDING, &originalFrameBuffer);
        }
    
        //
        glGenFramebuffers(1, &m_FrameBuffer.frameBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer.frameBuffer);
        
        //
        if(m_TextureFrame == nullptr)
        {
            //
            glGenRenderbuffers(1, &m_FrameBuffer.renderBuffer);
            glBindRenderbuffer(GL_RENDERBUFFER, m_FrameBuffer.renderBuffer);
            
            //
            ServiceLocator::GetPlatformLayer()->SetRenderBufferStorage();

            //
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_FrameBuffer.renderBuffer);
            
            //Get the backbuffer width and height.
            glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &m_FrameBuffer.width);
            glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &m_FrameBuffer.height);
        }
        else
        {
            //
            if(m_TextureFrame->GetTexture()->GetId() == ServiceLocator::GetGraphics()->GetBoundTextureId())
            {
                ServiceLocator::GetGraphics()->BindTexture(nullptr);
            }
        
            //
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureFrame->GetTexture()->GetId(), 0);
            
            //
            m_FrameBuffer.width = (int)m_TextureFrame->GetSourceFrame().size.x;
            m_FrameBuffer.height = (int)m_TextureFrame->GetSourceFrame().size.y;
        }
        
        //
        Log(VerbosityLevel_Graphics, "Create Frame Buffer (%i, %i)", m_FrameBuffer.width, m_FrameBuffer.height);

        
        //Check for errors, if any are found; return NO.
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            Error(false, "Failed to create framebuffer");
        }
        
        //
        if(m_TextureFrame != nullptr && m_TextureFrame->GetTexture() != nullptr)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, originalFrameBuffer);
        }
    }
    
    void RenderTarget::DestroyFrameBuffer()
    {
        //
        Log(VerbosityLevel_Graphics, "Destroy Frame Buffer");
    
        //
        glDeleteFramebuffers(1, &m_FrameBuffer.frameBuffer);
        m_FrameBuffer.frameBuffer = 0;
        
        //
        glDeleteRenderbuffers(1, &m_FrameBuffer.renderBuffer);
        m_FrameBuffer.renderBuffer = 0;
    }
}