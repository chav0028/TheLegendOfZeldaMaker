//
//  File.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2013-07-17.
//  Copyright (c) 2013 Algonquin College. All rights reserved.
//


#include "File.h"
#include "../Services/ServiceLocator.h"
#include "../Platforms/PlatformLayer.h"


namespace GameDev2D
{
    File::File(const string& aPath) : BaseObject("File")
    {        
        //Check to see if the file exists, if it doesn't the assert below will be hit
        bool doesExist = ServiceLocator::GetPlatformLayer()->DoesFileExistAtPath(aPath);
        assert(doesExist == true);
        
        //Does the file exist
        if(doesExist == true)
        {
            //Open the input stream for the file
            ifstream inputFile(aPath.c_str());
            
            //Read line by line the contents of the file,
            //while there is still content to load
            while(inputFile.good() == true)
            {
                string line;
                getline(inputFile, line);
                m_FileText.append(line + "\n");
            }
        }
    }
    
    File::~File()
    {
        
    }
    
    const char* File::GetBuffer()
    {
        return m_FileText.c_str();
    }
    
    unsigned long File::GetBufferSize()
    {
        return m_FileText.length();
    }
}
