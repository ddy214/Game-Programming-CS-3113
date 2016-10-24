//
//  SheetSprite.hpp
//  NYUCodebase
//
//  Created by Derek Yu on 10/19/16.
//  Copyright © 2016 Ivan Safrin. All rights reserved.
//

#ifndef SheetSprite_hpp
#define SheetSprite_hpp

#include <stdio.h>
#include "ShaderProgram.h"

class SheetSprite{
    public:
    SheetSprite();
    SheetSprite(unsigned int textureID, float u, float v, float width, float height, float size);
    void Draw(ShaderProgram * program);
    
    float getWidth();
    
    float getHeight();
    
    float size;
    unsigned int textureID;
    float u;
    float v;
    float width;
    float height;
    
    
};

#endif /* SheetSprite_hpp */
