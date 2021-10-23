    //
//  Audio.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-04-30.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#include "Audio.h"
#include "../Services/AudioManager/AudioManager.h"
#include "../Services/ServiceLocator.h"
#include "../Events/Event.h"


namespace GameDev2D
{
    FMOD_RESULT F_CALLBACK ChannelCallback(FMOD_CHANNEL* channel, FMOD_CHANNEL_CALLBACKTYPE type, void* commanddata1, void* commanddata2);


    Audio::Audio(const string& aFilename, const string& aExtension, bool aStreamed, bool aLooped) : BaseObject("Audio"),
        m_System(nullptr),
        m_Sound(nullptr),
        m_Channel(nullptr),
        m_Frequency(0),
        m_Volume(0.0f),
        m_Pan(0.0f),
        m_Priority(0),
        m_Position(0),
        m_Length(0),
        m_IsStreamed(aStreamed),
        m_Key(""),
        m_DelayPlay(),
        m_DelayPause(),
        m_DelayStop(),
        m_TweenFrequency(),
        m_TweenVolume(),
        m_TweenPan()
    {
        //Load the sound
        m_Sound = ServiceLocator::GetAudioManager()->LoadSound(&m_System, aFilename, aExtension, aStreamed, aLooped);
    
        //Set the audio key
        m_Key = aFilename + "." + aExtension;
    
        //Safety check the sound object
        if(m_Sound != nullptr)
        {
            //Register the audio object with the audio manager
            ServiceLocator::GetAudioManager()->RegisterAudio(this);
        
            //Get the length and default properties of the audio file
            m_Sound->getLength(&m_Length, FMOD_TIMEUNIT_MS);
            m_Sound->getDefaults(&m_Frequency, &m_Volume, &m_Pan, &m_Priority);
            
            //Format the length of the audio string
            stringstream lengthStream;
            if(m_Length > 1000.0f)
            {
                lengthStream << setprecision(4) << static_cast<float>(m_Length) / 1000.0f << " seconds";
            }
            else if(m_Length > 1000.0f * 60.0f)
            {
                lengthStream << setprecision(4) << static_cast<float>(m_Length) / (1000.0f * 60.0f) << " minutes";
            }
            else
            {
                lengthStream << m_Length << " mille-seconds";
            }
            
            //Log that the audio file was loaded as well as the length and frequency
            Log(VerbosityLevel_Audio, "Loaded: %s, length: %s, frequency: %.2f", m_Key.c_str(), lengthStream.str().c_str(), m_Frequency);
        }
    }
    
    Audio::~Audio()
    {
        //Unregister the audio object from the audio manager
        ServiceLocator::GetAudioManager()->UnregisterAudio(this);
    
        //Safety check, then unload the sound
        if(m_Sound != nullptr)
        {
            ServiceLocator::GetAudioManager()->UnloadSound(m_Sound, m_IsStreamed == true ? "" : m_Key);
            m_Sound = nullptr;
        }
        
        //Set the system to null
        m_System = nullptr;
    }
    
    void Audio::Update(double aDelta)
    {
        //Update the tweens
        UpdateTween(aDelta, &m_TweenFrequency, &Audio::SetFrequency);
        UpdateTween(aDelta, &m_TweenVolume, &Audio::SetVolume);
        UpdateTween(aDelta, &m_TweenPan, &Audio::SetPan);
        
        //Update the delay timers
        UpdateDelay(aDelta, &m_DelayPlay, &Audio::Play);
        UpdateDelay(aDelta, &m_DelayPause, &Audio::Pause);
        UpdateDelay(aDelta, &m_DelayStop, &Audio::Stop);
    }
    
    void Audio::UpdateTween(double aDelta, Tween* aTween, AudioTweenSetMethod aTweenSetMethod)
    {
        //Safety check the Tween and make sure it is running
        if(aTween != nullptr && aTween->IsRunning() == true)
        {
            //Update the Tween
            aTween->Update(aDelta);
            
            //Using the function pointer, call the set method
            //and pass the Tween's current value as the parameter
            (this->*aTweenSetMethod)(aTween->GetCurrent(), 0.0, nullptr, false, 0);
        }
    }
    
    void Audio::UpdateDelay(double aDelta, Timer* aTimer, AudioDelayMethod aDelayMethod)
    {
        //Safety check the Timer and make sure it is running
        if(aTimer != nullptr && aTimer->IsRunning() == true)
        {
            //Update the Timer
            aTimer->Update(aDelta);
            
            //Has the Timer completed, if it has call the method
            if(aTimer->GetPercentageComplete() == 1.0f)
            {
                (this->*aDelayMethod)(0.0);
            }
        }
    }
    
    const char* Audio::GetKey()
    {
        return m_Key.c_str();
    }
    
    void Audio::Play(double aDelay)
    {
        //Is there a delay
        if(aDelay == 0.0)
        {
            //Is the channel null?
            if(m_Channel == nullptr)
            {
                //Safety check the system
                if(m_System != nullptr)
                {
                    //Play the sound, and check for an error
                    if(m_System->playSound(FMOD_CHANNEL_FREE, m_Sound, true, &m_Channel) != FMOD_OK)
                    {
                        Error(false, "Failed to play audio %s", m_Key.c_str());
                    }
                    
                    //Set the channel's user data and call back function
                    m_Channel->setUserData(this);
                    m_Channel->setCallback(&ChannelCallback);
                }
            }
            
            //Safety check the channel
            if(m_Channel != nullptr)
            {
                bool isPaused = false;
                m_Channel->getPaused(&isPaused);
                
                //Set the volume
                m_Channel->setVolume(m_Volume);
            
                //Is the audio paused
                if(isPaused == false)
                {
                    //Set the channel's frequency, volume, pan, position and priority settings
                    m_Channel->setFrequency(m_Frequency);
                    m_Channel->setPan(m_Pan);
                    m_Channel->setPriority(m_Priority);
                    m_Channel->setPosition(m_Position, FMOD_TIMEUNIT_MS);
                }
                
                //Play the audio
                m_Channel->setPaused(false);
                
                //Dispatch an audio play event
                DispatchEvent(new Event(AUDIO_PLAY_EVENT, this, VerbosityLevel_Audio));
            }
        }
        else
        {
            //Setup the delay
            m_DelayPlay.SetDuration(aDelay);
            m_DelayPlay.Reset(true);
        }
    }
    
    void Audio::Pause(double aDelay)
    {
        //Is there a delay?
        if(aDelay == 0.0)
        {
            //Safety check the channel isn't null
            if(m_Channel != nullptr)
            {
                //Pause the audio channel
                m_Channel->setPaused(true);
                
                //Dispatch an audio pause event
                DispatchEvent(new Event(AUDIO_PAUSE_EVENT, this, VerbosityLevel_Audio));
            }
        }
        else
        {
            //Setup the delay
            m_DelayPause.SetDuration(aDelay);
            m_DelayPause.Reset(true);
        }
    }
    
    void Audio::Stop(double aDelay)
    {
        //Is there a delay
        if(aDelay == 0.0)
        {
            //Safety check the channel
            if(m_Channel != nullptr)
            {
                //Stop the audio channel
                m_Channel->stop();
                
                //Dispatch an audio stopped event
                DispatchEvent(new Event(AUDIO_STOP_EVENT, this, VerbosityLevel_Audio));
            }
        }
        else
        {
            //Setup the delay
            m_DelayStop.SetDuration(aDelay);
            m_DelayStop.Reset(true);
        }
    }
    
    bool Audio::IsPlaying()
    {
        //Safety check the channel
        if(m_Channel != nullptr)
        {
            //Determine if the channel playing?
            bool isPlaying = false;
            m_Channel->isPlaying(&isPlaying);
            return isPlaying;
        }
        return false;
    }
    
    void Audio::SetFrequency(float aFrequency, double aDuration, EasingFunction aEasingFunction, bool aReverse, int aRepeatCount)
    {
        
        if(aDuration == 0.0)
        {
            //Bounds check the frequency, it can't be negative
            m_Frequency = fmaxf(aFrequency, 0.0f);
            
            //Log the frequency change
            Log(VerbosityLevel_Audio, "%s - SetFrequency(%f)", m_Key.c_str(), m_Frequency);
            
            //Safety check the channel and set the channel's frequency
            if(m_Channel != nullptr)
            {
                m_Channel->setFrequency(m_Frequency);
            }
        }
        else
        {
            //Setup the frequency tween
            m_TweenFrequency.Set(m_Frequency, fmaxf(aFrequency, 0.0f), aDuration, aEasingFunction, aReverse, aRepeatCount);
        }
    }
    
    float Audio::GetFrequency()
    {
        return m_Frequency;
    }
    
    void Audio::SetVolume(float aVolume, double aDuration, EasingFunction aEasingFunction, bool aReverse, int aRepeatCount)
    {
        if(aDuration == 0.0)
        {
            //Bounds check the volume
            m_Volume = fminf(fmaxf(aVolume, 0.0f), 1.0f);
            
            //Log the volume change
            Log(VerbosityLevel_Audio, "%s - SetVolume(%f)", m_Key.c_str(), m_Volume);
            
            //Safety check the channel and set the volume
            if(m_Channel != nullptr)
            {
                m_Channel->setVolume(m_Volume);
            }
        }
        else
        {
            //Setup the volume tween
            m_TweenVolume.Set(m_Volume, fminf(fmaxf(aVolume, 0.0f), 1.0f), aDuration, aEasingFunction, aReverse, aRepeatCount);
        }
    }
    
    float Audio::GetVolume()
    {
        return m_Volume;
    }
    
    void Audio::SetPan(float aPan, double aDuration, EasingFunction aEasingFunction, bool aReverse, int aRepeatCount)
    {
        if(aDuration == 0.0)
        {
            //Bounds check the pan
            m_Pan = fminf(fmaxf(aPan, -1.0f), 1.0f);
            
            //Log the pan change
            Log(VerbosityLevel_Audio, "%s - SetPan(%f)", m_Key.c_str(), m_Pan);
            
            //Safety check the channel and set the pan
            if(m_Channel != nullptr)
            {
                m_Channel->setPan(m_Pan);
            }
        }
        else
        {
            //Setup the pan tween
            m_TweenPan.Set(m_Pan, fminf(fmaxf(aPan, -1.0f), 1.0f), aDuration, aEasingFunction, aReverse, aRepeatCount);
        }
    }
    
    float Audio::GetPan()
    {
        return m_Pan;
    }
    
    void Audio::SetPriority(int aPriority)
    {
        //Set the priority
        m_Priority = aPriority;
        
        //Log the priority change
        Log(VerbosityLevel_Audio, "%s - SetPriority(%f)", m_Key.c_str(), m_Priority);
        
        //Safety check the channel and set the priority
        if(m_Channel != nullptr)
        {
            m_Channel->setPriority(m_Priority);
        }
    }
    
    int Audio::GetPriority()
    {
        return m_Priority;
    }
    
    void Audio::SetPosition(unsigned int aPosition)
    {
        //Bounds check the audio's position
        m_Position = aPosition < GetLength() ? aPosition : GetLength();
        
        //Log the position change
        Log(VerbosityLevel_Audio, "%s - SetPosition(%f)", m_Key.c_str(), m_Position);
        
        //Safety check the channel and set the position
        if(m_Channel != nullptr)
        {
            m_Channel->setPosition(aPosition, FMOD_TIMEUNIT_MS);
        }
    }
    
    unsigned int Audio::GetPosition()
    {
        //Safety check the channel and get the position
        if(m_Channel != nullptr)
        {
            m_Channel->getPosition(&m_Position, FMOD_TIMEUNIT_MS);
        }
        return m_Position;
    }
    
    unsigned int Audio::GetRemaining()
    {
        return GetLength() - GetPosition();
    }
        
    unsigned int Audio::GetLength()
    {
        return m_Length;
    }

    void Audio::InvalidateChannel()
    {
        //Set the channel to null
        m_Channel = nullptr;
        
        //Dispatch an audio channel invalidation event
        DispatchEvent(new Event(AUDIO_CHANNEL_INVALIDATED_EVENT, this, VerbosityLevel_Audio));
    }
    
    FMOD_RESULT F_CALLBACK ChannelCallback(FMOD_CHANNEL* aChannel, FMOD_CHANNEL_CALLBACKTYPE aType, void* aCommanddata1, void* aCommanddata2)
    {
        FMOD::Channel* channel = (FMOD::Channel *)aChannel;

        //Playback has reach the end
        if(aType == FMOD_CHANNEL_CALLBACKTYPE_END)
        {
            //Get the user data (audio*) from the channel
            void* userData;
            channel->getUserData(&userData);
            
            //Safety check the user data
            if(userData != nullptr)
            {
                //Dispatch an audio playback finished event and invalidate the channel
                ((Audio*)userData)->DispatchEvent(new Event(AUDIO_PLAYBACK_FINISHED_EVENT, userData, VerbosityLevel_Audio));
                ((Audio*)userData)->InvalidateChannel();
            }
        }

        return FMOD_OK;
    }
}