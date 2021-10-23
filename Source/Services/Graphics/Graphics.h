//
//  Graphics.h
//  GameDev2D
//
//  Created by Bradley Flood on 2013-07-15.
//  Copyright (c) 2013 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__GDGraphics__
#define __GameDev2D__GDGraphics__

#include "../GameService.h"
#include "../../Graphics/OpenGL.h"
#include "../../Graphics/Core/Shader.h"
#include "../../Graphics/Core/Camera.h"
#include "../../Graphics/Core/RenderTarget.h"
#include "../../Graphics/Core/Color.h"
#include "../../Graphics/Textures/Texture.h"
#include "../../Graphics/Textures/TextureFrame.h"
#include "../../Graphics/Primitives/Primitives.h"
#include "matrix_transform.hpp"


using namespace glm;

namespace GameDev2D
{
    //The Graphics game service is responsible for clearing the back buffer, managing
    //the active RenderTarget and Camera, resizing the viewport and backbuffer. Managing
    //a clipping rect stack and the currently bound texture and vertex array object.
    class Graphics : public GameService
    {
    public:
        Graphics();
        ~Graphics();
        
        //Clears the currently bound RenderTarget's back buffer
        void Clear();
        
        //Set the clear color
        void SetClearColor(Color color);
        Color GetClearColor();
        
        //Returns the back buffer's width and height
        int GetWidth();
        int GetHeight();
        
        //Returns the active camera's projection and view matrices
        mat4 GetProjectionMatrix();
        mat4 GetViewMatrix();
        
        //Used to resize the back buffer size
        void Resize(int width, int height);
        
        //Resets the viewport to the desired size
        virtual void ResetViewport();
        
        //Sets a new size for the viewport and reset's the active
        //camera's projection and view matrices
        void SetViewportSize(int width, int height);
        
        //Returns the viewport's width and height
		int GetViewportWidth();
        int GetViewportHeight();
        
        //Returns the active RenderTarget
        RenderTarget* GetActiveRenderTarget();
        
        //Sets the active RenderTarget
        void SetActiveRenderTarget(RenderTarget* renderTarget);
        
        //Returns the active Camera
        Camera* GetActiveCamera();
        
        //Sets the actice Camera
        void SetActiveCamera(Camera* camera);
        
        //Set the default Camera
        void SetDefaultCamera();
    
        //Push a clipping rect onto the clipping stack
        void PushScissorClip(float x, float y, float width, float height);
        
        //Pops the last clipping rect off the clipping stack
        void PopScissorClip();
    
        //Bind the open gl texture id associated with the Texture object
        void BindTexture(Texture* texture);
        
        //Returns the currently bound texture id
        unsigned int GetBoundTextureId();
        
        //Binds an open gl vertex array
        void BindVertexArray(unsigned int vertexArray);
        
        //Returns the currently bound vertex array
        unsigned int GetBoundVertexArray();
        
        //Enables and disables open gl alpha blending
        void EnableBlending(GLenum sourceBlending = GL_SRC_ALPHA, GLenum destinationBlending = GL_ONE_MINUS_SRC_ALPHA);
        void DisableBlending();

        //Used to determine if the Graphics should be updated and drawn
        bool CanUpdate();
        bool CanDraw();

    private:
        //RenderTarget members
        RenderTarget* m_ActiveRenderTarget;
        
        #if TARGET_OS_IPHONE
        RenderTarget* m_MainRenderTarget;
        #else
        int m_Width;
        int m_Height;
        Color m_ClearColor;
        #endif
        
        //Camera members
        Camera* m_DefaultCamera;
        Camera* m_ActiveCamera;
        
        //The viewport's width and height
        int m_ViewportWidth;
        int m_ViewportHeight;
        
        //Members for the bound texture and vertex array objects
        unsigned int m_BoundTextureId;
        unsigned int m_BoundVertexArray;
        
        //Scissor stack to keep track of multiple clipping rects
        vector<pair<vec2, vec2>> m_ScissorStack;
    };
}

#endif /* defined(__GameDev2D__GDGraphics__) */
