//
//  PhysicsEditorWrapper.m
//  GameDevFramework
//
//  Created by Bradley Flood on 2012-10-03.
//  Copyright (c) 2012 Algonquin College. All rights reserved.
//

#import "PhysicsEditorWrapper.h"
#import "GB2ShapeCache.h"


namespace PhysicsEditorCpp
{
  void addShapesFromPlist(const char* aFilename)
  {
    NSString *filename = [[NSString alloc] initWithCString:aFilename encoding:NSUTF8StringEncoding];
    [[GB2ShapeCache sharedShapeCache] addShapesWithFile:filename];
    [filename release];
  }
  
  void addFixturesToBody(b2Body* aBody, const char* aShape)
  {
    NSString *shape = [[NSString alloc] initWithCString:aShape encoding:NSUTF8StringEncoding];
    [[GB2ShapeCache sharedShapeCache] addFixturesToBody:aBody forShapeName:shape];
    [shape release];
  }
  
  void anchorPointForShape(float& aX, float& aY, const char* aShape)
  {
    NSString *shape = [[NSString alloc] initWithCString:aShape encoding:NSUTF8StringEncoding];
    CGPoint point = [[GB2ShapeCache sharedShapeCache] anchorPointForShape:shape];
    aX = point.x;
    aY = point.y;
    [shape release];
  }
}
