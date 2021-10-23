//
//  AudioExample.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-08-11.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#include "AudioExample.h"
#include "../../Source/Services/ServiceLocator.h"
#include "../../Source/Services/AudioManager/AudioManager.h"
#include "../../Source/Audio/Audio.h"
#include "../../Source/Events/Input/InputEvents.h"



namespace GameDev2D
{
    AudioExample::AudioExample() : Scene("AudioExample"),
        m_Music(nullptr),
        m_SoundEffect(nullptr)
    {

    }
    
    AudioExample::~AudioExample()
    {
        //Delete the Audio object
        SafeDelete(m_Music);
        SafeDelete(m_SoundEffect);
    }
    
    float AudioExample::LoadContent()
    {
        //Create a new 'streamed' Audio object that loops. Usually larger audio files
        //(usually used for background music) are streamed because they take up are
        //large and take up a lot of memory
        m_Music = new Audio("OverworldTheme", "mp3", true, true);
        
        //Create a new Audio object that is NOT streamed and does NOT loop.
        m_SoundEffect = new Audio("Secret", "wav", false, false);
        
        //Return 1.0f when loading has completed
        return 1.0f;
    }
    
    void AudioExample::HandleEvent(Event* aEvent)
    {
        switch (aEvent->GetEventCode())
        {
            case TOUCH_EVENT:
            {
                TouchEvent* touchEvent = (TouchEvent*)aEvent;
                if(touchEvent->GetTouchType() == TouchBegan)
                {
                    m_Music->Play();
                }
                else if(touchEvent->GetTouchType() == TouchEnded)
                {
                    m_SoundEffect->Play();
                }
            }
            break;
        
            case KEYBOARD_EVENT:
            {
                KeyEvent* keyEvent = (KeyEvent*)aEvent;
                if(keyEvent->GetKeyEventType() == KeyUp)
                {
                    //When the Space key is pressed play the sound effect Audio
                    if(keyEvent->GetKeyCode() == KEY_CODE_SPACE)
                    {
                        m_SoundEffect->Play();
                    }
                    
                    //When the Tab key is pressed play the music Audio
                    else if(keyEvent->GetKeyCode() == KEY_CODE_TAB)
                    {
                        m_Music->Play();
                    }
                    
                    //When the Left Shift key is pressed pause both the sound effect AND music Audio
                    else if(keyEvent->GetKeyCode() == KEY_CODE_LEFT_SHIFT)
                    {
                        m_SoundEffect->Pause();
                        m_Music->Pause();
                    }
                    
                    //When the Right Shift key is pressed pause both the sound effect AND music Audio
                    else if(keyEvent->GetKeyCode() == KEY_CODE_RIGHT_SHIFT)
                    {
                        m_SoundEffect->Stop();
                        m_Music->Stop();
                    }
                }
            }
            break;
                
            default:
                break;
        }
    }
}