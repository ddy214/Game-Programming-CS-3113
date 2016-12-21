//
//  LevelLoader.hpp
//  NYUCodebase
//
//  Created by Derek Yu on 11/15/16.
//  Copyright © 2016 Ivan Safrin. All rights reserved.
//

#ifndef LevelLoader_hpp
#define LevelLoader_hpp

#include <stdio.h>
#include "string"
#include "fstream"
#include "sstream"
#include "iostream"
#include "vector"
#include "Entity.hpp"

using namespace std;



class LevelLoader {
public:
    string name;
    unsigned char** tileSet;
    int mapWidth;
    int mapHeight;
    int sprite_count_x;
    int sprite_count_y;
    vector<float> textureData;
    vector<float> vertexData;
    vector<Entity*> entities;
    
    LevelLoader(string filename, int sprite_count_x, int sprite_count_y);
    bool readHeader(ifstream& file);
    void readEntities(ifstream& file);
    void readLayers(ifstream& file);
    
    void placeEntity(float x, float y, string type);
    
    void createLevelData();
};
#endif /* LevelLoader_hpp */
