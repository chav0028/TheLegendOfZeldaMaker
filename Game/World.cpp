//
//  World.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-10-14.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

/*
ChangeLog
_______________________________________________
-In LoadContent, case WorldLoadSubsections, modified the string being read to include
the ""SaveData" path, so that it reads the files directly from our desired folder.THIS REQUIRED
MODIFYING THE PROJECT PROERTIES, WORKING DIRECTORY TO BE THE ROOT OF THE PROJECT "../../../"
-Added background looping music
-Made stop music function
*/

#include "World.h"
#include "SubSection.h"
#include "Game.h"
#include "Player/Hero.h"
#include "../Source/Services/ServiceLocator.h"
#include "../Source/Platforms/PlatformLayer.h"
#include "../Source/Graphics/Core/Camera.h"
#include "../Source/Audio/Audio.h"
#include "../Source/Events/Event.h"
#include "../Source/Events/Input/KeyEvent.h"

namespace GameDev2D
{
    World::World() : Scene("World"),
        m_Hero(nullptr),
        m_SubSections(nullptr),
        m_ActiveSubSection(nullptr),
        m_TransitionSubSection(nullptr),
        m_OverworldTheme(nullptr),
        m_LoadStep(0),
        m_LoadSubSectionIndex(0)
    {

    }

    World::~World()
    {
        //Safety check the subsections pointer
        if (m_SubSections != nullptr)
        {
            //Cycle through and delete each subsection object in the subsections array
            for (unsigned int i = 0; i < GetNumberOfSubSections(); i++)
            {
                SafeDelete(m_SubSections[i]);
            }

            //Delete subsection array
            SafeDeleteArray(m_SubSections);
        }

        //Delete the hero object
        SafeDelete(m_Hero);

        //Delete the overworld music
        SafeDelete(m_OverworldTheme);
    }

    float World::LoadContent()
    {
        //Flag to handle loading the same load step multiple times
        bool incrementLoadStep = m_LoadStep != WorldLoadSubSections;

        //Load the current load step
        switch (m_LoadStep)
        {
        case WorldLoadRequired:
        {
            //Create the subsections array
            m_SubSections = new SubSection*[GetNumberOfSubSections()];
        }
        break;

        case WorldLoadSubSections:
        {
            //Calculate the coordintes for the current load index
            uvec2 coordinates = GetSubSectionCoordinatesForIndex(m_LoadSubSectionIndex);

            //Create the subsection object for the current load index
            m_SubSections[m_LoadSubSectionIndex] = new SubSection(this, coordinates);

            //Format the filename for the subsection to load
            stringstream ss;
            ss << "SaveData/SubSection" << coordinates.x << "-" << coordinates.y << ".bin";

            //Load the subsection
            m_SubSections[m_LoadSubSectionIndex]->Load(ss.str());

            //Increment load index
            m_LoadSubSectionIndex++;

            //Have all the sub sections been loaded?
            if (m_LoadSubSectionIndex == GetNumberOfSubSections())
            {
                incrementLoadStep = true;
            }
        }
        break;

        case WorldLoadHero:
        {
            //Local variables
            unsigned int subSectionIndex = 0;
            unsigned int tileIndex = 0;
            bool spawnPointFound = false;

            //Cycle through the subsection and find a hero spawn point
            for (unsigned int i = 0; i < GetNumberOfSubSections(); i++)
            {
                for (unsigned int j = 0; j < GetNumberOfTiles(); j++)
                {
                    //If a spawn point was found, break out of the inner for loop
                    if (m_SubSections[i]->GetTileForIndex(j)->IsHeroSpawnPoint() == true)
                    {
                        spawnPointFound = true;
                        subSectionIndex = i;
                        tileIndex = j;
                        break;
                    }
                }

                //Was a spawn point found, break out of the outer for loop
                if (spawnPointFound == true)
                {
                    break;
                }
            }

            //Get the subsection and tile to spawn the player at
            SubSection* subSection = GetSubSectionForIndex(subSectionIndex);
            Tile* tile = subSection->GetTileForIndex(tileIndex);

            //Create the hero object
            m_Hero = new Hero(this, tile);
            m_Hero->SetIsEnabled(false);

            //Set the active sub section based on the Hero's location
            SetActiveSubSection(subSection);

            //Set the camera's position to the correct subsection
            GetCamera()->SetPosition(-subSection->GetWorldPosition());
        }
        break;

        case WorldLoadMusic:
        {
            m_OverworldTheme = new Audio("OverworldTheme", "mp3", true, true);//Set the background music
            m_OverworldTheme->SetVolume(WORLD_OVERWORLD_THEME_SOUND);//Set the volume
        }
        break;

        case WorldLoadFinal:
        {
            //Setup a delayed method call to start the game overworld music
            DelaySceneMethod(&World::EnableHero, GAME_INTRO_DURATION);
            DelaySceneMethod(&World::StartMusic, GAME_INTRO_DURATION);//Start playing the music
        }
        break;

        default:
            break;
        }

        //Increment the load step
        if (incrementLoadStep == true)
        {
            m_LoadStep++;
        }

        //Calculate the percentage that has loaded
        float percentageLoaded = (float)(m_LoadStep + m_LoadSubSectionIndex) / (float)(GetNumberOfSubSections() + WorldLoadStepCount);

        //Return the percentage loaded
        return percentageLoaded;
    }

    void World::Update(double aDelta)
    {
        //Safety check the active sub-section and update it
        if (m_ActiveSubSection != nullptr)
        {
            m_ActiveSubSection->Update(aDelta);
        }

        //Safety check the transition sub-section and update it
        if (m_TransitionSubSection != nullptr)
        {
            m_TransitionSubSection->Update(aDelta);
        }

        //Safety check the hero
        if (m_Hero != nullptr && m_Hero->IsEnabled() == true)
        {
            //Get the sub section the hero is on before we update the hero
            uvec2 previousSubSection = GetSubSectionCoordinatesForPlayer(m_Hero);

            //Update the hero
            m_Hero->Update(aDelta);

            //Get the sub section the hero is on after we update the hero
            uvec2 currentSubSection = GetSubSectionCoordinatesForPlayer(m_Hero);

            //Did the hero change sub sections during the update call
            if (previousSubSection != currentSubSection)
            {
                //Notify the hero that they have changed subsections
                m_Hero->HasChangedSubSections(GetSubSectionForCoordinates(currentSubSection), GetSubSectionForCoordinates(previousSubSection));

                //Shift the camera
                ShiftCamera(ivec2(previousSubSection - currentSubSection));

                //Is the section debug drawing
                unsigned int debugFlags = m_ActiveSubSection->GetDebugDrawFlags();

                //Set the transition section to the active section
                m_TransitionSubSection = m_ActiveSubSection;
                m_TransitionSubSection->DisableDebugDrawing();

                //Set the new active section
                unsigned int index = GetSubSectionIndexForCoordinates(currentSubSection);
                SetActiveSubSection(m_SubSections[index]);

                //Set the debug flags to the new sub section
                m_ActiveSubSection->EnableDebugDrawing(debugFlags);

                //Setup a delay method to null the transition section pointer in one second
                DelaySceneMethod(&World::NullTransitionSection, WORLD_CAMERA_SHIFT_DURATION);
            }
        }

        //Update the Scene's GameObjects
        Scene::Update(aDelta);

    }

    void World::Draw()
    {
        //Safety check the active sub-section and draw it
        if (m_ActiveSubSection != nullptr)
        {
            m_ActiveSubSection->Draw();
        }

        //Safety check the transition sub-section and draw it
        if (m_TransitionSubSection != nullptr)
        {
            m_TransitionSubSection->Draw();
        }

        //Safety check and draw the hero
        if (m_Hero != nullptr && m_Hero->IsEnabled() == true)
        {
            m_Hero->Draw();
        }

        //Draw the Scene's GameObjects
        Scene::Draw();
    }

    Hero* World::GetHero()
    {
        return m_Hero;
    }

    SubSection* World::GetActiveSubSection()
    {
        return m_ActiveSubSection;
    }

    void World::SetActiveSubSection(SubSection* aSubSection)
    {
        //Set the new active section
        m_ActiveSubSection = aSubSection;

        //Set the isActive flag to true
        m_ActiveSubSection->SetIsEnabled(true);
    }

    uvec2 World::GetSubSectionSize()
    {
        return uvec2(GetNumberOfHorizontalTiles() * GetTileSize(), GetNumberOfVerticalTiles() * GetTileSize());
    }

    unsigned int World::GetNumberOfHorizontalSubSections()
    {
        return WORLD_NUMBER_OF_SUBSECTIONS.x;
    }

    unsigned int World::GetNumberOfVerticalSubSections()
    {
        return WORLD_NUMBER_OF_SUBSECTIONS.y;
    }

    unsigned int World::GetNumberOfSubSections()
    {
        return GetNumberOfHorizontalSubSections() * GetNumberOfVerticalSubSections();
    }

    unsigned int World::GetTileSize()
    {
        return WORLD_TILE_SIZE;
    }

    unsigned int World::GetNumberOfHorizontalTiles()
    {
        return WORLD_NUMBER_OF_HORIZONTAL_TILES;
    }

    unsigned int World::GetNumberOfVerticalTiles()
    {
        return WORLD_NUMBER_OF_VERTICAL_TILES;
    }

    unsigned int World::GetNumberOfTiles()
    {
        return GetNumberOfHorizontalTiles() * GetNumberOfVerticalTiles();
    }

    uvec2 World::GetSubSectionCoordinatesForIndex(unsigned int aIndex)
    {
        //If this assert is hit, the index you passed in is out of bounds
        assert(aIndex < GetNumberOfSubSections() || aIndex == 50);

        //Calculate the coordinates and set the tile position and size
        uvec2 coordinates = uvec2(0, 0);
        coordinates.x = (aIndex % GetNumberOfHorizontalSubSections());
        coordinates.y = ((aIndex - coordinates.x) / GetNumberOfHorizontalSubSections());

        return coordinates;
    }

    uvec2 World::GetSubSectionCoordinatesForPosition(vec2 aPosition)
    {
        uvec2 coordinates = uvec2(aPosition.x / GetSubSectionSize().x, aPosition.y / GetSubSectionSize().y);
        return coordinates;
    }

    uvec2 World::GetSubSectionCoordinatesForSubSection(SubSection* aSubSection)
    {
        return GetSubSectionCoordinatesForIndex(GetSubSectionIndexForSubSection(aSubSection));
    }

    uvec2 World::GetSubSectionCoordinatesForPlayer(Player* aPlayer)
    {
        return GetSubSectionCoordinatesForIndex(GetSubSectionIndexForPlayer(aPlayer));
    }

    unsigned int World::GetSubSectionIndexForPosition(vec2 aPosition)
    {
        uvec2 coordinates = GetSubSectionCoordinatesForPosition(aPosition);
        return GetSubSectionIndexForCoordinates(coordinates);
    }

    unsigned int World::GetSubSectionIndexForCoordinates(uvec2 aCoordinates)
    {
        return aCoordinates.x + (aCoordinates.y * GetNumberOfHorizontalSubSections());
    }

    unsigned int World::GetSubSectionIndexForSubSection(SubSection* aSubSection)
    {
        return GetSubSectionIndexForPosition(aSubSection->GetLocalPosition());
    }

    unsigned int World::GetSubSectionIndexForPlayer(Player* aPlayer)
    {
        return GetSubSectionIndexForPosition(aPlayer->GetWorldPosition());
    }

    SubSection* World::GetSubSectionForIndex(unsigned int aIndex)
    {
        //If this assert is hit, the index that was passed in is out of bounds
        assert(aIndex < GetNumberOfSubSections());

        //Return the tile at the index
        return m_SubSections[aIndex];
    }

    SubSection* World::GetSubSectionForPosition(vec2 aPosition)
    {
        return GetSubSectionForIndex(GetSubSectionIndexForPosition(aPosition));
    }

    SubSection* World::GetSubSectionForCoordinates(uvec2 aCoordinates)
    {
        return GetSubSectionForIndex(GetSubSectionIndexForCoordinates(aCoordinates));
    }

    SubSection* World::GetSubSectionForPlayer(Player* aPlayer)
    {
        return GetSubSectionForPosition(aPlayer->GetWorldPosition());
    }

    void World::ShiftCamera(ivec2 aDirection)
    {
        //Calculate the camera's
        vec2 position = -GetActiveSubSection()->GetWorldPosition() + vec2((float)GetSubSectionSize().x * aDirection.x, (float)GetSubSectionSize().y * aDirection.y);

        //Shift the camera
        GetCamera()->SetPosition(position, WORLD_CAMERA_SHIFT_DURATION);
    }

    void World::NullTransitionSection()
    {
        if (m_TransitionSubSection != nullptr)
        {
            m_TransitionSubSection->SetIsEnabled(false);
        }
        m_TransitionSubSection = nullptr;
    }

    void World::EnableHero()
    {
        m_Hero->SetIsEnabled(true);
    }

    void World::StartMusic()
    {
		if (m_OverworldTheme != nullptr)
		{
			m_OverworldTheme->Play();
		}
    }

    void World::PauseMusic()
    {
		if (m_OverworldTheme != nullptr)
		{
			m_OverworldTheme->Pause();
		}
    }

	//Used to completely stop the music (called when exiting the game screen)
	void World::StopMusic()
	{
		if (m_OverworldTheme != nullptr)
		{
			m_OverworldTheme->Stop();
		}
	}
}