//
//  HitBox.cpp
//  NYUCodebase
//
//  Created by Derek Yu on 12/19/16.
//  Copyright © 2016 Ivan Safrin. All rights reserved.
//

#include "HitBox.hpp"

void HitBox::Draw(ShaderProgram * program){
    
    float vertices [] = {
        0, 0,
        w , h,
        0, h,
        w, h,
        0 , 0,
        w, 0
    };
    
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(program->positionAttribute);
}

HitBox::HitBox(float x, float y, float w, float h):x(x), y(y), w(w), h(h){
    
}
