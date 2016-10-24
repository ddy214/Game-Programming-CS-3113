//
//  Entity.hpp
//  NYUCodebase
//
//  Created by Derek Yu on 10/19/16.
//  Copyright © 2016 Ivan Safrin. All rights reserved.
//

#ifndef Entity_hpp
#define Entity_hpp

#include <stdio.h>
#include "ShaderProgram.h"
#include "SheetSprite.hpp"
#include "Matrix.h"

enum EntityType {ALIEN, PLAYER, ALIEN_MISSILE, PLAYER_MISSILE};


class Entity {
public:
    Entity();
    
    Entity(float x, float y, const SheetSprite& sheet, int entity_type);
    
    void Update(float elapsed);
    
    void Render(ShaderProgram *program);
    
    bool collidesWith(Entity * entity);
    
    SheetSprite sprite;
    
    float width;
    float height;
    float x;
    float y;
    float velocity_x=0;
    float velocity_y=0;
    Matrix modelMatrix;
    int entity_type;
    float totalTimeElapsed=0;
    bool collidedTop;
    bool collidedBottom;
    bool collidedLeft;
    bool collidedRight;
    
};



#endif /* Entity_hpp */
