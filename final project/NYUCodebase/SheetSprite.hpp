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
#include <vector>
#include "ShaderProgram.h"
class SheetSprite{
public:
    SheetSprite();
    SheetSprite(unsigned int textureID, int spriteCountX, int spriteCountY, float size, int index);
    SheetSprite(unsigned int textureID, int spriteCountX, int spriteCountY, float size, int index, int index2);
    SheetSprite(unsigned int textureID, float size);
    void Draw(ShaderProgram * program);
    
    void DrawPlayer(ShaderProgram * program);
    
    void DrawExample(ShaderProgram * program);
    
    void Draw2(ShaderProgram * program, float u, float v , float height, float width);
    
    void Draw3(ShaderProgram * program, std::vector<float>action);
    
    void DrawExtra(ShaderProgram * program, std::vector<float>action,float xExtra, float yExtra);
    
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
