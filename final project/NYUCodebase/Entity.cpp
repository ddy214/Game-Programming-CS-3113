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
    modelMatrix.Scale(direction, 1, 1);
    program->setModelMatrix(modelMatrix);
    switch(action){
        case NO_ATTACK:
            if(elapsedTimeAction >= 0.20){
                std::vector<float>currAction = actions["stand"][frame];
                sprite.Draw3(program, currAction);
                frame++;
                if(frame == currAction.size()){
                    frame = 0;
                }
                elapsedTimeAction = 0;
            }else {
                std::vector<float>currAction = actions["stand"][frame];
                sprite.Draw3(program, currAction);
            }
            break;
        case PUNCH:
            sprite.Draw2(program, 118.0f/1024.0f, 108.0f/1024.0f, 83.0f/1024.0f, 112.0f/1024.0f);
            hitBox = new HitBox(x, y, 0.30, 0.50);
            hitBox->Draw(program);
            break;
        case KICK:
            sprite.Draw2(program, 0.0f, 826.0f/1024.0f, 93.0f/1024.0f, 106.0f/1024.0f);
            hitBox = new HitBox(x, y, 0.30, 0.50);
            hitBox->Draw(program);
            break;
        case MOVING:
            if(elapsedTimeAction >= 0.20){
                std::vector<float>currAction = actions["walking"][frame];
                sprite.Draw3(program, currAction);
                frame++;
                if(frame == currAction.size()){
                    frame = 0;
                }
                elapsedTimeAction = 0;
            }else {
                std::vector<float>currAction = actions["walking"][frame];
                sprite.Draw3(program, currAction);
                
            }
            break;
        case MOVING_B:
            if(elapsedTimeAction >= 0.30){
                std::vector<float>currAction = actions["walkingB"][frame];
                sprite.Draw3(program, currAction);
                frame++;
                if(frame == currAction.size()){
                    frame = 0;
                }
                elapsedTimeAction = 0;
            }else {
                std::vector<float>currAction = actions["walkingB"][frame];
                sprite.Draw3(program, currAction);
                
            }
            break;
        case JUMPING:
            if(frame >= 3){
                frame = 0;
            }
            if(frame == 0){
                std::vector<float>currAction = actions["jumping"][frame];
                sprite.DrawExtra(program, currAction, -0.15, 0.30);
                
            } else if(frame == 1){
                std::vector<float>currAction = actions["jumping"][frame];
                sprite.DrawExtra(program, currAction, -0.05, 0.10);
                
            } else if(frame == 2 ){
                std::vector<float>currAction = actions["jumping"][frame];
                sprite.DrawExtra(program, currAction, -0.10, -0.10);
            }
            if(elapsedTimeAction >= 0.30){
                frame++;
                elapsedTimeAction = 0;
            }
            break;
        case HIT:
            if(frame >= 3){
                frame = 0;
            }
            if(frame == 0){
                std::vector<float>currAction = actions["hit"][frame];
                sprite.Draw3(program, currAction);
                
            } else if(frame == 1){
                std::vector<float>currAction = actions["hit"][frame];
                sprite.Draw3(program, currAction);

            } else if(frame == 2 ){
                std::vector<float>currAction = actions["hit"][frame];
                sprite.DrawExtra(program, currAction,  -0.10, 0);

            }
            if(elapsedTimeAction >= 0.20){
                frame++;
                elapsedTimeAction = 0;
            }
            break;
    }
    modelMatrix.identity();
    modelMatrix.Translate(x, y+0.75, 0);
    modelMatrix.Scale(1,1,1);
    program->setModelMatrix(modelMatrix);
    life->text = std::to_string(health);
    life->Draw(program, 0.25f, 0.015f);
    
}

void Entity::Update(float elapsed){
    elapsedTimeAction += elapsed;
    if((action != NO_ATTACK && action != MOVING && action != MOVING_B && action != JUMPING )){
        elapsedTime += elapsed;
    }
    if(elapsedTime >= 0.30 && (action != NO_ATTACK && action != MOVING && action != MOVING_B && action != JUMPING && action!=HIT)){
        if(collidedBottom){
            action=NO_ATTACK;
            hitBox = nullptr;
        } else {
            action=JUMPING;
            hitBox = nullptr;
        }
        elapsedTime=0;
    }
    
    if(elapsedTime >= 0.50 && action == HIT){
        if(collidedBottom){
            action=NO_ATTACK;
            hitBox = nullptr;
        } else {
            action=JUMPING;
            hitBox = nullptr;
        }
        elapsedTime=0;
    }
    
    if(velocity_x < 5 && acceleration_x > 0){
        velocity_x += acceleration_x * elapsed;
    } else if(velocity_x > -5 && acceleration_x < 0){
        velocity_x += acceleration_x * elapsed;
    }
    velocity_y += acceleration_y * elapsed;
    x += velocity_x*elapsed;
    y += velocity_y*elapsed;
    
    if(collidedBottom){
        velocity_y = 0;
        if(action == JUMPING){
            action=NO_ATTACK;
        }
    }
    
    if(collidedRight){
        velocity_x = 0;
    }
    if(collidedLeft){
        velocity_x = 0;
    }
    
    if(health == 0){
        dead = true;
    }
}


bool Entity::collidesWith(Entity * entity){
    if((x+width/2)-0.30 >= entity->x - entity->width/2 && x-width/2  <= entity->x - entity->width/2
        && y-height/2 <= entity->y + entity->height/2 && entity->y+entity->height/2 <= y + height/2){
        float penetration = ((x+width/2)-0.30) - (entity->x - entity->width/2);
        x -= (penetration+0.01);
    }
    else if((x-width/2)+0.30 <= entity->x + entity->width/2 && x+width/2  >= entity->x + entity->width/2 && y-height/2 <= entity->y + entity->height/2 && entity->y+entity->height/2 <= y + height/2){
        float penetration = (entity->x + entity->width/2)-((x-width/2)+0.30) ;
        x += (penetration+0.01);
    }
    
    if(hitBox != nullptr){
        if(hitBox->x +hitBox->w >= entity->x - entity->width/2 && hitBox->x - hitBox->w <= entity->x - entity->width/2 && hitBox->y - hitBox->h <= entity->y + entity->height/2 && entity->y+entity->height/2 <= hitBox->y + hitBox->h){
            if(entity->action!=HIT){
                entity->action=HIT;
                entity->health -= 5;
                entity->x+=0.05;
            }
            
        }
        else if(hitBox->x-hitBox->w <= entity->x + entity->width/2 && hitBox->x+hitBox->w  >= entity->x + entity->width/2 && hitBox->y-hitBox->h <= entity->y + entity->height/2 && entity->y+entity->height/2 <= hitBox->y + hitBox->h){
            if(entity->action!=HIT){
                entity->action=HIT;
                entity->health -= 5;
                entity->x-=0.05;
            }
        }
    }
    
    return true;
}
