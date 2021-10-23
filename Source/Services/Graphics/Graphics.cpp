//
//  Graphics.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2013-07-15.
//  Copyright (c) 2013 Algonquin College. All rights reserved.
//

#include "Graphics.h"
#include "../ServiceLocator.h"
#include "../ShaderManager/ShaderManager.h"
#include "../../Platforms/PlatformLayer.h"


namespace GameDev2D
{
    Graphics::Graphics() : GameService("Graphics"),
        m_ActiveRenderTarget(nullptr),
        #if TARGET_OS_IPHONE
        m_MainRenderTarget(nullptr),
        #else
        m_Width(0),
        m_Height(0),
        m_ClearColor(GRAPHICS_DEFAULT_CLEAR_COLOR),
        #endif
        m_DefaultCamera(nullptr),
        m_ActiveCamera(nullptr),
        m_ViewportWidth(0),
        m_ViewportHeight(0),
        m_BoundTextureId(0),
        m_BoundVertexArray(0)
    {
        #if TARGET_OS_IPHONE
        m_MainRenderTarget = new RenderTarget();
        SetActiveRenderTarget(m_MainRenderTarget);
        #else
        SetClearColor(m_ClearColor);
        #endif
        
        //Create the default camera and set it as the active camera
        m_DefaultCamera = new Camera();
        m_ActiveCamera = m_DefaultCamera;
    }
    
    Graphics::~Graphics()
    {
        //Delete the 'main' render target, only used on iOS
        #if TARGET_OS_IPHONE
        SafeDelete(m_MainRenderTarget);
        m_ActiveRenderTarget = nullptr;
        #endif
    
        //Delete the default camera
        SafeDelete(m_DefaultCamera);
        m_ActiveCamera = nullptr;
        
        //Clear the scissor stack
        m_ScissorStack.clear();
    }
    
    void Graphics::Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT);
    }
    
    void Graphics::SetClearColor(Color aColor)
    {
        #if TARGET_OS_IPHONE
        m_MainRenderTarget->SetClearColor(aColor);
        #else
        m_ClearColor = aColor;
        glClearColor(m_ClearColor.Red(), m_ClearColor.Green(), m_ClearColor.Blue(), m_ClearColor.Alpha());
        #endif
    }
    
    Color Graphics::GetClearColor()
    {
        #if TARGET_OS_IPHONE
        return m_MainRenderTarget->GetClearColor();
        #else
        return m_ClearColor;
        #endif
    }
        
    int Graphics::GetWidth()
    {
        if(m_ActiveRenderTarget != nullptr)
        {
            return m_ActiveRenderTarget->GetWidth();
        }
        
        #if !TARGET_OS_IPHONE
        return m_Width;
        #else
        return 0;
        #endif
    }
    
    int Graphics::GetHeight()
    {
        if(m_ActiveRenderTarget != nullptr)
        {
            return m_ActiveRenderTarget->GetHeight();
        }
        #if !TARGET_OS_IPHONE
        return m_Height;
        #else
        return 0;
        #endif
    }
    
    mat4 Graphics::GetProjectionMatrix()
    {
        assert(m_ActiveCamera != nullptr);
        return m_ActiveCamera->GetProjectionMatrix();
    }
    
    mat4 Graphics::GetViewMatrix()
    {
        assert(m_ActiveCamera != nullptr);
        return m_ActiveCamera->GetViewMatrix();
    }
    
    void Graphics::Resize(int aWidth, int aHeight)
    {
        #if TARGET_OS_IPHONE
        //We need to create a new frame buffer since is resized
        m_MainRenderTarget->DestroyFrameBuffer();
        m_MainRenderTarget->CreateFrameBuffer();
        
        //If the assert is hit, the render target's frame buffer was NOT the size we expected to resize to
        assert(m_MainRenderTarget->GetWidth() == aWidth && m_MainRenderTarget->GetHeight() == aHeight);
        #else
        m_Width = aWidth;
        m_Height = aHeight;
        #endif
        
        Log(VerbosityLevel_Graphics, "Resize back buffer (%i, %i)", aWidth, aHeight);
        
        //Lastly set the viewport size
        SetViewportSize(aWidth, aHeight);
    }
    
    void Graphics::ResetViewport()
    {
        glViewport(0, 0, m_ViewportWidth, m_ViewportHeight);
        glScissor(0, 0,  m_ViewportWidth, m_ViewportHeight);
    }
    
    void Graphics::SetViewportSize(int aWidth, int aHeight)
    {
        //Reset the viewport
    	m_ViewportWidth = aWidth;
        m_ViewportHeight = aHeight;
        ResetViewport();
        
        //Reset the projection and view matrices
        if(m_ActiveCamera != nullptr)
        {
            m_ActiveCamera->ResetProjectionMatrix();
            m_ActiveCamera->ResetViewMatrix();
        }
    }
    
    int Graphics::GetViewportWidth()
    {
        return m_ViewportWidth;
    }
    
    int Graphics::GetViewportHeight()
    {
        return m_ViewportHeight;
    }
    
    RenderTarget* Graphics::GetActiveRenderTarget()
    {
        return m_ActiveRenderTarget;
    }
    
    void Graphics::SetActiveRenderTarget(RenderTarget* aRenderTarget)
    {
        //Safety check that the render target isn't null and that it isn't already set
        if(aRenderTarget != m_ActiveRenderTarget)
        {
            //Safety check the active render target and unbind it
            if(m_ActiveRenderTarget != nullptr)
            {
                m_ActiveRenderTarget->Unbind();
            }
            
            //Set the active render target
            m_ActiveRenderTarget = aRenderTarget;
            
            //Safety check the newly active render target
            if(m_ActiveRenderTarget != nullptr)
            {
                //And bind it
                m_ActiveRenderTarget->Bind();
                
                //Set the viewport size to match the render target's size
                SetViewportSize(m_ActiveRenderTarget->GetWidth(), m_ActiveRenderTarget->GetHeight());
                Log(VerbosityLevel_Graphics, "Set active render target: (%i, %i)", m_ActiveRenderTarget->GetWidth(), m_ActiveRenderTarget->GetHeight());
            }
            else
            {
                #if !TARGET_OS_IPHONE
                SetViewportSize(m_Width, m_Height);
                SetClearColor(m_ClearColor);
                #endif
            }
        }
    }
    
    Camera* Graphics::GetActiveCamera()
    {
        return m_ActiveCamera;
    }
    
    void Graphics::SetActiveCamera(Camera* aCamera)
    {
        if(aCamera != nullptr)
        {
            m_ActiveCamera = aCamera;
        }
    }
        
    void Graphics::SetDefaultCamera()
    {
        SetActiveCamera(m_DefaultCamera);
    }
    
    void Graphics::PushScissorClip(float aX, float aY, float aWidth, float aHeight)
    {
        //If this is the first scissor clip, enable scissor clipping
        if(m_ScissorStack.size() == 0)
        {
            glEnable(GL_SCISSOR_TEST);
        }
        
        //Set the scissor clipping rect
        glScissor((GLint)aX, (GLint)aY, (GLsizei)aWidth, (GLsizei)aHeight);
        Log(VerbosityLevel_Graphics, "Push scissor clip: (%f, %f, %f, %f)", aX, aY, aWidth, aHeight);
        
        //Push the scissor clip onto the scissor stack
        m_ScissorStack.push_back(make_pair(vec2(aX, aY), vec2(aWidth, aHeight)));
    }
    
    void Graphics::PopScissorClip()
    {
        //Pop back the scissor stack
        m_ScissorStack.pop_back();
        Log(VerbosityLevel_Graphics, "Pop scissor clip");
        
        //Is there still clipping rects on the scissor stack?
        if(m_ScissorStack.size() > 0)
        {
            //Set the front scissor rect
            vec2 position = m_ScissorStack.front().first;
            vec2 size = m_ScissorStack.front().first;
            glScissor((GLint)position.x, (GLint)position.y, (GLsizei)size.x, (GLsizei)size.y);
        }
        else
        {
            //Disable the scissor test
            glDisable(GL_SCISSOR_TEST);
        }
    }
    
    void Graphics::BindTexture(Texture* aTexture)
    {
        //Safety check the Texture object
        if(aTexture != nullptr)
        {
            //Get the texture id from the Texture object
            unsigned int textureId = aTexture->GetId();
            
            //Bind the texture id (if its not aleady bound)
            if(textureId != m_BoundTextureId)
            {
                m_BoundTextureId = textureId;
                glBindTexture(GL_TEXTURE_2D, m_BoundTextureId);
                Log(VerbosityLevel_Graphics, "Bind texture: %u", m_BoundTextureId);
            }
        }
        else
        {
            //Unbind the texture
            m_BoundTextureId = 0;
            glBindTexture(GL_TEXTURE_2D, m_BoundTextureId);
        }
    }
    
    unsigned int Graphics::GetBoundTextureId()
    {
        return m_BoundTextureId;
    }
    
    void Graphics::BindVertexArray(unsigned int aVertexArray)
    {
        if(aVertexArray != m_BoundVertexArray)
        {
            m_BoundVertexArray = aVertexArray;
            glBindVertexArray(m_BoundVertexArray);
            Log(VerbosityLevel_Graphics, "Bind vertex array: %u", aVertexArray);
        }
    }
    
    unsigned int Graphics::GetBoundVertexArray()
    {
        return m_BoundVertexArray;
    }
    
    void Graphics::EnableBlending(GLenum aSourceBlending, GLenum aDestinationBlending)
    {
        glBlendFunc(aSourceBlending, aDestinationBlending);
        glEnable(GL_BLEND);
        Log(VerbosityLevel_Graphics, "Blending enabled");
    }
    
    void Graphics::DisableBlending()
    {
        glDisable(GL_BLEND);
        Log(VerbosityLevel_Graphics, "Blending disabled");
    }
    
    bool Graphics::CanUpdate()
    {
        return false;
    }
    
    bool Graphics::CanDraw()
    {
        return false;
    }
}
