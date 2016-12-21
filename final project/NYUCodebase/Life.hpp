//
//  Life.hpp
//  NYUCodebase
//
//  Created by Derek Yu on 12/20/16.
//  Copyright © 2016 Ivan Safrin. All rights reserved.
//

#ifndef Life_hpp
#define Life_hpp

#include <stdio.h>
#include "ShaderProgram.h"
#include <vector>
class Life{
public:
    std::string text;
    unsigned int fontTexture;
    void Draw(ShaderProgram * program, float size, float spacing);
    Life(unsigned int fontTexture);

};

#endif /* Life_hpp */
