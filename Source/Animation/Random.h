//
//  Random.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-03-15.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__Random__
#define __GameDev2D__Random__

#include "FrameworkConfig.h"
#include "../Core/BaseObject.h"


#define CMATH_N 624
#define CMATH_M 397
#define CMATH_MATRIX_A 0x9908b0df   // constant vector a
#define CMATH_UPPER_MASK 0x80000000 // most significant w-r bits
#define CMATH_LOWER_MASK 0x7fffffff // least significant r bits

#define CMATH_TEMPERING_MASK_B 0x9d2c5680
#define CMATH_TEMPERING_MASK_C 0xefc60000
#define CMATH_TEMPERING_SHIFT_U(y)  (y >> 11)
#define CMATH_TEMPERING_SHIFT_S(y)  (y << 7)
#define CMATH_TEMPERING_SHIFT_T(y)  (y << 15)
#define CMATH_TEMPERING_SHIFT_L(y)  (y >> 18)

namespace GameDev2D
{
    //Random number generator, based on the Mersenne twister,
    //the seed value can be explicitly set OR randomized
    class Random : public BaseObject
    {
    public:
        Random();

        //Returns a random integer between 0 and n
        unsigned int RandomInt(unsigned int n);
        
        //Returns a random integer between min and max
        unsigned int RandomRange(unsigned int min, unsigned int max);
        
        //Returns a random float
        float RandomFloat();
        
        //Getter and setter values for the seed
        void SetSeed(unsigned int seed);
        unsigned int GetSeed();
        
        //Randomizes the seed value
        unsigned int RandomizeSeed();
        
    private:
        unsigned int m_Seed;
        unsigned long m_Mt[CMATH_N];
        int m_Index;
    };
}

#endif /* defined(__GameDev2D__Random__) */
