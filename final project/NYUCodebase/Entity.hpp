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
#include <map>
#include <vector>
#include "HitBox.hpp"
#include "Life.hpp"

enum EntityType {
    ENTITY_PLAYER,
    ENTITY_ENEMY,
    ENTITY_BLOCK
};

enum ActionType {
    NO_ATTACK,
    PUNCH,
    BLOCK,
    KICK,
    MOVING,
    MOVING_B,
    JUMPING,
    HIT
};

class Entity{
public:
public:
    Entity();
    
    void Update(float elapsed);
    void Render(ShaderProgram *program);
    bool collidesWith(Entity *entity);
    
    SheetSprite sprite;
    HitBox * hitBox=nullptr;
    int health=100;
    float x=0;
    float y=0;
    float width=0;
    float height=0;
    float velocity_x=0;
    float velocity_y=0;
    float acceleration_x=0;
    float acceleration_y=-9.8;
    Matrix modelMatrix;
    ActionType action=NO_ATTACK;
    bool isStatic;
    EntityType entityType;
    int direction = 1;
    Life * life;
    bool collidedTop=false;
    bool collidedBottom=false;
    bool collidedLeft=false;
    bool collidedRight=false;
    float elapsedTime=0;
    float elapsedTimeAction = 0;
    int frame = 0;
    bool dead = false;
    bool attacking = false;
    std::map<std::string, std::vector<std::vector<float>>> actions;
    
};


#endif /* Entity_hpp */

