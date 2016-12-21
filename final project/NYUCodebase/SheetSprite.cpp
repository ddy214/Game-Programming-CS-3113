//
//  SheetSprite.cpp
//  NYUCodebase
//
//  Created by Derek Yu on 11/15/16.
//  Copyright © 2016 Ivan Safrin. All rights reserved.
//

#include "SheetSprite.hpp"

SheetSprite::SheetSprite(){
    
}

SheetSprite::SheetSprite(unsigned int textureID, int spriteCountX, int spriteCountY, float size, int index):textureID(textureID),spriteCountX(spriteCountX), spriteCountY(spriteCountY),size(size), index(index){
    
}
SheetSprite::SheetSprite(unsigned int textureID, int spriteCountX, int spriteCountY, float size, int index, int index2):textureID(textureID), spriteCountX(spriteCountX), spriteCountY(spriteCountY), size(size), index(index), index2(index2){
    
}

SheetSprite::SheetSprite(unsigned int textureID,float size):textureID(textureID), size(size){
    
}

void SheetSprite::DrawExample(ShaderProgram * program){
    glBindTexture(GL_TEXTURE_2D, textureID);
    float textCoords[] = {
        0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 1.0, 1.0
    };
    
    float vertices [] = {
        -size, -size,
        size , size,
        -size, size,
        size , size,
        -size , -size,
        size , -size
    };
    
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, textCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
    
}

void SheetSprite::DrawPlayer(ShaderProgram * program){
    glBindTexture(GL_TEXTURE_2D, textureID);
    float u = (float) (((int) index)%spriteCountX)/(float)spriteCountX;
    float v = (float) (((int) index)/spriteCountX)/(float)spriteCountY;
    float u2 =(float) (((int) index2)%spriteCountX)/(float)spriteCountX;
    float v2 = (float) (((int) index2)/spriteCountX)/(float)spriteCountY;
    width = 1.0/(float)spriteCountX;
    height = 1.0/(float)spriteCountY;
    
    GLfloat textCoords[] = {
        u, v+height,
        u+width, v,
        u, v,
        u+width, v,
        u, v+height,
        u+width, v + height,
        u2, v2+height,
        u2+width, v2,
        u2, v2,
        u2+width, v2,
        u2, v2+height,
        u2+width, v2 + height
    };
    
    
    float vertices [] = {
        -size, -size,
        size, size,
        -size, size,
        size , size,
        -size , -size,
        size , -size,
        -size, -size,
        size , size,
        -size, size,
        size , size,
        -size , -size,
       size , -size
    };
    
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, textCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);
    glDrawArrays(GL_TRIANGLES, 0, 12);
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}


void SheetSprite::Draw(ShaderProgram * program){
    glBindTexture(GL_TEXTURE_2D, textureID);
    float u = (float) (((int) index)%spriteCountX)/(float)spriteCountX;
    float v = (float) (((int) index)/spriteCountX)/(float)spriteCountY;
    width = 1.0/(float)spriteCountX;
    height = 1.0/(float)spriteCountY;
    
    GLfloat textCoords[] = {
        u, v+height,
        u+width, v,
        u, v,
        u+width, v,
        u, v+height,
        u+width, v + height
    };
    
    
    float vertices [] = {
        -size, -size,
        size , size,
        -size, size,
        size , size,
        -size , -size,
        size , -size
    };
    
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, textCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

void SheetSprite::Draw2(ShaderProgram * program, float u, float v , float width, float height){
    glBindTexture(GL_TEXTURE_2D, textureID);
    GLfloat textCoords[] = {
        u, v+height,
        u+width, v,
        u, v,
        u+width, v,
        u, v+height,//
        u+width, v + height
    };
    
    
    float vertices [] = {
        -size, -size,
        size , size,
        -size, size,
        size , size,
        -size , -size,
        size , -size
    };
    
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, textCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
    
}

void SheetSprite::Draw3(ShaderProgram * program, std::vector<float>action){
    glBindTexture(GL_TEXTURE_2D, textureID);
    GLfloat textCoords[] = {
        action[0], action[1]+action[3],
        action[0]+action[2], action[1],
        action[0], action[1],
        action[0]+action[2], action[1],
        action[0], action[1]+action[3],//
        action[0]+action[2], action[1] + action[3]
    };
    
    
    float vertices [] = {
        -size, -size,
        size , size,
        -size, size,
        size , size,
        -size , -size,
        size , -size
    };
    
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, textCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
    
}

void SheetSprite::DrawExtra(ShaderProgram * program, std::vector<float>action, float xExtra, float yExtra){
    glBindTexture(GL_TEXTURE_2D, textureID);
    GLfloat textCoords[] = {
        action[0], action[1]+action[3],
        action[0]+action[2], action[1],
        action[0], action[1],
        action[0]+action[2], action[1],
        action[0], action[1]+action[3],//
        action[0]+action[2], action[1] + action[3]
    };
    
    
    float vertices [] = {
        -size-xExtra, -size-yExtra,
        size+xExtra , size+yExtra,
        -size-xExtra, size+yExtra,
        size+xExtra, size+yExtra,
        -size-xExtra , -size-yExtra,
        size+xExtra , -size-yExtra
    };
    
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, textCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

