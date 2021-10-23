//
//  PrimitivesExample.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-08-12.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#include "PrimitivesExample.h"
#include "../../Source/Services/ServiceLocator.h"
#include "../../Source/Platforms/PlatformLayer.h"
#include "../../Source/Graphics/Primitives/Primitives.h"
#include "../../Source/Events/Input/InputEvents.h"



namespace GameDev2D
{
    PrimitivesExample::PrimitivesExample() : Scene("PrimitivesExample"),
        m_Circle(nullptr),
        m_Rect(nullptr),
        m_Line(nullptr),
        m_Polygon(nullptr)
    {
    
    }
    
    PrimitivesExample::~PrimitivesExample()
    {
        //The Scene takes care of deleting any GameObjects, which includes our Polygon objects
    }
    
    float PrimitivesExample::LoadContent()
    {
        //Local variables used for positioning
        float width = (float)ServiceLocator::GetPlatformLayer()->GetWidth();
        float height = (float)ServiceLocator::GetPlatformLayer()->GetHeight();
    
        //Setup the vertices and create a Polygon
        unsigned int vertexCount = 5;
        vec2 vertices[5];
        vertices[0] = vec2(-100.0f, 0.0f);
        vertices[1] = vec2(-150.0f, 100.0f);
        vertices[2] = vec2(0.0f, 200.0f);
        vertices[3] = vec2(150.0f, 100.0f);
        vertices[4] = vec2(100.0f, 0.0f);
        m_Polygon = (Polygon*)AddGameObject(new Polygon(vertices, vertexCount));
        m_Polygon->SetLocalPosition(200.0f, 200.0f);
    
        //Create the Circle object
        m_Circle = (Circle*)AddGameObject(new Circle(width / 2.0f, height / 2.0f, 50.0f));
        m_Circle->SetIsFilled(false);

        //Create the Rect object
        m_Rect = (Rect*)AddGameObject(new Rect(0.0f, 0.0f, 100.0f, 100.0f));
        m_Rect->SetAnchorPoint(0.5f, 0.5f);
        
        //Create the Line object
        m_Line = (Line*)AddGameObject(new Line(0.0f, 0.0f, width, height));
        
        //Return 1.0f when loading has completed
        return 1.0f;
    }
    
    void PrimitivesExample::HandleEvent(Event* aEvent)
    {
        switch (aEvent->GetEventCode())
        {
            //On a mouse click event, create a point at the mouse location, set the point size to 10
            case MOUSE_CLICK_EVENT:
            {
                MouseClickEvent* mouseClickEvent = (MouseClickEvent*)aEvent;
                if(mouseClickEvent->GetMouseClickEventType() == MouseClickUp && mouseClickEvent->GetMouseButton() == MouseButtonLeft)
                {
                    Point* point = (Point*)AddGameObject(new Point(mouseClickEvent->GetPosition()));
                    point->SetPointSize(10.0f);
                    m_Points.push_back(point);
                }
            }
            break;
        
            case KEYBOARD_EVENT:
            {
                KeyEvent* keyEvent = (KeyEvent*)aEvent;
                if(keyEvent->GetKeyEventType() == KeyUp)
                {
                    //When the Space key is pressed animate the circle's radius with an elastic ease, that repeats twice
                    if(keyEvent->GetKeyCode() == KEY_CODE_SPACE)
                    {
                        m_Circle->SetRadius(100.0f, 1.0, Elastic::Out, true, 2);
                    }
                    
                    //When the Tab key is pressed toggle the primitives filled in state
                    else if(keyEvent->GetKeyCode() == KEY_CODE_TAB)
                    {
                        m_Circle->SetIsFilled(!m_Circle->GetIsFilled());
                    }
                    
                    //When the left Shift key is pressed, the Rect animates its position, angle and size
                    else if(keyEvent->GetKeyCode() == KEY_CODE_LEFT_SHIFT)
                    {
                        float width = (float)ServiceLocator::GetPlatformLayer()->GetWidth();
                        float height = (float)ServiceLocator::GetPlatformLayer()->GetHeight();
                        m_Rect->SetLocalPosition(width, height, 2.0);
                        m_Rect->SetLocalAngle(360.0f, 2.0);
                        m_Rect->SetSize(10.0f, 10.0f, 2.0);
                    }
                    
                    //When the right Shift key is pressed, the line animates its point locations
                    else if(keyEvent->GetKeyCode() == KEY_CODE_RIGHT_SHIFT)
                    {
                        float width = (float)ServiceLocator::GetPlatformLayer()->GetWidth();
                        float height = (float)ServiceLocator::GetPlatformLayer()->GetHeight();
                        m_Line->SetLocalPosition(0.0f, height, 1.0);
                        m_Line->SetPositionB(width, 0.0f, 1.0);
                    }
                    
                    //When the A key is pressed, the Polygon rotates and scales
                    else if(keyEvent->GetKeyCode() == KEY_CODE_A)
                    {
                        m_Polygon->SetLocalAngle(m_Polygon->GetLocalAngle() - 360.0f, 3.0);
                        m_Polygon->SetLocalScale(2.0f, 2.0f, 3.0);
                    }
                
                    //When the Backspace key is pressed reset all the buttons back to their initial position
                    else if(keyEvent->GetKeyCode() == KEY_CODE_BACKSPACE)
                    {
                        Reset();
                    }
                }
            }
            break;
                
            default:
                break;
        }
    }
    
    void PrimitivesExample::Reset()
    {
        //Cycle through and clear all the points
        for(unsigned int i = 0; i < m_Points.size(); i++)
        {
            RemoveGameObject(m_Points.at(i));
        }
        m_Points.clear();
    
        //Reset the circle's radius
        m_Circle->SetRadius(50.0f);
    
        //Reset the Rect's position, angle and size
        m_Rect->SetLocalPosition(0.0f, 0.0f);
        m_Rect->SetLocalAngle(0.0f);
        m_Rect->SetSize(100.0f, 100.0f);
        
        //Reset the Line's position
        float width = (float)ServiceLocator::GetPlatformLayer()->GetWidth();
        float height = (float)ServiceLocator::GetPlatformLayer()->GetHeight();
        m_Line->SetLocalPosition(0.0f, 0.0f);
        m_Line->SetPositionB(width, height);
        
        //Reset the Polygon's angle and scale
        m_Polygon->SetLocalAngle(0.0f);
        m_Polygon->SetLocalScale(1.0f, 1.0f);
    }
}