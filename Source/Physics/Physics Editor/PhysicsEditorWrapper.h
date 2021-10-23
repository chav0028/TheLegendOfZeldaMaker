//
//  PhysicsEditorWrapper.h
//  GameDevFramework
//
//  Created by Bradley Flood on 2012-10-03.
//  Copyright (c) 2012 Algonquin College. All rights reserved.
//

#ifndef PHYSICS_EDITOR_WRAPPER_H
#define PHYSICS_EDITOR_WRAPPER_H

#include "Box2D.h"

namespace PhysicsEditorCpp
{
  void addShapesFromPlist(const char* filename);
  void addFixturesToBody(b2Body* body, const char* shape);
  void anchorPointForShape(float& x, float& y, const char* shape);
}

#endif