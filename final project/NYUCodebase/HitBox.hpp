//
//  HitBox.hpp
//  NYUCodebase
//
//  Created by Derek Yu on 12/19/16.
//  Copyright © 2016 Ivan Safrin. All rights reserved.
//

#ifndef HitBox_hpp
#define HitBox_hpp

#include <stdio.h>
#include "ShaderProgram.h"

class HitBox{
public:
    float x;
    float y;
    float w;
    float h;
    
    HitBox(float x, float y, float w, float h);
    void Draw(ShaderProgram * program);
};

#endif /* HitBox_hpp */

