//
//  SheetSprite.cpp
//  NYUCodebase
//
//  Created by Derek Yu on 10/19/16.
//  Copyright © 2016 Ivan Safrin. All rights reserved.
//

#include "SheetSprite.hpp"

void SheetSprite::Draw(ShaderProgram * program){
    glBindTexture(GL_TEXTURE_2D, textureID);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_CLAMP);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_CLAMP);
    //std::cout<<width<<height;
    GLfloat textCoords[] = {
        u, v+height,
        u+width, v,
        u, v,
        u+width, v,
        u, v+height,//
        u+width, v + height
    };
    
    float aspect = width/height;
    
    float vertices [] = {
        -0.5f * size * aspect, -0.5f * size,
        0.5f * size * aspect, 0.5f * size,
        -0.5f * size * aspect, 0.5f * size,
        0.5f * size * aspect, 0.5f * size,
        -0.5f * size * aspect, -0.5f * size,
        0.5f * size * aspect, -0.5f * size
    };
    
    //std::cout<<(vertices);
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, textCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
    
}

float SheetSprite::getWidth(){
    return size*(width/height)*0.5f;
}

float SheetSprite::getHeight(){
    return size*0.5f;
}

SheetSprite::SheetSprite(unsigned int textureID, float u, float v, float width, float height, float size):textureID(textureID),u(u), v(v), width(width), height(height), size(size){
    //std::cout<<width;
}

SheetSprite::SheetSprite(){
    
}
