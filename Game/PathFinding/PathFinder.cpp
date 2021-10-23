//
//  PathFinder.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-10-20.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

/*
ChangeLog
_______________________________________________
- Implemented PathFind function in Pathfinder.cpp, according to video.
*/

#include "PathFinder.h"
#include "PathNode.h"
#include "../SubSection.h"
#include "../Tiles/Tile.h"
#include "../../Source/UI/UI.h"
#include "../../Source/Graphics/Primitives/Primitives.h"


namespace GameDev2D
{
    PathFinder::PathFinder(SubSection* aSubSection) : BaseObject("PathFinder"),
        m_SubSection(aSubSection),
        m_State(StateIdle),
        m_DestinationTileIndex(-1),
        m_DebugLabel(nullptr),
        m_DebugRect(nullptr)
    {
#if DEBUG
        m_DebugLabel = new Label("OpenSans-CondBold", "ttf", 18);
        m_DebugRect = new Rect();
        m_DebugRect->SetIsFilled(false);
        m_DebugRect->SetAnchorPoint(0.5f, 0.5f);
#endif
    }

    PathFinder::~PathFinder()
    {
        SafeDelete(m_DebugLabel);
        SafeDelete(m_DebugRect);

        ClearPathNodes();
    }

    void PathFinder::DebugDraw()
    {
#if DEBUG
        //Paint the open list path scoring
        m_DebugRect->SetColor(Color::BlueColor());
        for (unsigned int i = 0; i < m_PathNodeOpen.size(); i++)
        {
            m_PathNodeOpen.at(i)->DebugDraw(m_DebugLabel, m_DebugRect);
        }

        //Paint the closed list path scoring
        m_DebugRect->SetColor(Color::RedColor());
        for (unsigned int i = 0; i < m_PathNodeClosed.size(); i++)
        {
            m_PathNodeClosed.at(i)->DebugDraw(m_DebugLabel, m_DebugRect);
        }

        //Paint the final path scoring
        m_DebugRect->SetColor(Color::YellowColor());
        for (unsigned int i = 0; i < m_PathNodeFinal.size(); i++)
        {
            m_PathNodeFinal.at(i)->DebugDraw(m_DebugLabel, m_DebugRect);
        }
#endif
    }

    void PathFinder::Reset()
    {
        //Set the state to idle
        m_State = StateIdle;

        //Clear the path nodes
        ClearPathNodes();
    }

    bool PathFinder::FindPath(Tile* aStartingTile, Tile* aDestinationTile)
    {
        //Check they are valid tiles
        if (aStartingTile != nullptr && aDestinationTile != nullptr)
        {

            //Reset the Pathfinder before we do a search
            Reset();

            //Get the starting tile indesx and the destination tile index
            int startingTileIndex = m_SubSection->GetTileIndexForTile(aStartingTile);
            m_DestinationTileIndex = m_SubSection->GetTileIndexForTile(aDestinationTile);

            //Safe check that the starting tile and the destination aren't the same
            if (startingTileIndex == m_DestinationTileIndex)
            {
                Error(false, "Couldn't find path- The current tile and the destination tile are the same");
                return false;
            }

            //Calculate the H score
            int scoreH = GetManhattanDistanceCost(aStartingTile, aDestinationTile);//Manhatattan distance, distance ignoring obstacles

            //Allocate the starting tile's Path Node and add it to the Open List
            PathNode* pathNode = new PathNode(aStartingTile, scoreH);
            AddPathNodeToOpenList(pathNode);

            //Set the state to searching
            m_State = StateSearchingPath;

            //Loop through the Open List until a path is found or a path can not be found
            while (IsSearchingForPath() == true && m_DestinationTileIndex != -1)
            {
                //Safety check that there path nodes in the open list
                if (m_PathNodeOpen.size() <= 0)
                {
                    //Set that there is an error
                    Error(false, "Couldn't find path");
                    m_State = StateError;
                    return false;
                }

                //Get the node with the lowest F score from the Open List, since its sorted it will
                //be the first element
                int scoreH = GetManhattanDistanceCost(aStartingTile, aDestinationTile);

                //Allocate the starting tile's Path Node and add it to the open list
                PathNode* pathNode = new PathNode(aStartingTile, scoreH);
                AddPathNodeToOpenList(pathNode);

                //Set the state to searching
                m_State = StateSearchingPath;

                //Go through the open list until we find a path, or we check all the tiles and don't find a path
                while (IsSearchingForPath() == true && m_DestinationTileIndex != -1)
                {
                    //Safe check that there are path node's in the Open List
                    if (m_PathNodeOpen.size() == 0)
                    {
                        Error(false, "Couldn't find path");
                        m_State = StateError;
                        return false;
                    }

                    //Get the node with the lowest F score from the Open List, since its sorted
                    //It will be the first element in the Open List
                    PathNode* currentNode = m_PathNodeOpen.front();

                    //Add the node to the clsoed list and remove it from the open list
                    m_PathNodeClosed.push_back(currentNode);//Place in closed list
                    m_PathNodeOpen.erase(m_PathNodeOpen.begin());//Remove from open list

                    //Check to see if the node is at the destination
                    int currentTileIndex = m_SubSection->GetTileIndexForTile(currentNode->GetTile());

                    if (currentTileIndex == m_DestinationTileIndex)
                    {
                        //Trace the path used to reach destination
                        BuildFinalNodePath(currentNode);

                        //Set that we found a path
                        m_State = StateFoundPath;

                        //return success
                        return true;
                    }

                    //If this current node is not the destination tile, search the tiles adjacent
                    //to it.
                    vector <Tile*> adjacentTiles;

                    //Check the tiles adjacent to the current one, including if they are walkable and add them to the vector
                    //Up tile
                    AddAdjacentTile(adjacentTiles, currentNode->GetTile(), 0, 1);

                    //Down tile
                    AddAdjacentTile(adjacentTiles, currentNode->GetTile(), 0, -1);

                    //Right tile
                    AddAdjacentTile(adjacentTiles, currentNode->GetTile(), 1, 0);

                    //Left tile
                    AddAdjacentTile(adjacentTiles, currentNode->GetTile(), -1, 0);

                    //Cycle through the walkable adjacent tiles
                    for (unsigned int i = 0; i < adjacentTiles.size(); i++)
                    {
                        Tile* adjacentTile = adjacentTiles.at(i);

                        //Check if the tile is in the closed list
                        if (DoesTileExistInClosedList(adjacentTile) == true)
                        {
                            continue;//Ignore it
                        }

                        //Check if the tile is in the open list
                        if (DoesTileExistInOpenList(adjacentTile) == false)
                        {
                            //If it is not in the open list, calculate its H score so that we can add it to the Open List
                            Tile* destinationTile = m_SubSection->GetTileForIndex(m_DestinationTileIndex);

                            int HScore = GetManhattanDistanceCost(adjacentTile, destinationTile);//Calcuate Manhattan distance from enemy to Destination tile

                            //Create a path node for the tile
                            PathNode* pathNode = new PathNode(adjacentTile, currentNode, HScore);

                            //Add path node to open list, which will also sort the list
                            AddPathNodeToOpenList(pathNode);
                        }
                        else//if the tile is already in the open list
                        {
                            //Compare the G score and keep the path node with the lowest G score
                            PathNode* existingNode = GetOpenPathNodeForTile(adjacentTile);

                            //If the tile has a lower G score, update the new G score and the parent
                            if (currentNode->GetScoreG() + 1 < existingNode->GetScoreG())
                            {
                                //Update the existing node's parent, this will also update the G
                                //score of the existing node
                                existingNode->SetParentNode(currentNode);

                                //Re-Sort the open list
                                SortOpenList();

                            }
                        }


                    }


                }

            }
        }
        //If we got here, then a path was NOT found
        return false;
    }

    bool PathFinder::IsSearchingForPath()
    {
        return m_State == StateSearchingPath;
    }

    int PathFinder::GetPathSize()
    {
        return m_PathNodeFinal.size();
    }

    PathNode* PathFinder::GetPathNodeAtIndex(unsigned int aIndex)
    {
        if ((int)aIndex < GetPathSize())
        {
            return m_PathNodeFinal.at(aIndex);
        }
        return nullptr;
    }

    void PathFinder::AddAdjacentTile(std::vector<Tile*>& aAdjacentTiles, Tile* aCurrentTile, int aDeltaX, int aDeltaY)
    {
        //Get the adjacent tile's coordinates
        ivec2 adjacentCoordinate = m_SubSection->GetTileCoordinatesForPosition(aCurrentTile->GetCenter(false));
        adjacentCoordinate.x += aDeltaX;
        adjacentCoordinate.y += aDeltaY;

        //Validate the tile coordinates
        if (m_SubSection->ValidateCoordinates(adjacentCoordinate) == true)
        {
            //Verify that the adjacent tile is walkable
            Tile* adjacentTile = m_SubSection->GetTileForCoordinates(adjacentCoordinate);
            if (adjacentTile != nullptr)
            {
                if (adjacentTile->IsWalkable() == true)
                {
                    aAdjacentTiles.push_back(adjacentTile);
                }
            }
        }
    }

    bool PathFinder::DoesTileExistInClosedList(Tile* aTile)
    {
        //Get the tile index from the subsection for the tile pointer
        int tileIndex = m_SubSection->GetTileIndexForTile(aTile);

        //Cycle through the closed list and compare the tile indexes
        for (unsigned int i = 0; i < m_PathNodeClosed.size(); i++)
        {
            PathNode* pathNode = m_PathNodeClosed.at(i);
            if (m_SubSection->GetTileIndexForTile(pathNode->GetTile()) == tileIndex)
            {
                return true;
            }
        }

        //The tile doesn't exist in the closed list
        return false;
    }

    bool PathFinder::DoesTileExistInOpenList(Tile* aTile)
    {
        return GetOpenPathNodeForTile(aTile) != nullptr;
    }

    PathNode* PathFinder::GetOpenPathNodeForTile(Tile* aTile)
    {
        //Get the tile index from the subsection for the tile pointer
        int tileIndex = m_SubSection->GetTileIndexForTile(aTile);

        //Cycle through the open list and compare the tile indexes
        for (unsigned int i = 0; i < m_PathNodeOpen.size(); i++)
        {
            PathNode* pathNode = m_PathNodeOpen.at(i);
            if (m_SubSection->GetTileIndexForTile(pathNode->GetTile()) == tileIndex)
            {
                return pathNode;
            }
        }

        //The tile doesn't exist in the open list, return null
        return nullptr;
    }

    void PathFinder::SortOpenList()
    {
        sort(m_PathNodeOpen.begin(), m_PathNodeOpen.end(), PathNode::CompareNodes);
    }

    void PathFinder::AddPathNodeToOpenList(PathNode* aPathNode)
    {
        //Insert the Path node into the Open path node vector
        m_PathNodeOpen.push_back(aPathNode);

        //Then, sort the open list
        SortOpenList();
    }

    void PathFinder::BuildFinalNodePath(PathNode* aPathNode)
    {
        do
        {
            //Safety check the parentNode
            if (aPathNode->GetParent() != nullptr)
            {
                m_PathNodeFinal.insert(m_PathNodeFinal.begin(), aPathNode);
            }

            //Set the path node's pointer to it's parent
            aPathNode = aPathNode->GetParent();
        } while (aPathNode != nullptr);
    }

    void PathFinder::ClearPathNodes()
    {
        //Now cycle through the Open node path vector, and delete all the path node
        while (m_PathNodeOpen.size() > 0)
        {
            //Get the last element in the vector
            PathNode* node = m_PathNodeOpen.back();

            //Delete the path node
            delete node;

            //Remove the last element in the vector
            m_PathNodeOpen.pop_back();
        }

        //Lastly cycle through the Closed node path vector, and delete all the path node
        while (m_PathNodeClosed.size() > 0)
        {
            //Get the last element in the vector
            PathNode* node = m_PathNodeClosed.back();

            //Delete the path node
            delete node;

            //Remove the last element in the vector
            m_PathNodeClosed.pop_back();
        }

        //Clear the final path node list
        m_PathNodeFinal.clear();

        //Reset the destination tile index
        m_DestinationTileIndex = -1;
    }

    int PathFinder::GetManhattanDistanceCost(Tile* aStartTile, Tile* aDestinationTile)
    {
        //Here we use the Manhattan method, which calculates the total number of step moved horizontally and vertically to reach the
        //final desired step from the current step, ignoring any obstacles that may be in the way
        uvec2 startCoordinates = m_SubSection->GetTileCoordinatesForPosition(aStartTile->GetCenter(false));
        uvec2 destinationCoordinates = m_SubSection->GetTileCoordinatesForPosition(aDestinationTile->GetCenter(false));

        //Calculate the manhattan distance
        int distance = std::abs((int)(destinationCoordinates.x - startCoordinates.x)) + std::abs((int)(destinationCoordinates.y - startCoordinates.y));
        return distance;
    }
}