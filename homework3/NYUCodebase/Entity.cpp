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
    program->setModelMatrix(modelMatrix);
    modelMatrix.Translate(x, y, 0);
    sprite.Draw(program);
}

void Entity::Update(float elapsed){
    
}

Entity::Entity(){
    
}

Entity::Entity(float x, float y, const SheetSprite& sheet):x(x), y(y), sprite(sprite){
    
}
