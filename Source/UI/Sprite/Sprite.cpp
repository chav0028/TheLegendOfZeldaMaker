//
//  Sprite.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-03-05.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#include "Sprite.h"
#include "../../Graphics/Textures/TextureFrame.h"
#include "../../Services/ServiceLocator.h"
#include "../../Platforms/PlatformLayer.h"

#if DRAW_SPRITE_RECT
    #ifndef DRAW_SPRITE_RECT_COLOR
        #define DRAW_SPRITE_RECT_COLOR Color::RedColor()
    #endif
#endif



namespace GameDev2D
{
    Sprite::Sprite(const string& aFilename, const string& aAtlasKey) : GameObject("Sprite"),
        m_Shader(nullptr),
        m_AnchorPoint(0.0f, 0.0f),
        m_Color(Color::WhiteColor()),
        m_DoesLoop(false),
        m_FrameSpeed(0.0f),
        m_ElapsedTime(0.0),
        m_FrameIndex(0),
        m_TweenAnchorX(),
        m_TweenAnchorY(),
        m_TweenAlpha(),
        m_DidDispatchFinishedEvent(false)
    {
        //Get the passthrough texture shader
        SetShader(ServiceLocator::GetShaderManager()->GetPassthroughTextureShader());
    
        //Create the texture frame object
        AddFrame(new TextureFrame(aFilename, aAtlasKey, "Images/"));

        //Initialize the frame index
        SetFrameIndex(0);
        
        //Reset the model matrix
        ModelMatrixIsDirty(true);
    }
    
    Sprite::Sprite(Shader* aShader, const string& aFilename, const string& aAtlasKey) : GameObject("Sprite"),
        m_Shader(nullptr),
        m_AnchorPoint(0.0f, 0.0f),
        m_Color(Color::WhiteColor()),
        m_DoesLoop(false),
        m_FrameSpeed(0.0f),
        m_ElapsedTime(0.0),
        m_FrameIndex(0),
        m_TweenAnchorX(),
        m_TweenAnchorY(),
        m_TweenAlpha(),
        m_DidDispatchFinishedEvent(false)
    {
        //Set the shader
        SetShader(aShader);
    
        //Create the texture frame object
        AddFrame(new TextureFrame(aFilename, aAtlasKey, "Images/"));
        
        //Initialize the frame index
        SetFrameIndex(0);
        
        //Reset the model matrix
        ModelMatrixIsDirty(true);
    }
    
    Sprite::Sprite(const string& aImageFile, const string& aJsonFile, bool aDoesLoop, float aFrameSpeed) : GameObject("Sprite"),
        m_Shader(nullptr),
        m_AnchorPoint(0.0f, 0.0f),
        m_Color(Color::WhiteColor()),
        m_DoesLoop(aDoesLoop),
        m_FrameSpeed(aFrameSpeed),
        m_ElapsedTime(0.0),
        m_FrameIndex(0),
        m_TweenAnchorX(),
        m_TweenAnchorY(),
        m_TweenAlpha(),
        m_DidDispatchFinishedEvent(false)
    {
        //Get the passthrough texture shader
        SetShader(ServiceLocator::GetShaderManager()->GetPassthroughTextureShader());
        
        //Load the animation frames
        LoadFrames(aImageFile, aJsonFile);
        
        //Initialize the frame index
        SetFrameIndex(0);
        
        //Reset the model matrix
        ModelMatrixIsDirty(true);
    }
    
    Sprite::Sprite(Shader* aShader, const string& aImageFile, const string& aJsonFile, bool aDoesLoop, float aFrameSpeed) : GameObject("Sprite"),
        m_Shader(nullptr),
        m_AnchorPoint(0.0f, 0.0f),
        m_Color(Color::WhiteColor()),
        m_DoesLoop(aDoesLoop),
        m_FrameSpeed(aFrameSpeed),
        m_ElapsedTime(0.0),
        m_FrameIndex(0),
        m_TweenAnchorX(),
        m_TweenAnchorY(),
        m_TweenAlpha(),
        m_DidDispatchFinishedEvent(false)
    {
        //Set the Shader
        SetShader(aShader);
    
        //Load the animation frames
        LoadFrames(aImageFile, aJsonFile);
        
        //Initialize the frame index
        SetFrameIndex(0);
        
        //Reset the model matrix
        ModelMatrixIsDirty(true);
    }
    
    Sprite::~Sprite()
    {
        //Set the shader to null
        m_Shader = nullptr;
    
        //Cycle through and delete all the frames in the sprite
        for(unsigned int i = 0; i < m_Frames.size(); i++)
        {
            delete m_Frames.at(i);
            m_Frames.at(i) = nullptr;
        }
        
        m_Frames.clear();
    }
    
    void Sprite::Update(double aDelta)
    {
        //Update the sprite anchor tween's
        UpdateTween(aDelta, &m_TweenAnchorX, static_cast<GameObject::TweenSetMethod>(&Sprite::SetAnchorX));
        UpdateTween(aDelta, &m_TweenAnchorY, static_cast<GameObject::TweenSetMethod>(&Sprite::SetAnchorY));
        UpdateTween(aDelta, &m_TweenAlpha, static_cast<GameObject::TweenSetMethod>(&Sprite::SetAlpha));
    
        //Is the Sprite animated
        if(IsAnimated() == true)
        {
            //Increment the elapsed time.
            m_ElapsedTime += aDelta;

            //Calculate the new frame index.
            int lastFrameIndex = m_FrameIndex;
            int frameIndex = (int)(m_ElapsedTime / (1.0 / m_FrameSpeed));

            //If the animation loops, we need to see if it is ready wrap around to the start.
            if(m_DoesLoop == true)
            {
                frameIndex = frameIndex % GetFrameCount();
            }

            //Is the animation complete?
            if(frameIndex >= (int)GetFrameCount())
            {
                //Dispatch an event saying the sprite did finish animating
                if(m_DidDispatchFinishedEvent == false)
                {
                    DispatchEvent(new Event(SPRITE_DID_FINISH_ANIMATING, this, VerbosityLevel_UI));
                    m_DidDispatchFinishedEvent = true;
                }
            }

            //If the frame index has changed, set the new frame index.
            if(frameIndex != lastFrameIndex)
            {
                SetFrameIndex(frameIndex);
            }
        }
        
        //Update the GameObject, this will update any children
        GameObject::Update(aDelta);
    }
    
    void Sprite::Draw()
    {
        //Safety check the texture frame
        if(m_Frames.size() == 0)
        {
            Error(false, "Failed to draw sprite, there aren't any frames to draw");
            return;
        }
        
        //Safety check the shader
        if(m_Shader == nullptr)
        {
            Error(false, "Failed to draw sprite, the shader is null");
            return;
        }
     
        //Check to see if the model matrix is dirty
        if(IsModelMatrixDirty() == true)
        {
            ResetModelMatrix();
        }
        
        //Draw the current frame
        m_Frames.at(m_FrameIndex)->Draw(m_ModelMatrix);
        
        #if DRAW_SPRITE_RECT
        Rect rect(GetWorldPosition().x, GetWorldPosition().y, GetWidth(), GetHeight());
        rect.SetIsFilled(false);
        rect.SetColor(DRAW_SPRITE_RECT_COLOR);
        rect.SetAnchorPoint(GetAnchorPoint());
        rect.SetLocalAngle(GetWorldAngle());
        rect.SetLocalScale(GetWorldScale());
        rect.Draw();
        #endif
        
        //Draw the GameObject, this will draw any children
        GameObject::Draw();
    }
    
    void Sprite::Reset()
    {
        //Reset the elapsed time and frame index
        m_ElapsedTime = 0.0;
        SetFrameIndex(0);
        m_DidDispatchFinishedEvent = false;
        
        //Reset the GameObject, this will reset any children
        GameObject::Reset();
    }
    
    void Sprite::SetShader(Shader* aShader)
    {
        if(aShader != nullptr)
        {
            //Set the new shader
            m_Shader = aShader;
            
            //Cycle through all the frames and set the shader
            for(unsigned int i = 0; i < m_Frames.size(); i++)
            {
                m_Frames.at(i)->SetShader(m_Shader);
            }
        }
    }
    
    vec2 Sprite::GetSize()
    {
        return vec2(GetWidth(), GetHeight());
    }
    
    float Sprite::GetWidth()
    {
        if(m_Frames.size() > 0)
        {
            return m_Frames.at(m_FrameIndex)->GetSourceFrame().size.x;
        }
        return 0.0f;
    }
    
    float Sprite::GetHeight()
    {
        if(m_Frames.size() > 0)
        {
            return m_Frames.at(m_FrameIndex)->GetSourceFrame().size.y;
        }
        return 0.0f;
    }
    
    void Sprite::SetAnchorX(float aAnchorX, double aDuration, EasingFunction aEasingFunction, bool aReverse, int aRepeatCount)
    {
        if(aDuration == 0.0)
        {
            m_AnchorPoint.x = fminf(fmaxf(aAnchorX, 0.0f), 1.0f);
            ModelMatrixIsDirty();
        }
        else
        {
            if(m_AnchorPoint.x != aAnchorX)
            {
                m_TweenAnchorX.Set(m_AnchorPoint.x, aAnchorX, aDuration, aEasingFunction, aReverse, aRepeatCount);
            }
        }
    }
    
    void Sprite::SetAnchorY(float aAnchorY, double aDuration, EasingFunction aEasingFunction, bool aReverse, int aRepeatCount)
    {
        if(aDuration == 0.0)
        {
            m_AnchorPoint.y = fminf(fmaxf(aAnchorY, 0.0f), 1.0f);
            ModelMatrixIsDirty();
        }
        else
        {
            if(m_AnchorPoint.y != aAnchorY)
            {
                m_TweenAnchorY.Set(m_AnchorPoint.y, aAnchorY, aDuration, aEasingFunction, aReverse, aRepeatCount);
            }
        }
    }
    
    void Sprite::SetAnchorPoint(float aAnchorX, float aAnchorY, double aDuration, EasingFunction aEasingFunction, bool aReverse, int aRepeatCount)
    {
        SetAnchorX(aAnchorX, aDuration, aEasingFunction, aReverse, aRepeatCount);
        SetAnchorY(aAnchorY, aDuration, aEasingFunction, aReverse, aRepeatCount);
    }
    
    void Sprite::SetAnchorPoint(vec2 aAnchorPoint, double aDuration, EasingFunction aEasingFunction, bool aReverse, int aRepeatCount)
    {
        SetAnchorPoint(aAnchorPoint.x, aAnchorPoint.y, aDuration, aEasingFunction, aReverse, aRepeatCount);
    }
    
    vec2 Sprite::GetAnchorPoint()
    {
        return m_AnchorPoint;
    }
    
    void Sprite::SetAlpha(float aAlpha, double aDuration, EasingFunction aEasingFunction, bool aReverse, int aRepeatCount)
    {
        if(aDuration == 0.0)
        {
            //Set the alpha
            m_Color.SetAlpha(aAlpha);
            
            //And update the color for all the frames
            for(unsigned int i = 0; i < m_Frames.size(); i++)
            {
                m_Frames.at(i)->SetColor(m_Color);
            }
        }
        else
        {
            if(m_Color.Alpha() != aAlpha)
            {
                m_TweenAlpha.Set(m_Color.Alpha(), aAlpha, aDuration, aEasingFunction, aReverse, aRepeatCount);
            }
        }
    }
    
    float Sprite::GetAlpha()
    {
        return m_Color.Alpha();
    }
    
    void Sprite::AddFrame(TextureFrame* aTextureFrame)
    {
        assert(aTextureFrame != nullptr);
        if(aTextureFrame != nullptr)
        {
            //Add a texture frame to the frames vector
            m_Frames.push_back(aTextureFrame);
            
            //Set the texture frame's shader and color
            if(m_Shader != nullptr)
            {
                aTextureFrame->SetShader(m_Shader);
                aTextureFrame->SetColor(m_Color);
            }
        }
    }
    
    void Sprite::AddFrame(const string& aFilename, const string& aAtlasKey)
    {
        AddFrame(new TextureFrame(aFilename, aAtlasKey, "Images/"));
    }
    
    bool Sprite::IsAnimated()
    {
        return m_Frames.size() > 1;
    }

    unsigned long Sprite::GetFrameCount()
    {
        return m_Frames.size();
    }

    int Sprite::GetFrameIndex()
    {
        return m_FrameIndex;
    }
    
    void Sprite::SetFrameIndex(unsigned int aFrameIndex)
    {
        //Safety check the frame index.
        if(aFrameIndex < GetFrameCount())
        {
            //Set the frames index.
            m_FrameIndex = aFrameIndex;

            //Is the sprite animated
            if(IsAnimated() == true)
            {
                //Set the elapsed time
                m_ElapsedTime = (double)m_FrameIndex * (1.0 / m_FrameSpeed);
                
                //Flag the model matrix as dirty
                ModelMatrixIsDirty(true);
                
                //Reset the completion flag
                m_DidDispatchFinishedEvent = false;
            }
        }
    }

    bool Sprite::DoesLoop()
    {
        return m_DoesLoop;
    }
    
    void Sprite::SetDoesLoop(bool aDoesLoop)
    {
        m_DoesLoop = aDoesLoop;
    }
    
    float Sprite::GetFrameSpeed()
    {
        return m_FrameSpeed;
    }
    
    void Sprite::SetFrameSpeed(float aFrameSpeed)
    {
        m_FrameSpeed = aFrameSpeed;
    }
    
    void Sprite::CancelTweens()
    {
        m_TweenAnchorX.Cancel();
        m_TweenAnchorY.Cancel();
        m_TweenAlpha.Cancel();
        
        GameObject::CancelTweens();
    }
    
    void Sprite::ResetModelMatrix()
    {
        //Translate the anchor, then translate the position
        mat4 anchor = translate(mat4(1.0f), vec3(-GetWidth() * GetWorldScale().x * m_AnchorPoint.x, -GetHeight() * GetWorldScale().y * m_AnchorPoint.y, 0.0f));
        mat4 viewTranslate = translate(anchor, vec3(GetWorldPosition().x, GetWorldPosition().y, 0.0f));

        //Calculate the rotation based on the anchor point
        mat4 halfTranslate1 = translate(viewTranslate, vec3(GetWidth() * GetWorldScale().x * m_AnchorPoint.x, GetHeight() * GetWorldScale().y * m_AnchorPoint.y, 0.0f));
        mat4 viewRotation = rotate(halfTranslate1, GetWorldAngle(), vec3(0.0f, 0.0f, 1.0f));
        mat4 halfTranslate2 = translate(viewRotation, vec3(-GetWidth() * GetWorldScale().x * m_AnchorPoint.x, -GetHeight() * GetWorldScale().y * m_AnchorPoint.y, 0.0f));
        
        //Lastly the scale
        m_ModelMatrix = scale(halfTranslate2, vec3(GetWorldScale().x, GetWorldScale().y, 0.0f));
        
        //Call the GameObject's ResetModelMatrix() method
        GameObject::ResetModelMatrix();
    }
    
    void Sprite::LoadFrames(const string& aImageFile, const string& aJsonFile)
    {
        //Append the filename to the directory
        string jsonFile = aJsonFile;
    
        //Was .json appended to the filename? If it was, remove it
        size_t found = jsonFile.find(".json");
        if(found != std::string::npos)
        {
            jsonFile.erase(found, 5);
        }
    
        //Get the json path
        string jsonPath = ServiceLocator::GetPlatformLayer()->GetPathForResourceInDirectory(jsonFile.c_str(), "json", "Images");
        
        //Does the json file exist, if the assert is hit it means the json file doesn't exist
        bool doesExist = ServiceLocator::GetPlatformLayer()->DoesFileExistAtPath(jsonPath);
        assert(doesExist == true);
        
        //Does the file exist, load the json data
        if(doesExist == true)
        {
            //Open the input stream
            std::ifstream inputStream;
            inputStream.open(jsonPath, std::ifstream::in);
            
            //Parse the json file
            Json::Value root;
            Json::Reader reader;
            if(reader.parse(inputStream, root, false) == true)
            {
                //Get the frames data
                const Json::Value frames = root["frames"];
                
                //Cycle through and setup each frame in the animation
                for(unsigned int index = 0; index < frames.size(); index++)
                {
                    //Get the atlas key
                    Json::Value atlasKey = frames[index]["filename"];

                    //Add the frame
                    AddFrame(new TextureFrame(aImageFile, atlasKey.asString(), "Images"));
                }
            }
            
            //Close the input stream
            inputStream.close();
        }
    }
}