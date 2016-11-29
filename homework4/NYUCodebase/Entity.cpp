//
//  Entity.cpp
//  NYUCodebase
//
//  Created by Derek Yu on 11/15/16.
//  Copyright © 2016 Ivan Safrin. All rights reserved.
//

#include "Entity.hpp"

Entity::Entity(){
    
}

void Entity::Render(ShaderProgram *program){
    
    modelMatrix.identity();
    modelMatrix.Translate(x, y, 0);
    program->setModelMatrix(modelMatrix);
    if(entityType == ENTITY_PLAYER){
        if(dead && elapsedTime >= 0.5){
            sprite.DrawPlayer(program);
        } else if(!dead){
            sprite.DrawPlayer(program);
        }
        
        if(elapsedTime >= 0.65){
            elapsedTime = 0;
        }
    }
    else {
        sprite.Draw(program);
    }
}

void Entity::Update(float elapsed){
    if(dead){
        elapsedTime += elapsed;
        elapsedTimeTotal += elapsed;
    }
    if(velocity_x < 2 && acceleration_x > 0){
        velocity_x += acceleration_x * elapsed;
    } else if(velocity_x > -2 && acceleration_x < 0){
        velocity_x += acceleration_x * elapsed;
    }
    velocity_y += acceleration_y * elapsed;
    x += velocity_x*elapsed;
    y += velocity_y*elapsed;
    
    if(collidedBottom){
        velocity_y = 0;
    }
    
    if(collidedRight){
        velocity_x = 0;
    }
    if(collidedLeft){
        velocity_x = 0;
    }
}


bool Entity::collidesWith(Entity * entity){
    if(x > entity->x - entity->width/2 && x < entity->x + entity->width/2 && y-height/2 <= entity->y + entity->height/2 && y+height/2 > entity->y + entity->height/2){
        if(!entity->dead){
            entity->velocity_y = 3.0;
            entity->collidedBottom = false;
            entity->dead = true;
        }
    }
    else if(x-width < entity->x - entity->width/2 && x+width/2 >= entity->x - entity->width/2 && ((y+height/2 >= entity->y - entity->height/2 && y-height/2 <= entity->y - entity->height/2 )||(y-height/2 <= entity->y + entity->height/2 && y-height/2 >= entity->y - entity->height/2))){
        if(entityType == ENTITY_PLAYER ){
            if(!entity->dead){
                velocity_y = 3.0;
                velocity_x = -2.0;
                collidedBottom = false;
                collidedRight = true;
                dead = true;
            }
        }
    }
    else if(x+width > entity->x + entity->width/2 && x-width/2 <= entity->x + entity->width/2 && ((y+height/2 >= entity->y - entity->height/2 && y-height/2 <= entity->y - entity->height/2 )||(y-height/2 <= entity->y + entity->height/2 && y-height/2 >= entity->y - entity->height/2))){
        if(entityType == ENTITY_PLAYER ){
            if(!entity->dead) {
                velocity_y = 3.0;
                velocity_x = 2.0;
                collidedBottom = false;
                collidedLeft = true;
                dead = true;
            }
        }
    }
    
    
    return true;
}
