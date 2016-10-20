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

class Entity {
public:
    Entity();
    
    Entity(float x, float y, const SheetSprite& sheet);
    
    void Update(float elapsed);
    
    void Render(ShaderProgram *program);
    
    bool collidesWith(Entity * entity);
    
    SheetSprite sprite;
    
    float width;
    float height;
    float x;
    float y;
    float velocity_x;
    float velocity_y;
    Matrix modelMatrix;
    
};



#endif /* Entity_hpp */
