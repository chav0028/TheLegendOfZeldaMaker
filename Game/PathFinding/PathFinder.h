//
//  PathFinder.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-10-20.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__PathFinder__
#define __GameDev2D__PathFinder__

#include "../../Source/Core/BaseObject.h"
#include "PathNode.h"


namespace GameDev2D
{
    //Enum to keep track of the internal state of the path finder
    enum PathFinderState
    {
        StateIdle = 0,
        StateSearchingPath,
        StateFoundPath,
        StateError
    };

    //Forward declaration
    class SubSection;
    class Tile;
    class Label;
    class Rect;

    //The Path finder class with search the walkable tiles in a subsection for a path based on a starting tile and a destination tile
    class PathFinder : public BaseObject
    {
    public:
        PathFinder(SubSection* subSection);
        ~PathFinder();

        //Used to debug draw the path node scores
        void DebugDraw();

        //Resets teh PathFinder to an idle state
        void Reset();

        //Pass in a starting tile and a destination tile and it will return true if a
        //path can be found, and false if a path can NOT be found
        bool FindPath(Tile* startingTile, Tile* destinationTile);

        //Returns wether the PathFinder is searching for a path
        bool IsSearchingForPath();

        //Returns the size (number of path nodes) for the found path
        int GetPathSize();

        //Returns the PathNode from the final path list for a given index
        PathNode* GetPathNodeAtIndex(unsigned int index);

    private:
        //Adds add adjacent tile to the adjacentTiles parameter if it is walkable
        void AddAdjacentTile(vector<Tile*>& adjacentTiles, Tile* currentTile, int deltaX, int deltaY);

        //Checks if a tile already exists in the Open and Closed List
        bool DoesTileExistInClosedList(Tile* tile);
        bool DoesTileExistInOpenList(Tile* tile);

        //Returns a PathNode that matches the Tile from the Open List
        PathNode* GetOpenPathNodeForTile(Tile* tile);

        //Sort the open list, the path node with the lowest F score will be first
        void SortOpenList();

        //Add a path node to the open list and sorts it based on the F score
        void AddPathNodeToOpenList(PathNode* pathNode);

        //Build the final path, called once a path is found
        void BuildFinalNodePath(PathNode* pathNode);

        //Clears the Open, closed and final path Lists, cleans up all memory associated with them
        void ClearPathNodes();

        //Calculate the manhattan distance (h score)
        int GetManhattanDistanceCost(Tile* startTile, Tile* destinationTile);

        //Member variables
        SubSection* m_SubSection;
        PathFinderState m_State;
        int m_DestinationTileIndex;
        std::vector<PathNode*> m_PathNodeOpen;
        std::vector<PathNode*> m_PathNodeClosed;
        std::vector<PathNode*> m_PathNodeFinal;
        Label* m_DebugLabel;
        Rect* m_DebugRect;
    };
}

#endif /* defined(__GameDev2D__PathFinder__) */
