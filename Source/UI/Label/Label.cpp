//
//  Font.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2013-08-13.
//  Copyright (c) 2013 Algonquin College. All rights reserved.
//

#include "Label.h"
#include "../../Graphics/Textures/TextureFrame.h"
#include "../../Services/ServiceLocator.h"
#include "../../Services/Graphics/Graphics.h"
#include "../../Animation/Tween.h"


#if DRAW_LABEL_RECT
    #ifndef DRAW_LABEL_RECT_COLOR
        #define DRAW_LABEL_RECT_COLOR Color::RedColor()
    #endif
#endif


using namespace std;

namespace GameDev2D
{
    Label::Label(const string& aBitmapFont) : GameObject("Label"),
        m_Font(nullptr),
        m_Shader(nullptr),
        m_RenderTarget(nullptr),
        m_Justification(JustifyLeft),
        m_Text(""),
        m_Size(0.0f, 0.0f),
        m_Color(Color::WhiteColor()),
        m_AnchorPoint(0.0f, 0.0f),
        m_CharacterAngle(0.0f),
        m_CharacterSpacing(0.0f),
        m_TweenAnchorX(),
        m_TweenAnchorY(),
        m_TweenAlpha(),
        m_TweenCharacterAngle(),
        m_TweenCharacterSpacing()
    {
        //Load the font
        m_Font = ServiceLocator::GetFontManager()->AddBitmapFont(aBitmapFont);
        
        //Set the label height
        if(m_Font != nullptr)
        {
            m_Size.y = (float)m_Font->GetLineHeight();
        }
        
        //Get a texture passthrough shader
        m_Shader = ServiceLocator::GetShaderManager()->GetPassthroughTextureShader();
    }
    
    Label::Label(Shader* aShader, const string& aBitmapFont) : GameObject("Label"),
        m_Font(nullptr),
        m_Shader(nullptr),
        m_RenderTarget(nullptr),
        m_Justification(JustifyLeft),
        m_Text(""),
        m_Size(0.0f, 0.0f),
        m_Color(Color::WhiteColor()),
        m_AnchorPoint(0.0f, 0.0f),
        m_CharacterAngle(0.0f),
        m_CharacterSpacing(0.0f),
        m_TweenAnchorX(),
        m_TweenAnchorY(),
        m_TweenAlpha(),
        m_TweenCharacterAngle(),
        m_TweenCharacterSpacing()
    {
        //Load the font
        m_Font = ServiceLocator::GetFontManager()->AddBitmapFont(aBitmapFont);
        
        //Set the label height
        if(m_Font != nullptr)
        {
            m_Size.y = (float)m_Font->GetLineHeight();
        }
        
        //Set the shader
        SetShader(aShader);
    }
    
    Label::Label(const string& aTrueTypeFont, const string& aExtension, const unsigned int aFontSize) : GameObject("Label"),
        m_Font(nullptr),
        m_Shader(nullptr),
        m_RenderTarget(nullptr),
        m_Justification(JustifyLeft),
        m_Text(""),
        m_Size(0.0f, 0.0f),
        m_Color(Color::WhiteColor()),
        m_AnchorPoint(0.0f, 0.0f),
        m_CharacterAngle(0.0f),
        m_CharacterSpacing(0.0f),
        m_TweenAnchorX(),
        m_TweenAnchorY(),
        m_TweenAlpha(),
        m_TweenCharacterAngle(),
        m_TweenCharacterSpacing()
    {
        //Load the font
        m_Font = ServiceLocator::GetFontManager()->AddTrueTypeFont(aTrueTypeFont, aExtension, aFontSize, CHARACTER_SET_ALPHA_NUMERIC_EXT);
        
        //Set the label height
        if(m_Font != nullptr)
        {
            m_Size.y = (float)m_Font->GetLineHeight();
        }
        
        //Get a texture passthrough shader
        m_Shader = ServiceLocator::GetShaderManager()->GetPassthroughTextureShader();
    }
    
    Label::Label(Shader* aShader, const string& aTrueTypeFont, const string& aExtension, const unsigned int aFontSize) : GameObject("Label"),
        m_Font(nullptr),
        m_Shader(nullptr),
        m_RenderTarget(nullptr),
        m_Justification(JustifyLeft),
        m_Text(""),
        m_Size(0.0f, 0.0f),
        m_Color(Color::WhiteColor()),
        m_AnchorPoint(0.0f, 0.0f),
        m_CharacterAngle(0.0f),
        m_CharacterSpacing(0.0f),
        m_TweenAnchorX(),
        m_TweenAnchorY(),
        m_TweenAlpha(),
        m_TweenCharacterAngle(),
        m_TweenCharacterSpacing()
    {
        //Load the font
        m_Font = ServiceLocator::GetFontManager()->AddTrueTypeFont(aTrueTypeFont, aExtension, aFontSize, CHARACTER_SET_ALPHA_NUMERIC_EXT);
        
        //Set the label height
        if(m_Font != nullptr)
        {
            m_Size.y = (float)m_Font->GetLineHeight();
        }
        
        //Set the shader
        SetShader(aShader);
    }
    
    Label::~Label()
    {
        //Set the shader to null
        m_Shader = nullptr;

        //Remove the font
        if(m_Font != nullptr)
        {
            ServiceLocator::GetFontManager()->RemoveFont(m_Font);
            m_Font = nullptr;
        }

        //Delete the render target
        SafeDelete(m_RenderTarget);
    }
    
    void Label::Update(double aDelta)
    {
        //Update all the tweens
        UpdateTween(aDelta, &m_TweenAnchorX, static_cast<GameObject::TweenSetMethod>(&Label::SetAnchorX));
        UpdateTween(aDelta, &m_TweenAnchorY, static_cast<GameObject::TweenSetMethod>(&Label::SetAnchorY));
        UpdateTween(aDelta, &m_TweenAlpha, static_cast<GameObject::TweenSetMethod>(&Label::SetAlpha));
        UpdateTween(aDelta, &m_TweenCharacterAngle, static_cast<GameObject::TweenSetMethod>(&Label::SetCharacterAngle));
        UpdateTween(aDelta, &m_TweenCharacterSpacing, static_cast<GameObject::TweenSetMethod>(&Label::SetCharacterSpacing));
    
        //Update the GameObject, which updates all the children
        GameObject::Update(aDelta);
    }
    
    void Label::Draw()
    {
        //If this assert is hit, it means there isn't a Shader set
        assert(m_Shader != nullptr);
    
        //Is the model matrix dirty?
        if(IsModelMatrixDirty() == true)
        {
            ResetModelMatrix();
        }
    
        //Is the render target null
        if(m_RenderTarget != nullptr)
        {
            //Draw the render target
            m_RenderTarget->GetTextureFrame()->Draw(m_ModelMatrix);
        }
        else
        {
            //Draw the text with out the render target
            DrawText();
        }
        
        //Draw a debug label rect
        #if DRAW_LABEL_RECT
        Rect rect(GetWorldPosition().x, GetWorldPosition().y, GetWidth(), GetHeight());
        rect.SetIsFilled(false);
        rect.SetColor(DRAW_LABEL_RECT_COLOR);
        rect.SetAnchorPoint(GetAnchorPoint());
        rect.SetLocalAngle(GetWorldAngle());
        rect.SetLocalScale(GetWorldScale());
        rect.Draw();
        #endif
        
        //Draw the GameObject, which draws all the children
        GameObject::Draw();
    }
    
    void Label::SetText(const string& aText)
    {
        //We can't modify the render target while it is bound
        if(m_RenderTarget != nullptr && m_RenderTarget->IsBound() == true)
        {
            Error(false, "Failed to set Label text, the render target is already bound");
            return;
        }
        
        //Is the text the exact same, if so return
        if(m_Text == aText)
        {
            return;
        }
    
        //Set the text
        m_Text = string(aText);
        
        //Resize the render target
        ResizeRenderTarget();
    }
    
    string Label::GetText()
    {
        return m_Text;
    }
    
    void Label::SetShader(Shader* aShader)
    {
        if(aShader != nullptr)
        {
            //Set the new shader
            m_Shader = aShader;
            
            //Set the render target's shader
            if(m_RenderTarget != nullptr)
            {
                m_RenderTarget->GetTextureFrame()->SetShader(m_Shader);
            }
        }
    }
    
    vec2 Label::GetSize()
    {
        return m_Size;
    }
    
    float Label::GetWidth()
    {
        return m_Size.x;
    }
    
    float Label::GetHeight()
    {
        return m_Size.y;
    }

    void Label::SetAnchorX(float aAnchorX, double aDuration, EasingFunction aEasingFunction, bool aReverse, int aRepeatCount)
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
    
    void Label::SetAnchorY(float aAnchorY, double aDuration, EasingFunction aEasingFunction, bool aReverse, int aRepeatCount)
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
    
    void Label::SetAnchorPoint(float aAnchorX, float aAnchorY, double aDuration, EasingFunction aEasingFunction, bool aReverse, int aRepeatCount)
    {
        SetAnchorX(aAnchorX, aDuration, aEasingFunction, aReverse, aRepeatCount);
        SetAnchorY(aAnchorY, aDuration, aEasingFunction, aReverse, aRepeatCount);
    }
    
    void Label::SetAnchorPoint(vec2 aAnchorPoint, double aDuration, EasingFunction aEasingFunction, bool aReverse, int aRepeatCount)
    {
        SetAnchorPoint(aAnchorPoint.x, aAnchorPoint.y, aDuration, aEasingFunction, aReverse, aRepeatCount);
    }
    
    vec2 Label::GetAnchorPoint()
    {
        return m_AnchorPoint;
    }
    
    void Label::SetColor(Color aColor)
    {
        m_Color = aColor;
        
        //Set the render targets color
        if(m_RenderTarget != nullptr)
        {
            m_RenderTarget->GetTextureFrame()->SetColor(m_Color);
        }
    }
    
    Color Label::GetColor()
    {
        return m_Color;
    }
    
    void Label::SetAlpha(float aAlpha, double aDuration, EasingFunction aEasingFunction, bool aReverse, int aRepeatCount)
    {
        if(aDuration == 0.0)
        {
            //Set the alpha
            m_Color.SetAlpha(aAlpha);
            
            //Set the render target's alpha
            if(m_RenderTarget != nullptr)
            {
                m_RenderTarget->GetTextureFrame()->SetColor(m_Color);
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
    
    float Label::GetAlpha()
    {
        return m_Color.Alpha();
    }
    
    void Label::SetCharacterAngle(float aCharacterAngle, double aDuration, EasingFunction aEasingFunction, bool aReverse, int aRepeatCount)
    {
        if(aDuration == 0.0)
        {
            m_CharacterAngle = aCharacterAngle;
            ResetRenderTarget();
        }
        else
        {
            if(m_CharacterAngle != aCharacterAngle)
            {
                m_TweenCharacterAngle.Set(m_CharacterAngle, aCharacterAngle, aDuration, aEasingFunction, aReverse, aRepeatCount);
            }
        }
    }
    
    float Label::GetCharacterAngle()
    {
        return m_CharacterAngle;
    }
    
    void Label::SetCharacterSpacing(float aCharacterSpacing, double aDuration, EasingFunction aEasingFunction, bool aReverse, int aRepeatCount)
    {
        if(aDuration == 0.0)
        {
            m_CharacterSpacing = aCharacterSpacing;
            ResizeRenderTarget();
        }
        else
        {
            if(m_CharacterSpacing != aCharacterSpacing)
            {
                m_TweenCharacterSpacing.Set(m_CharacterSpacing, aCharacterSpacing, aDuration, aEasingFunction, aReverse, aRepeatCount);
            }
        }
    }
    
    float Label::GetCharacterSpacing()
    {
        return m_CharacterSpacing;
    }
    
    void Label::SetJustification(LabelJustification aJustification)
    {
        //Safety check that the justification isn't already set
        if(aJustification != m_Justification)
        {
            //Set the justification
            m_Justification = aJustification;
            
            //Reset the render target
            if(m_Text.length() > 0)
            {
                ResetRenderTarget();
            }
        }
    }
    
    LabelJustification Label::GetJustification()
    {
        return m_Justification;
    }
    
    unsigned long Label::GetNumberOfLines()
    {
        return m_LineWidth.size();
    }
    
    void Label::CancelTweens()
    {
        m_TweenAnchorX.Cancel();
        m_TweenAnchorY.Cancel();
        m_TweenAlpha.Cancel();
        m_TweenCharacterAngle.Cancel();
        m_TweenCharacterSpacing.Cancel();
        
        GameObject::CancelTweens();
    }
    
    void Label::ResetModelMatrix()
    {
        //Translate the anchor, then translate the position
        mat4 anchor = translate(mat4(1.0f), vec3(-GetWidth() * GetWorldScale().x * GetAnchorPoint().x, -GetHeight() * GetWorldScale().y * GetAnchorPoint().y, 0.0f));
        mat4 viewTranslate = translate(anchor, vec3(GetWorldPosition().x, GetWorldPosition().y, 0.0f));

        //Calculate the rotation based on the anchor point
        mat4 halfTranslate1 = translate(viewTranslate, vec3(GetWidth() * GetWorldScale().x * GetAnchorPoint().x, GetHeight() * GetWorldScale().y * GetAnchorPoint().y, 0.0f));
        mat4 viewRotation = rotate(halfTranslate1, GetWorldAngle(), vec3(0.0f, 0.0f, 1.0f));
        mat4 halfTranslate2 = translate(viewRotation, vec3(-GetWidth() * GetWorldScale().x * GetAnchorPoint().x, -GetHeight() * GetWorldScale().y * GetAnchorPoint().y, 0.0f));
        
        //Lastly the scale
        m_ModelMatrix = scale(halfTranslate2, vec3(GetWorldScale().x, GetWorldScale().y, 0.0f));
        
        //Reset the model matrix
        GameObject::ResetModelMatrix();
    }
    
    void Label::ResizeRenderTarget()
    {
        //Calculate the size of the Label
        m_Size = CalculateSize();

        //Delete the previous render target
        SafeDelete(m_RenderTarget);
        
        //Create a new render target
        if(m_Text.length() > 1)
        {
            m_RenderTarget = new RenderTarget((int)m_Size.x, (int)m_Size.y);
            m_RenderTarget->CreateFrameBuffer();

            //Reset the render target
            ResetRenderTarget();
        }
        
        //Flag the model matrix as dirty
        ModelMatrixIsDirty();
    }
    
    void Label::ResetRenderTarget()
    {
        RenderTarget* renderTarget = nullptr;
    
        //Safety check the render target
        if(m_RenderTarget != nullptr)
        {
            //Set the render target's shader and color
            m_RenderTarget->GetTextureFrame()->SetShader(m_Shader);
            m_RenderTarget->GetTextureFrame()->SetColor(m_Color);
        
            //Cache the graphics active render target
            renderTarget = ServiceLocator::GetGraphics()->GetActiveRenderTarget();
            
            //Set the Label's render target as the active render target
            ServiceLocator::GetGraphics()->SetActiveRenderTarget(m_RenderTarget);
            ServiceLocator::GetGraphics()->Clear();
        }
        
        //Draw the text to the render target
        DrawText();

        //Set the original graphic's render target back
        ServiceLocator::GetGraphics()->SetActiveRenderTarget(renderTarget);
    }
    
    vec2 Label::CalculateSize()
    {
        //Safety check that the font pointer is null
        if(m_Font == nullptr)
        {
            Error(false, "Unable to calculate the size of the Label, the font pointer is null");
            return vec2(0.0f, 0.0f);
        }
    
        //Used to track the max width and height
        float x = 0.0f;
        vec2 maxSize(0.0f, m_Font->GetLineHeight());
        
        //Clear the line width's vector
        m_LineWidth.clear();
    
        //Cycle through all the characters in the text string
        for(unsigned int i = 0; i < m_Text.length(); i++)
        {
            //Did we reach a new line?
            if(m_Text.at(i) == '\n')
            {
                //Add the line width to the line widths vector
                m_LineWidth.push_back(x);
                
                //Calculate the max width and increment the max height
                maxSize.x = fmaxf(x, maxSize.x);
                maxSize.y += m_Font->GetLineHeight();
                
                //Set x back to zero
                x = 0.0f;
                continue;
            }
            
            //Advance the x, by the x-advance of the character
            x += m_Font->GetAdvanceXForCharacter(m_Text.at(i)) + GetCharacterSpacing();
        }
        
        //We are done, add the line width to the line widths vector
        m_LineWidth.push_back(x);
        
        //Calculate the max width
        maxSize.x = fmaxf(x, maxSize.x);
        
        //And return the max width and height
        return vec2(maxSize.x, maxSize.y);
    }
    
    void Label::DrawText()
    {
        //Safety check that the font pointer is null
        if(m_Font == nullptr)
        {
            Error(false, "Unable to draw the text of the Label, the font pointer is null");
            return;
        }
        
        //Is there any text to render?
        if(m_Text.length() == 0)
        {
            return;
        }
        
        //calculate the baseline and origin for the label
        unsigned int baseline = m_Font->GetLineHeight() - m_Font->GetBaseLine();
        vec2 origin(0.0f, baseline + (m_Font->GetLineHeight() * (GetNumberOfLines() - 1)));
        int lineIndex = 0;
        
        //What justification are we dealing with
        if(m_Justification == JustifyLeft)
        {
            origin.x = 0.0f;
        }
        else if(m_Justification == JustifyCenter)
        {
            origin.x = (GetSize().x - m_LineWidth.at(lineIndex)) / 2.0f;
        }
        else if(m_Justification == JustifyRight)
        {
            origin.x = GetSize().x - m_LineWidth.at(lineIndex);
        }
        
        //Cycle through the characters in the text label
        for(unsigned int i = 0; i < m_Text.length(); i++)
        {
            //Did we reach a new line?
            if(m_Text.at(i) == '\n')
            {
                //Increment the line index
                lineIndex++;
            
                //Calculate the line's origin based on the justification
                if(m_Justification == JustifyLeft)
                {
                    origin.x = 0.0f;
                }
                else if(m_Justification == JustifyCenter)
                {
                    origin.x = (GetSize().x - m_LineWidth.at(lineIndex)) / 2.0f;
                }
                else if(m_Justification == JustifyRight)
                {
                    origin.x = GetSize().x - m_LineWidth.at(lineIndex);
                }
            
                //Set the y line origin based on the line height of the font
                origin.y -= m_Font->GetLineHeight();
                continue;
            }
            
            //Get the texture frame for the character
            TextureFrame* textureFrame = m_Font->GetTextureFrameForCharacter(m_Text.at(i));
            
            //Safety check the texture frame
            if(textureFrame != nullptr)
            {
                //Calculate the character position based on the x and y bearing
                vec2 charPosition = origin;
                charPosition.x += m_Font->GetBearingXForCharacter(m_Text.at(i));
                charPosition.y += m_Font->GetBearingYForCharacter(m_Text.at(i)) - m_Font->GetSourceFrameForCharacter(m_Text.at(i)).size.y;

                //Draw the character
                DrawCharacter(textureFrame, charPosition);
                
                //Increment the origin
                origin.x += m_Font->GetAdvanceXForCharacter(m_Text.at(i)) + GetCharacterSpacing();
            }
        }
    }
    
    void Label::DrawCharacter(TextureFrame* aTextureFrame, vec2 aPosition)
    {    
        //Translate the anchor, then translate the position
        mat4 viewTranslate = translate(mat4(1.0f), vec3(aPosition.x, aPosition.y, 0.0f));

        //Get the size of the character
        vec2 size = vec2(aTextureFrame->GetSourceFrame().size.x, aTextureFrame->GetSourceFrame().size.y);

        //Calculate the model matrix
        mat4 halfTranslate = translate(viewTranslate, vec3((size.x / 2.0f), (size.y / 2.0f), 0.0f));
        mat4 viewRotation = rotate(halfTranslate, m_CharacterAngle, vec3(0.0f, 0.0f, 1.0f));
        mat4 modelMatrix = translate(viewRotation, vec3(-(size.x / 2.0f), -(size.y / 2.0f), 0.0f));

        //Render the character as white
        Color color = Color::WhiteColor();

        //Unless the text has a length of 1, in which case we aren't rendering with the render target
        if(m_Text.length() <= 1)
        {
            modelMatrix = m_ModelMatrix * modelMatrix;
            color = GetColor();
        }
        
        //Set the texture frame's shader and color
        aTextureFrame->SetShader(m_Shader);
        aTextureFrame->SetColor(color);

        //Draw the character
        aTextureFrame->Draw(modelMatrix);
    }
}

