//
//  Polygon.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-03-18.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#include "Polygon.h"
#include "../../Services/ServiceLocator.h"
#include "../../Services/Graphics/Graphics.h"
#include "../../Services/ShaderManager/ShaderManager.h"


namespace GameDev2D
{
    Polygon::Polygon(const string& aType) : GameObject(aType == "" ? "Polygon" : aType),
        m_Shader(nullptr),
        m_AnchorPoint(0.0f, 0.0f),
        m_AnchorLocation(0.0f, 0.0f),
        m_Color(Color::BlueColor()),
        m_RenderMode(GL_TRIANGLE_FAN),
        m_IsFilled(true),
        m_PointSize(1.0f),
        m_TweenAnchorX(),
        m_TweenAnchorY(),
        m_TweenAlpha(),
        m_VertexArrayObject(0),
        m_VertexBufferObject(0)
    {
        //Set the shader as the default passthrough shader
        SetShader(ServiceLocator::GetShaderManager()->GetPassthroughShader());
        
        //Intialize the position and angle
        SetLocalPosition(0.0f, 0.0f);
        SetLocalAngle(0.0f);
        
        //Set the model matrix to dirty
        ModelMatrixIsDirty();
    }
    
    Polygon::Polygon(vec2* aVertices, unsigned int aCount) : GameObject("Polygon"),
        m_Shader(nullptr),
        m_AnchorPoint(0.0f, 0.0f),
        m_AnchorLocation(0.0f, 0.0f),
        m_Color(Color::BlueColor()),
        m_RenderMode(GL_TRIANGLE_FAN),
        m_IsFilled(true),
        m_PointSize(1.0f),
        m_TweenAnchorX(),
        m_TweenAnchorY(),
        m_TweenAlpha(),
        m_VertexArrayObject(0),
        m_VertexBufferObject(0)
    {
        //Set the shader as the default passthrough shader
        SetShader(ServiceLocator::GetShaderManager()->GetPassthroughShader());
        
        //Set the vertices
        SetVertices(aVertices, aCount);
        
        //Intialize the position and angle
        SetLocalPosition(vec2(0.0f, 0.0f));
        SetLocalAngle(0.0f);
        
        //Set the model matrix to dirty
        ModelMatrixIsDirty(true);
    }
    
    Polygon::Polygon(vec2* aVertices, unsigned int aCount, Shader* aShader) : GameObject("Polygon"),
        m_Shader(aShader),
        m_AnchorPoint(0.0f, 0.0f),
        m_AnchorLocation(0.0f, 0.0f),
        m_Color(Color::BlackColor()),
        m_RenderMode(GL_TRIANGLE_FAN),
        m_IsFilled(true),
        m_PointSize(1.0f),
        m_TweenAnchorX(),
        m_TweenAnchorY(),
        m_TweenAlpha(),
        m_VertexArrayObject(0),
        m_VertexBufferObject(0)
    {
        //Set the vertices
        SetVertices(aVertices, aCount);
    
        //Intialize the position and angle
        SetLocalPosition(vec2(0.0f, 0.0f));
        SetLocalAngle(0.0f);
        
        //Set the model matrix to dirty
        ModelMatrixIsDirty();
    }
    
    Polygon::~Polygon()
    {
        //Set the shader to null
        m_Shader = nullptr;
        
        //Delete the VBO
        if(m_VertexBufferObject != 0)
        {
            glDeleteBuffers(1, &m_VertexBufferObject);
            m_VertexBufferObject = 0;
        }

        //Delete the VAO
        if(m_VertexArrayObject != 0)
        {
            glDeleteVertexArrays(1, &m_VertexArrayObject);
            m_VertexArrayObject = 0;
        }
        
        //Clear the vertices vector
        m_Vertices.clear();
        
        //Clear the colors vector
        m_Colors.clear();
    }
    
    void Polygon::Update(double aDelta)
    {
        //Update the anchor points and alpha tweens
        UpdateTween(aDelta, &m_TweenAnchorX, static_cast<GameObject::TweenSetMethod>(&Polygon::SetAnchorX));
        UpdateTween(aDelta, &m_TweenAnchorY, static_cast<GameObject::TweenSetMethod>(&Polygon::SetAnchorY));
        UpdateTween(aDelta, &m_TweenAlpha, static_cast<GameObject::TweenSetMethod>(&Polygon::SetAlpha));
    
        //Call the GameObject's Update() method, this will ensure that any children will also get updated
        GameObject::Update(aDelta);
    }
    
    void Polygon::Draw()
    {
        //Safety check the shader
        if(m_Shader == nullptr)
        {
            return;
        }
        
        //If the model matrix is dirty, reset it
        if(IsModelMatrixDirty() == true)
        {
            ResetModelMatrix();
        }
    
        //Use the shader
        m_Shader->Use();
        
        //Set the point size attribute
        int pointSizeIndex = m_Shader->GetAttribute("a_pointSize");
        glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
        glVertexAttrib1f(pointSizeIndex, m_PointSize);
        
        //Cache the graphics service
        Graphics* graphics = ServiceLocator::GetGraphics();
    
        //Bind the vertex array object
        graphics->BindVertexArray(m_VertexArrayObject);

        //Set the model view projection matrix
        mat4 mvp = graphics->GetProjectionMatrix() * graphics->GetViewMatrix() * m_ModelMatrix;
        glUniformMatrix4fv(m_Shader->GetModelViewProjectionUniform(), 1, 0, &mvp[0][0]);
        
        //Validate the shader
        if(m_Shader->Validate() == false)
        {
            Error(false, "Can't draw Polygon, shader failed to validate");
            return;
        }
        
        //Disable blending, if we did in fact have it enabled
        if(m_Color.Alpha() != 1.0f)
        {
            graphics->EnableBlending();
        }
        
        //Render the polygon
        glDrawArrays(m_RenderMode, 0, (GLsizei)m_Vertices.size());
        
        //Disable blending, if we did in fact have it enabled
        if(m_Color.Alpha() != 1.0f)
        {
            graphics->DisableBlending();
        }
        
        //Unbind the vertex array
        ServiceLocator::GetGraphics()->BindVertexArray(0);
        
        //Draw the debug anchor point
        #if DRAW_POLYGON_ANCHOR_POINT
        if(GetType() != "Point" && GetType() != "Line")
        {
            Line lineA(m_AnchorLocation, vec2(m_AnchorLocation.x, m_AnchorLocation.y + DRAW_POLYGON_ANCHOR_POINT_SIZE));
            lineA.SetLocalAngle(GetWorldAngle());
            lineA.SetColor(Color::RedColor());
            lineA.Draw();
        
            Line lineB(m_AnchorLocation, vec2(m_AnchorLocation.x + DRAW_POLYGON_ANCHOR_POINT_SIZE, m_AnchorLocation.y));
            lineB.SetLocalAngle(GetWorldAngle());
            lineB.SetColor(Color::GreenColor());
            lineB.Draw();
        }
        #endif
        
        //Call the GameObject's Draw() method, this will ensure that any children will also get drawn
        GameObject::Draw();
    }
    
    void Polygon::AddVertex(vec2 aVertex)
    {
        //Add the vertex to the vertices vector
        m_Vertices.push_back(aVertex);
        
        //Add color for the vertex to the colors vector
        m_Colors.push_back(vec4(m_Color.Red(), m_Color.Green(), m_Color.Blue(), m_Color.Alpha()));
    }
        
    void Polygon::SetVertices(vec2* aVertices, unsigned int aCount)
    {
        //Clear the vertices vector
        m_Vertices.clear();
        
        //Clear the colors vector
        m_Colors.clear();
        
        //Cycle through and add all the vertices
        for(unsigned int i = 0; i < aCount; i++)
        {
            AddVertex(aVertices[i]);
        }
        
        //Reset the polygon data
        ResetPolygonData();
    }
    
    void Polygon::SetShader(Shader* aShader)
    {
        //Set the new shader
        if(aShader != nullptr)
        {
            m_Shader = aShader;
        }
    }
    
    Shader* Polygon::GetShader() const
    {
        return m_Shader;
    }
    
    void Polygon::SetAnchorX(float aAnchorX, double aDuration, EasingFunction aEasingFunction, bool aReverse, int aRepeatCount)
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
    
    void Polygon::SetAnchorY(float aAnchorY, double aDuration, EasingFunction aEasingFunction, bool aReverse, int aRepeatCount)
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
    
    void Polygon::SetAnchorPoint(float aAnchorX, float aAnchorY, double aDuration, EasingFunction aEasingFunction, bool aReverse, int aRepeatCount)
    {
        SetAnchorX(aAnchorX, aDuration, aEasingFunction, aReverse, aRepeatCount);
        SetAnchorY(aAnchorY, aDuration, aEasingFunction, aReverse, aRepeatCount);
    }
    
    void Polygon::SetAnchorPoint(vec2 aAnchorPoint, double aDuration, EasingFunction aEasingFunction, bool aReverse, int aRepeatCount)
    {
        SetAnchorPoint(aAnchorPoint.x, aAnchorPoint.y, aDuration, aEasingFunction, aReverse, aRepeatCount);
    }
    
    vec2 Polygon::GetAnchorPoint()
    {
        return m_AnchorPoint;
    }
    
    void Polygon::SetColor(Color aColor, bool aUpdateVector)
    {
        //Set the color
        m_Color = aColor;
        
        //Cycle through and update the colors vector
        if(aUpdateVector == true)
        {
            for(unsigned int i = 0; i < m_Colors.size(); i++)
            {
                m_Colors.at(i).r = m_Color.Red();
                m_Colors.at(i).g = m_Color.Green();
                m_Colors.at(i).b = m_Color.Blue();
                m_Colors.at(i).a = m_Color.Alpha();
            }
            
            ResetPolygonData();
        }
    }
    
    Color Polygon::GetColor()
    {
        return m_Color;
    }
    
    void Polygon::SetAlpha(float aAlpha, double aDuration, EasingFunction aEasingFunction, bool aReverse, int aRepeatCount)
    {
        if(aDuration == 0.0)
        {
            //Set the alpha
            m_Color.SetAlpha(aAlpha);
            
            //Cycle through all the vertices and set the color for them
            for(unsigned int i = 0; i < m_Colors.size(); i++)
            {
                m_Colors.at(i).a = m_Color.Alpha();
            }
            
            ResetPolygonData();
        }
        else
        {
            if(m_Color.Alpha() != aAlpha)
            {
                m_TweenAlpha.Set(m_Color.Alpha(), aAlpha, aDuration, aEasingFunction, aReverse, aRepeatCount);
            }
        }
    }
    
    float Polygon::GetAlpha()
    {
        return m_Color.Alpha();
    }
    
    void Polygon::SetIsFilled(bool aIsFilled)
    {
        m_IsFilled = aIsFilled;
        ResetPolygonData();
    }
    
    bool Polygon::GetIsFilled()
    {
        return m_IsFilled;
    }
    
    void Polygon::SetRenderMode(unsigned int aRenderMode)
    {
        m_RenderMode = aRenderMode;
    }
    
    void Polygon::CancelTweens()
    {
        m_TweenAnchorX.Cancel();
        m_TweenAnchorY.Cancel();
        m_TweenAlpha.Cancel();
    }
    
    void Polygon::SetPointSize(float aPointSize)
    {
        m_PointSize = aPointSize;
        UpdateVertexBuffer();
    }
    
    float Polygon::GetPointSize()
    {
        return m_PointSize;
    }
    
    void Polygon::ResetPolygonData()
    {
        //Set the render mode
        SetRenderMode(m_IsFilled ? GL_TRIANGLE_FAN : GL_LINE_LOOP);
        
        //And update the vertex buffer
        UpdateVertexBuffer();
    }
    
    void Polygon::UpdateVertexBuffer()
    {
        //If the shader is null we can't update the vertex buffer
        if(m_Shader == nullptr)
        {
            return;
        }
        
        //Safety check the vertices
        if(m_Vertices.size() == 0)
        {
            return;
        }
    
        //If the VAO hasn't been generated, generate one
        if(m_VertexArrayObject == 0)
        {
            glGenVertexArrays(1, &m_VertexArrayObject);
        }
        
        //Bind the VAO
        ServiceLocator::GetGraphics()->BindVertexArray(m_VertexArrayObject);

        //Use the shader
        m_Shader->Use();

        //If the VBO hasn't been generated, generate one
        if(m_VertexBufferObject == 0)
        {
            glGenBuffers(1, &m_VertexBufferObject);
        }
        
        //Bind the VBO
        glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferObject);

        //Create an array to hold the vertices
        const int vertexCount = m_Vertices.size();
        const int vertexSize = 2;
        const int colorSize = 4;
        float* vertices = new float[vertexCount * (vertexSize + colorSize)];
        
        //Cycle through and set the vertices and their colors
        for(int i = 0; i < vertexCount; i++)
        {
            int offset = i * (vertexSize + colorSize);
            vertices[offset] = m_Vertices.at(i).x;      //X vertex
            vertices[offset+1] = m_Vertices.at(i).y;    //Y vertex
            vertices[offset+2] = m_Color.Red();         //Red color
            vertices[offset+3] = m_Color.Green();       //Green color
            vertices[offset+4] = m_Color.Blue();        //Blue color
            vertices[offset+5] = m_Color.Alpha();       //Alpha color
        }
        
        //Set the vertex data
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexCount * (vertexSize + colorSize), vertices, GL_STATIC_DRAW);
        
        //Enable the shader's vertices attribute
        int verticesIndex = m_Shader->GetAttribute("a_vertices");
        glEnableVertexAttribArray(verticesIndex);

        //Set the shader's vertices attribute
        int verticesOffset = 0;
        glVertexAttribPointer(verticesIndex, vertexSize, GL_FLOAT, GL_FALSE, sizeof(float) * (vertexSize + colorSize), (const GLvoid*)verticesOffset);
        
        //Enable the shader's color attribute
        int colorIndex = m_Shader->GetAttribute("a_sourceColor");
        glEnableVertexAttribArray(colorIndex);
        
        //Set the shader's colors attribute
        int colorOffset = vertexSize * sizeof(float);
        glVertexAttribPointer(colorIndex, colorSize, GL_FLOAT, GL_FALSE, sizeof(float) * (vertexSize + colorSize), (const GLvoid*)colorOffset);
        
        //Delete the vertices array
        SafeDeleteArray(vertices);
        
        //Unbind the VAO
        ServiceLocator::GetGraphics()->BindVertexArray(0);
    }
    
    void Polygon::ResetModelMatrix()
    {
        //Local variables used below
        float left = 0.0f;
        float right = 0.0f;
        float bottom = 0.0f;
        float top = 0.0f;
    
        //Cycle through and set the left, right, bottom and top vars
        for(unsigned int i = 0; i < m_Vertices.size(); i++)
        {
            left = fminf(left, m_Vertices.at(i).x);
            right = fmaxf(right, m_Vertices.at(i).x);
            bottom = fminf(bottom, m_Vertices.at(i).y);
            top = fmaxf(top, m_Vertices.at(i).y);
        }
    
        //Set the width and height
        float width = right - left;
        float height = top - bottom;

        //Translate the anchor, then translate the position
        mat4 anchor = translate(mat4(1.0f), vec3(-width * GetWorldScale().x * GetAnchorPoint().x, -height * GetWorldScale().y * GetAnchorPoint().y, 0.0f));
        mat4 viewTranslate = translate(anchor, vec3(GetWorldPosition().x, GetWorldPosition().y, 0.0f));

        //Calculate the rotation based on the anchor point
        mat4 halfTranslate1 = translate(viewTranslate, vec3(width * GetWorldScale().x * GetAnchorPoint().x, height * GetWorldScale().y * GetAnchorPoint().y, 0.0f));
        
    #if DRAW_POLYGON_ANCHOR_POINT
        vec4 transformedVector = halfTranslate1 * vec4(0.0f, 0.0f, 0.0f, 1.0);
        m_AnchorLocation = vec2(transformedVector.x, transformedVector.y);
    #endif
        
        mat4 viewRotation = rotate(halfTranslate1, GetWorldAngle(), vec3(0.0f, 0.0f, 1.0f));
        mat4 halfTranslate2 = translate(viewRotation, vec3(-width * GetWorldScale().x * GetAnchorPoint().x, -height * GetWorldScale().y * GetAnchorPoint().y, 0.0f));
        
        //Lastly the scale
        m_ModelMatrix = scale(halfTranslate2, vec3(GetWorldScale().x, GetWorldScale().y, 0.0f));
        
        //Reset the model matrix
        GameObject::ResetModelMatrix();
    }
}