//
//  PathNode.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-10-20.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__PathNode__
#define __GameDev2D__PathNode__

#include "../../Source/Core/BaseObject.h"
#include "../../Source/Graphics/Core/Color.h"

namespace GameDev2D
{
    //Forward declarations
    class Tile;
    class Label;
    class Rect;

    //The PathNode class represents a Tile in a Subsection, it contains data like the Tile's
    //G, H, and F score as well as the parent PathNode
    class PathNode : public BaseObject
    {
    public:
        PathNode(Tile* tile, unsigned int scoreH);
        PathNode(Tile* tile, PathNode* parent, unsigned int scoreH);
        ~PathNode();
        
        //Used to draw the path node's score
        void DebugDraw(Label* label, Rect* rect);
        
        //Returns the tile associated with the PathNode
        Tile* GetTile();
        
        //Returns the parent path node
        PathNode* GetParent();
        
        //Set the path node's parent node
        void SetParentNode(PathNode* parent);
        
        //Returns the g score (movement cost)
        unsigned int GetScoreG();
        
        //Returns the h score (manhattan distance)
        unsigned int GetScoreH();
        
        //Returns the final score for the path node
        unsigned int GetScoreF();
        
        //Comparison function, used to determine the path node with the higher f score
        static bool CompareNodes(PathNode* nodeA, PathNode* nodeB);
        
    private:
        //Member variables
        Tile* m_Tile;
        PathNode* m_Parent;
        unsigned int m_ScoreH;
    };
}

#endif /* defined(__GameDev2D__PathNode__) */
