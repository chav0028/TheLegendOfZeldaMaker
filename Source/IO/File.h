//
//  File.h
//  GameDev2D
//
//  Created by Bradley Flood on 2013-07-17.
//  Copyright (c) 2013 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__File__
#define __GameDev2D__File__

#include "../Core/BaseObject.h"


using namespace std;

namespace GameDev2D
{
    //The File class will attempt to open a file for the desired
    //path and load the contents into a buffer. The contents of
    //the buffer are destroyed when this File object is.
    class File : public BaseObject
    {
    public:
        //The contructor will take a path parameter and attempt to
        //load the file at the path, it will throw an assert if
        //the file can't be opened or doesn't exist
        File(const string& path);
        virtual ~File();
        
        //Returns a pointer to the buffer
        const char* GetBuffer();
        
        //Returns the size of the buffer
        unsigned long GetBufferSize();
        
    private:
        //Member variable
        string m_FileText;
    };
}

#endif /* defined(__GameDev2D__File__) */
