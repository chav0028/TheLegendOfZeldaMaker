//
//  Label.h
//  GameDev2D
//
//  Created by Bradley Flood on 2013-08-13.
//  Copyright (c) 2013 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__Label__
#define __GameDev2D__Label__

#include "../../Core/GameObject.h"
#include "../../Graphics/Fonts/TrueTypeFont.h"
#include "../../Graphics/Fonts/BitmapFont.h"
#include "../../Graphics/Core/Color.h"
#include "../../Animation/Tween.h"
#include "../../Animation/Easing.h"


using namespace glm;
using namespace std;

namespace GameDev2D
{
    //Forward declarations
    class Shader;
    class RenderTarget;
    class TextureFrame;
    
    //Label text can be left, center and right justified
    enum LabelJustification
    {
        JustifyLeft = 0,
        JustifyCenter,
        JustifyRight
    };
    
    //The Label class inherits from the GameObject class and can create a Label using either
    //a BitmapFont or a TrueTypeFont. The anchor points, alpha, character angle and character
    //spacing can all be animated, with easing applied. Label's created with Bitmap fonts must
    //use the 'Glyph Designer' software, see the BitmapFont header file for more info. Internally
    //the Label renders the text to a render target to ensure there is only 1 draw call instead of several
    class Label : public GameObject
    {
    public:
        //Contructs a Label using a BitmapFont, the font size can NOT be specified
        Label(const string& bitmapFont);
        
        //Contructs a Label using a BitmapFont, the font size can NOT be specified. In addition the
        //shader used can be specified, the Sprite takes ownership of the Shader and will remove it
        Label(Shader* shader, const string& bitmapFont);
        
        //Contructs a Label using a TrueTypeFont, the font size must be specified
        Label(const string& trueTypeFont, const string& extension, const unsigned int fontSize);
        
        //Contructs a Label using a TrueTypeFont, the font size must be specified. In addition the
        //shader used can be specified, the Sprite takes ownership of the Shader and will remove it
        Label(Shader* shader, const string& trueTypeFont, const string& extension, const unsigned int fontSize);
        ~Label();
        
        //Updates the Label
        void Update(double delta);
        
        //Draws the Label
        void Draw();
        
        //Sets the Shader to render the Label with
        void SetShader(Shader* shader);
        
        //Sets the text that the Label will display
        void SetText(const string& text);
        
        //Returns the text the Label is displaying
        string GetText();
        
        //Returns the size of the label
        vec2 GetSize();
        float GetWidth();
        float GetHeight();
        
        //Sets the anchor point for the Label, can be set to animate over a duration, an easing function can be applied, can be revered and repeated
        void SetAnchorX(float anchorX, double duration = 0.0, EasingFunction easingFunction = Linear::Interpolation, bool reverse = false, int repeatCount = 0);
        void SetAnchorY(float anchorY, double duration = 0.0, EasingFunction easingFunction = Linear::Interpolation, bool reverse = false, int repeatCount = 0);
        void SetAnchorPoint(float anchorX, float anchorY, double duration = 0.0, EasingFunction easingFunction = Linear::Interpolation, bool reverse = false, int repeatCount = 0);
        void SetAnchorPoint(vec2 anchorPoint, double duration = 0.0, EasingFunction easingFunction = Linear::Interpolation, bool reverse = false, int repeatCount = 0);
        
        //Returns the anchor point of the Label
        vec2 GetAnchorPoint();
        
        //Sets the color of the Label
        void SetColor(Color color);
        
        //Returns the Color of the Label
        Color GetColor();
        
        //Sets the alpha for the Label, can be set to animate over a duration, an easing function can be applied, can be revered and repeated
        void SetAlpha(float alpha, double duration = 0.0, EasingFunction easingFunction = Linear::Interpolation, bool reverse = false, int repeatCount = 0);
        
        //Returns the alpha of the Label
        float GetAlpha();
        
        //Set the angle of the characters, can be set to animate over a duration, an easing function can be applied, can be revered and repeated
        void SetCharacterAngle(float characterAngle, double duration = 0.0, EasingFunction easingFunction = Linear::Interpolation, bool reverse = false, int repeatCount = 0);
        
        //Returns the angles of the characters
        float GetCharacterAngle();
        
        //Set the spacing between each character, can be set to animate over a duration, an easing function can be applied, can be revered and repeated
        void SetCharacterSpacing(float characterSpacing, double duration = 0.0, EasingFunction easingFunction = Linear::Interpolation, bool reverse = false, int repeatCount = 0);
        
        //Returns the spacing between each character
        float GetCharacterSpacing();
        
        //Sets the justification of the Label
        void SetJustification(LabelJustification justification);
        
        //Returns the justification of the Label
        LabelJustification GetJustification();
        
        //Returns how many lines make up the Label
        unsigned long GetNumberOfLines();
        
        //Method used to cancel all running tweens
        void CancelTweens();
        
    private:
        //Used to Reset the Model Matrix
        void ResetModelMatrix();
        
        //Conveniance methods to resize and reset the render target
        void ResizeRenderTarget();
        void ResetRenderTarget();
        
        //Conveniance method to calculate the size of the Label, based on the text
        vec2 CalculateSize();
        
        //Conveniance method to draw the text
        void DrawText();
        
        //Conveniance method to draw an individual character
        void DrawCharacter(TextureFrame* textureFrame, vec2 position);
    
        //Member variables
        Font* m_Font;
        Shader* m_Shader;
        RenderTarget* m_RenderTarget;
        LabelJustification m_Justification;
        string m_Text;
        vec2 m_Size;
        vec2 m_AnchorPoint;
        Color m_Color;
        float m_CharacterAngle;
        float m_CharacterSpacing;
        vector<float> m_LineWidth;
        Tween m_TweenAnchorX;
        Tween m_TweenAnchorY;
        Tween m_TweenAlpha;
        Tween m_TweenCharacterAngle;
        Tween m_TweenCharacterSpacing;
    };
}

#endif /* defined(__GameDev2D__Label__) */
