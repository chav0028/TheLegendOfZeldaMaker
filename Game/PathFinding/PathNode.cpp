//
//  PathNode.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-10-20.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#include "PathNode.h"
#include "../Tiles/Tile.h"
#include "../../Source/UI/UI.h"
#include "../../Source/Graphics/Primitives/Primitives.h"


namespace GameDev2D
{
    PathNode::PathNode(Tile* aTile, unsigned int aScoreH) : BaseObject("PathNode"),
        m_Tile(aTile),
        m_Parent(nullptr),
        m_ScoreH(aScoreH)
    {
    
    }

    PathNode::PathNode(Tile* aTile, PathNode* aParent, unsigned int aScoreH) : BaseObject("PathNode"),
        m_Tile(aTile),
        m_Parent(aParent),
        m_ScoreH(aScoreH)
    {

    }
    
    PathNode::~PathNode()
    {

    }
    
    void PathNode::DebugDraw(Label* aLabel, Rect* aRect)
    {
#if DEBUG
        //Draw the rect, the color represent which list the path node is in
        aRect->SetLocalPosition(GetTile()->GetCenter(false));
        aRect->SetSize((float)GetTile()->GetSize(), (float)GetTile()->GetSize());
        aRect->Draw();

        //Draw the F score
        stringstream ss;
        ss << GetScoreF();
        aLabel->SetText(ss.str());
        aLabel->SetAnchorPoint(0.0f, 1.0f);
        aLabel->SetLocalPosition(vec2(GetTile()->GetLocalX(), GetTile()->GetLocalY() + GetTile()->GetSize()));
        aLabel->Draw();
        
        //Draw the S score
        ss.str("");
        ss << GetScoreG();
        aLabel->SetText(ss.str());
        aLabel->SetAnchorPoint(0.0f, 0.0f);
        aLabel->SetLocalPosition(vec2(GetTile()->GetLocalX(), GetTile()->GetLocalY()));
        aLabel->Draw();
        
        //Draw the H score
        ss.str("");
        ss << GetScoreH();
        aLabel->SetText(ss.str());
        aLabel->SetAnchorPoint(1.0f, 0.0f);
        aLabel->SetLocalPosition(vec2(GetTile()->GetLocalX() + GetTile()->GetSize(), GetTile()->GetLocalY()));
        aLabel->Draw();
#endif
    }

    Tile* PathNode::GetTile()
    {
        return m_Tile;
    }

    PathNode* PathNode::GetParent()
    {
        return m_Parent;
    }

    void PathNode::SetParentNode(PathNode* aParent)
    {
        m_Parent = aParent;
    }

    unsigned int PathNode::GetScoreG()
    {
        if(m_Parent != nullptr)
        {
            return m_Parent->GetScoreG() + 1;
        }
        return 1;
    }

    unsigned int PathNode::GetScoreH()
    {
        return m_ScoreH;
    }

    unsigned int PathNode::GetScoreF()
    {
        return GetScoreG() + GetScoreH();
    }

    bool PathNode::CompareNodes(PathNode* aNodeA, PathNode* aNodeB)
    {
        return aNodeA->GetScoreF() < aNodeB->GetScoreF();
    }
}