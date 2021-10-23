//
//  Polygon.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-03-18.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__Polygon__
#define __GameDev2D__Polygon__

#include "../../Core/GameObject.h"
#include "../Core/Color.h"
#include "../../Animation/Tween.h"
#include "../../Animation/Easing.h"


using namespace glm;
using namespace std;

namespace GameDev2D
{
    //Forward declarations
    class Shader;

    //Polygon class inherits from the GameObject, so it can be added to any scene. It has an anchor point
    //that acts as the 'origin' of the polygon, the position will be set to the anchor point. The polygon
    //will rotate around the anchor point as well. The Polygon class is the base class for all the other
    //primitives, but can be used on its own to come up with complex polygon.
    class Polygon : public GameObject
    {
    public:
        //Constructs the polygon with no vertices, the optional parameter is for inheriting classes to pass their type
        Polygon(const string& type = "");
        
        //Constructs the polygon with a set of vertices
        Polygon(vec2* vertices, unsigned int count);
        
        //Constructs the polygon with a set of vertices and a shader. The Polygon assumes ownership of the shader and
        //will remove the Shader from the ShaderManager
        Polygon(vec2* vertices, unsigned int count, Shader* shader);
        virtual ~Polygon();
        
        //Used to update and draw the polygon
        virtual void Update(double delta);
        virtual void Draw();
        
        //Adds a vertex to the polygon, the vertex will be added and set to the active polygon color
        void AddVertex(vec2 vertex);
        
        //Set an array of vertices
        void SetVertices(vec2* vertices, unsigned int count);
        
        //Set the shader used to render this polygon
        void SetShader(Shader* shader);
        
        //Returns the shader used to render this polygon
        Shader* GetShader() const;
        
        //Sets the anchor point for the polygon, can be set to animate over a duration, an easing function can be applied, can be revered and repeated
        void SetAnchorX(float anchorX, double duration = 0.0, EasingFunction easingFunction = Linear::Interpolation, bool reverse = false, int repeatCount = 0);
        void SetAnchorY(float anchorY, double duration = 0.0, EasingFunction easingFunction = Linear::Interpolation, bool reverse = false, int repeatCount = 0);
        void SetAnchorPoint(float anchorX, float anchorY, double duration = 0.0, EasingFunction easingFunction = Linear::Interpolation, bool reverse = false, int repeatCount = 0);
        void SetAnchorPoint(vec2 anchorPoint, double duration = 0.0, EasingFunction easingFunction = Linear::Interpolation, bool reverse = false, int repeatCount = 0);
        
        //Returns the anchor point for the polygon
        vec2 GetAnchorPoint();
        
        //Set the color of the polygon, the optional flag is used to update all the vertices in the polygon to the color
        void SetColor(Color color, bool updateVector = true);
        
        //Returns the active color for the polygon
        Color GetColor();
        
        //Sets the alpha for the Polygon, can be set to animate over a duration, an easing function can be applied, can be revered and repeated
        void SetAlpha(float alpha, double duration = 0.0, EasingFunction easingFunction = Linear::Interpolation, bool reverse = false, int repeatCount = 0);
        
        //Returns the alpha for the Sprite
        float GetAlpha();
        
        //Set if the polygon is filled or not
        void SetIsFilled(bool isFilled);
        
        //Returns wether the polygon is filled
        bool GetIsFilled();
        
        //Set the point size of the polygon, only applies in the render mode is set to GL_POINTS
        void SetPointSize(float pointSize);
        
        //Returns the point size of the polygon
        float GetPointSize();
        
        //Set the open gl render mode of the polygon
        void SetRenderMode(unsigned int renderMode);
        
        //Method to cancel any running tweens
        virtual void CancelTweens();
        
    protected:
        //Used to reset the polygon data
        virtual void ResetPolygonData();
        
        //Used to reset the model matrix for the polygon
        virtual void ResetModelMatrix();
    
        //Used to update the vertex buffer
        void UpdateVertexBuffer();
    
        //Member variables
        vector<vec2> m_Vertices;
        Shader* m_Shader;
        vec2 m_AnchorPoint;
        vec2 m_AnchorLocation;
        Color m_Color;
        vector<vec4> m_Colors;
        unsigned int m_RenderMode;
        bool m_IsFilled;
        float m_PointSize;
        Tween m_TweenAnchorX;
        Tween m_TweenAnchorY;
        Tween m_TweenAlpha;
        unsigned int m_VertexArrayObject;
        unsigned int m_VertexBufferObject;
    };
}

#endif /* defined(__GameDev2D__Polygon__) */
