//
//  Entity.hpp
//  NYUCodebase
//
//  Created by Derek Yu on 11/15/16.
//  Copyright © 2016 Ivan Safrin. All rights reserved.
//

#ifndef Entity_hpp
#define Entity_hpp

#include <stdio.h>
#include "SheetSprite.hpp"
//#include "SheetSprite.hpp"

enum EntityType {
    ENTITY_PLAYER,
    ENTITY_ENEMY,
    ENTITY_BLOCK
};


class Entity{
public:
public:
    Entity();
    
    void Update(float elapsed);
    void Render(ShaderProgram *program);
    bool collidesWith(Entity *entity);
    
    SheetSprite sprite;
    
    
    float x;
    float y;
    float width;
    float height;
    float velocity_x=0;
    float velocity_y=0;
    float acceleration_x;
    float acceleration_y=-9.8;
    Matrix modelMatrix;
    
    bool isStatic;
    EntityType entityType;
    
    bool collidedTop;
    bool collidedBottom;
    bool collidedLeft;
    bool collidedRight;
    float elapsedTime=0;
    float elapsedTimeTotal = 0;
    bool dead = false;
    
    
};


#endif /* Entity_hpp */

