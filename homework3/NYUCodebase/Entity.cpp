 //
//  Entity.cpp
//  NYUCodebase
//
//  Created by Derek Yu on 10/19/16.
//  Copyright © 2016 Ivan Safrin. All rights reserved.
//

#include "Entity.hpp"


void Entity::Render(ShaderProgram *program){
    modelMatrix.identity();
    modelMatrix.Translate(x, y, 0);
    program->setModelMatrix(modelMatrix);
    sprite.Draw(program);
}

void Entity::Update(float elapsed){
    switch(entity_type){
        case PLAYER:
            if(collidedRight){
                x = 3.55 - width;
            } else if(collidedLeft){
                x = -3.55 + width;
            }
            x += elapsed * velocity_x;
            break;
        case ALIEN:
            if(collidedRight){
                x = 3.55 - width;
            } else if(collidedLeft){
                x = -3.55 + width;
            }
            totalTimeElapsed += elapsed;
            if(totalTimeElapsed >= 0.25){
                x += elapsed * velocity_x;
                totalTimeElapsed = 0;
            }
            break;
        case ALIEN_MISSILE:
            y += elapsed * velocity_y;
            break;
        case PLAYER_MISSILE:
            y += elapsed * velocity_y;
            break;
    }

    }

Entity::Entity(){
    
}

Entity::Entity(float x, float y, const SheetSprite& sheet, int entity_type):x(x), y(y), sprite(sheet), entity_type(entity_type){
    width = sprite.getWidth();
    height= sprite.getHeight();
}

bool Entity::collidesWith(Entity * entity){
    switch(entity_type){
        case PLAYER:
            if(entity == nullptr){
                if((x+width)>= 3.55){
                    collidedRight = true;
                    return true;
                }else if ((x-width) <= -3.55){
                    collidedLeft = true;
                    return true;
                } else {
                    collidedRight = false;
                    collidedLeft = false;
                    return false;
                }
            }
        case ALIEN:
            if(entity == nullptr){
                if((x+width)>= 3.55){
                    collidedRight = true;
                    return true;
                }else if ((x-width) <= -3.55){
                    collidedLeft = true;
                    return true;
                } else {
                    collidedRight = false;
                    collidedLeft = false;
                    return false;
                }
            }
        case PLAYER_MISSILE:
            if(entity==nullptr){
                if((y+height >= 2.0)){
                    collidedTop = true;
                    return true;
                }
            }
            else if((entity->x-width <= x && entity->x+width >= x) && entity->y-height <= y+height){
                //std::cout<<entity->width<<entity->x<<x;
                return true;
            }
            break;
        case ALIEN_MISSILE:
            if(entity->entity_type == PLAYER){
                if((entity->x-width <= x && entity->x+width >= x) && entity->y-height <= y+height){
                    return true;
                }
            }
            
            else{
                if((entity->x-width <= x && entity->x+width >= x) && entity->y-height <= y+height){
                    return true;
                }
            }
            break;
    }
    return false;
}
