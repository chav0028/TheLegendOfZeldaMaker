//
//  World.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-10-14.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__World__
#define __GameDev2D__World__

#include "../Source/Core/Scene.h"


using namespace glm;

namespace GameDev2D
{
    //World load step enum
    enum WorldLoadStep
    {
        WorldLoadRequired = 0,
        WorldLoadSubSections,
        WorldLoadHero,
        WorldLoadMusic,
        WorldLoadFinal,
        WorldLoadStepCount
    };

    //Local Constants
    const uvec2 WORLD_NUMBER_OF_SUBSECTIONS = uvec2(4, 3);
    const unsigned int WORLD_TILE_SIZE = 64;
    const unsigned int WORLD_NUMBER_OF_HORIZONTAL_TILES = 16;
    const unsigned int WORLD_NUMBER_OF_VERTICAL_TILES = 11;
    const double WORLD_CAMERA_SHIFT_DURATION = 1.0;

    const float WORLD_OVERWORLD_THEME_SOUND = 0.6f;//Reduce the sound slightly, since it affects the gameplay sound effects.

    //Forward declarations
    class Player;
    class Hero;
    class Pickup;
    class SubSection;
    class Audio;

    //The world class manages the Subsections, camera and Hero in the world
    class World : public Scene
    {
    public:
        World();
        ~World();

        //Load the World's content
        float LoadContent();

        //Update and Draw the world
        void Update(double delta);

        void Draw();

        //Returns the hero object
        Hero* GetHero();

        //Returns the active subsection in the world
        SubSection* GetActiveSubSection();

        //Sets the active subsection in the world
        void SetActiveSubSection(SubSection* subSection);

        //Returns the size for each subsection in the world
        uvec2 GetSubSectionSize();

        //Returns the number of horizontal and vertical subsections in the world
        unsigned int GetNumberOfHorizontalSubSections();
        unsigned int GetNumberOfVerticalSubSections();
        unsigned int GetNumberOfSubSections();

        //Returns the size of a tile in each subsection in the world
        unsigned int GetTileSize();

        //Returns the number of horizontal and vertical tiles in each subsection in the world
        unsigned int GetNumberOfHorizontalTiles();
        unsigned int GetNumberOfVerticalTiles();
        unsigned int GetNumberOfTiles();

        //Conveniance methods to calculate a subsection's coordinates
        uvec2 GetSubSectionCoordinatesForIndex(unsigned int index);
        uvec2 GetSubSectionCoordinatesForPosition(vec2 position);
        uvec2 GetSubSectionCoordinatesForSubSection(SubSection* subSection);
        uvec2 GetSubSectionCoordinatesForPlayer(Player* player);

        //Conveniance methods to calculate a subsection's index
        unsigned int GetSubSectionIndexForPosition(vec2 position);
        unsigned int GetSubSectionIndexForCoordinates(uvec2 coordinates);
        unsigned int GetSubSectionIndexForSubSection(SubSection* subSection);
        unsigned int GetSubSectionIndexForPlayer(Player* player);

        //Conveniance methods to calculate a subsection's address
        SubSection* GetSubSectionForIndex(unsigned int index);
        SubSection* GetSubSectionForPosition(vec2 position);
        SubSection* GetSubSectionForCoordinates(uvec2 coordinates);
        SubSection* GetSubSectionForPlayer(Player* player);

        //Methods to play and pause music
        void StartMusic();
        void PauseMusic();
		void StopMusic();

    private:
        //Method to shift the camera in a given direction
        void ShiftCamera(ivec2 direction);

        //Delayed method to null the transition section pointer after a camera shift
        void NullTransitionSection();

        //Delayed method to enable the hero
        void EnableHero();

        //Member variables
        Hero* m_Hero;
        SubSection** m_SubSections;
        SubSection* m_ActiveSubSection;
        SubSection* m_TransitionSubSection;

        Audio* m_OverworldTheme;

        unsigned int m_LoadStep;
        unsigned int m_LoadSubSectionIndex;
    };
}

#endif /* defined(__GameDev2D__World__) */
