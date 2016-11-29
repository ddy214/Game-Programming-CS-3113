//
//  SheetSprite.hpp
//  NYUCodebase
//
//  Created by Derek Yu on 11/15/16.
//  Copyright © 2016 Ivan Safrin. All rights reserved.
//

#ifndef SheetSprite_hpp
#define SheetSprite_hpp

#include <stdio.h>
#include "ShaderProgram.h"
class SheetSprite{
public:
    SheetSprite();
    SheetSprite(unsigned int textureID, int spriteCountX, int spriteCountY, float size, int index);
    SheetSprite(unsigned int textureID, int spriteCountX, int spriteCountY, float size, int index, int index2);
    void Draw(ShaderProgram * program);
    
    void DrawPlayer(ShaderProgram * program);
    
    int spriteCountX;
    int spriteCountY;
    float size;
    unsigned int textureID;
    int index;
    int index2;
    float u;
    float v;
    float width;
    float height;
};


#endif /* SheetSprite_hpp */
