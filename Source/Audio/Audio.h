//
//  Audio.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-04-30.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__Audio__
#define __GameDev2D__Audio__

#include "FrameworkConfig.h"
#include "../Core/BaseObject.h"
#include "../Events/EventDispatcher.h"
#include "../Animation/Tween.h"
#include "../Animation/Timer.h"
#include <fmod.hpp>


namespace GameDev2D
{
    //Audio class to handle playback of both music and sounds effects in game.
    //This class dispatches the following events:
    // AUDIO_PLAY_EVENT
    // AUDIO_PAUSE_EVENT
    // AUDIO_STOP_EVENT
    // AUDIO_PLAYBACK_FINISHED_EVENT
    // AUDIO_CHANNEL_INVALIDATED_EVENT
    //The EventData* contains a pointer to the Audio* that disptached the events
    class Audio : public BaseObject, public EventDispatcher
    {
    public:
        //Constructor for the Audio class, many extensions are supported, use 'streamed' for longer files such as background music.
        //Don't stream for short files, such as sound effects. Audio files be looped.
        Audio(const string& filename, const string& extension, bool streamed, bool looped);
        ~Audio();

        //Returns the key (filename + extension) of the audio file
        const char* GetKey();

        //Play, Pause and Stop methods, can be delayed (in seconds), by default there is no delay
        void Play(double delay = 0.0);
        void Pause(double delay = 0.0);
        void Stop(double delay = 0.0);
        
        //Returns wether the audio is playing
        bool IsPlaying();
        
        //Set the frequency of the audio file, can be applied over time, it can have an easing function applied to it, reversed and repeated
        void SetFrequency(float frequency, double duration = 0.0, EasingFunction easingFunction = Linear::Interpolation, bool reverse = false, int repeatCount = 0);
        
        //Returns the current frequency of the audio file, must be positive
        float GetFrequency();
        
        //Set the volume of the audio file, it can be applied over time, it can have an easing function applied to it, reversed and repeated
        void SetVolume(float volume, double duration = 0.0, EasingFunction easingFunction = Linear::Interpolation, bool reverse = false, int repeatCount = 0);
        
        //Returns the current volume of the audio file, range 0.0f to 1.0f
        float GetVolume();
        
        //Set the pan of the audio file, it can be applied over time, it can have an easing function applied to it, reversed and repeated
        void SetPan(float pan, double duration = 0.0, EasingFunction easingFunction = Linear::Interpolation, bool reverse = false, int repeatCount = 0);
        
        //Returns the current pan of the audio file, range -1.0f to 1.0f
        float GetPan();
        
        //Sets the priority of the audio file
        void SetPriority(int priority);
        
        //Returns the audio file's priority
        int GetPriority();
        
        //Sets the current position of the audi0 file, in milleseconds
        void SetPosition(unsigned int position);
        
        //Returns the current time of the audio file, in milleseconds
        unsigned int GetPosition();
        
        //Returns the remaining time of the audio file, in milleseconds
        unsigned int GetRemaining();
        
        //Returns the total length of the audio file, in milleseconds
        unsigned int GetLength();
        
        //Invalidates the current audio channel, this stops all playback,
        //if you need to stop the audio, use the Stop() method instead,
        //this is called when the playback ends OR when the channel is
        //needed by a higher priority audio file
        void InvalidateChannel();
       
    protected:
        //Called by the AudioManager to ensure the Delays and Tweens are updated properly
        void Update(double delta);
        
        //Function pointer and conveniance method for tweens
        typedef void (Audio::*AudioTweenSetMethod)(float value, double duration, EasingFunction easingFunction, bool reverse, int repeatCount);
        void UpdateTween(double delta, Tween* tween, AudioTweenSetMethod tweenSetMethod);
        
        //Function pointer and conveniance method for delay timers
        typedef void (Audio::*AudioDelayMethod)(double delay);
        void UpdateDelay(double delta, Timer* timer, AudioDelayMethod delayMethod);
        
        //Allows the AudioManager to call the Update() method
        friend class AudioManager;
        
    private:
        //Member variables
        FMOD::System* m_System;
        FMOD::Sound* m_Sound;
        FMOD::Channel* m_Channel;
        float m_Frequency;
        float m_Volume;
        float m_Pan;
        int m_Priority;
        unsigned int m_Position;
        unsigned int m_Length;
        bool m_IsStreamed;
        string m_Key;
        
        //Play, pause and stop delay timers
        Timer m_DelayPlay;
        Timer m_DelayPause;
        Timer m_DelayStop;
        
        //Frequency, volume and pan tweens
        Tween m_TweenFrequency;
        Tween m_TweenVolume;
        Tween m_TweenPan;
    };
}

#endif /* defined(__GameDev2D__Audio__) */
