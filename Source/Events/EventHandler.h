//
//  EventHandler.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-03-05.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__EventHandler__
#define __GameDev2D__EventHandler__

#include "Event.h"


namespace GameDev2D
{
    //The EventHandler class is an interface to handle events that are being
    //dispatched by the EventDispatcher. Inherit from the EventHandler class
    //and implement the HandleEvent() method to receive Events.
	class EventHandler
    {
		public:
			EventHandler();
			virtual ~EventHandler();

            virtual void HandleEvent(Event* event);
	};
}

#endif /* defined(__GameDev2D__EventHandler__) */
