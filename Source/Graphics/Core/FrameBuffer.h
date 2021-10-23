//
//  FrameBuffer.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-03-15.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef GameDev2D_FrameBuffer_h
#define GameDev2D_FrameBuffer_h

namespace GameDev2D
{
    typedef struct FrameBuffer
    {
        unsigned int frameBuffer;
        unsigned int renderBuffer;
        int width;
        int height;
    }
    FrameBuffer;
}

#endif
