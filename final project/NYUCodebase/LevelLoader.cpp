//
//  LevelLoader.cpp
//  NYUCodebase
//
//  Created by Derek Yu on 11/15/16.
//  Copyright © 2016 Ivan Safrin. All rights reserved.
//

#include "LevelLoader.hpp"
#define TILE_SIZE 0.5f

LevelLoader::LevelLoader(string filename, int sprite_count_x, int sprite_count_y):sprite_count_x(sprite_count_x), sprite_count_y(sprite_count_y){
    ifstream mapFile(filename);
    string line;
    while(getline(mapFile, line)){
        if(line == "[header]"){
            if(!readHeader(mapFile)){
                return;
            }
        }
        else if(line == "[Entities]"){
            readEntities(mapFile);
        }
        
        else if(line == "[layer]"){
            readLayers(mapFile);
        }
    }
    
    createLevelData();
    
}

bool LevelLoader::readHeader(ifstream& file){
    string line;
    int mapWidth = -1;
    int mapHeight = -1;
    while(getline(file, line)){
        if(line == ""){
            break;
        }
        istringstream sStream(line);
        string key, value;
        getline(sStream, key, '=');
        getline(sStream, value);
        
        if(key == "width"){
            mapWidth = atoi(value.c_str());
            this->mapWidth = mapWidth;
            
        } else if(key ==  "height"){
            mapHeight = atoi(value.c_str());
            this->mapHeight = mapHeight;
        }
    }
    
    if(mapWidth == -1){
        return false;
    } else {
        tileSet = new unsigned char*[mapHeight];
        for(int i = 0; i < mapHeight; i++){
            tileSet[i] = new unsigned char[mapWidth];
        }
    }
    return true;
}

void LevelLoader::readLayers(ifstream& file){
    string line;
    while(getline(file, line)){
        if(line == ""){
            break;
        }
        istringstream sStream(line);
        string key, value;
        getline(sStream, key, '=');
        getline(sStream, value);
        if(key == "data"){
            for(int y=0; y < mapHeight; y++){
                getline(file, line);
                istringstream lineStream(line);
                string tile;
                
                for(int x=0; x < mapWidth; x++){
                    getline(lineStream, tile,',');
                    unsigned int val = (unsigned) atoi(tile.c_str());
                    if(val > 0){
                        tileSet[y][x] = val-1;
                    } else {
                        tileSet[y][x] = 0;
                    }
                }
            }
        }
    }
    
}

void LevelLoader::readEntities(ifstream& file){
    string line;
    string type;
    while(getline(file, line)){
        if(line == ""){
            break;
        }
        
        istringstream sStream(line);
        string key, value;
        getline(sStream, key, '=');
        getline(sStream, value);
        
        if(key == "type"){
            type = value;
        }
        else if(key == "location"){
            istringstream lineStream(value);
            string xPosition, yPostion;
            getline(lineStream, xPosition,',');
            getline(lineStream, yPostion, ',');
            
            float placeX = atoi(xPosition.c_str())*TILE_SIZE;
            float placeY = atoi(yPostion.c_str()) * -TILE_SIZE;
            placeEntity(placeX, placeY, type);
        }
    }
}

void LevelLoader::placeEntity(float x, float y, string type){
    if(type == "Start"){
        Entity * player = new Entity();
        player->entityType = ENTITY_PLAYER;
        player->x = x;
        player->y = y;
        entities.insert(entities.end(), player);
    }
    else if(type == "Enemy"){
        Entity * tempEntity = new Entity();
        tempEntity->entityType = ENTITY_ENEMY;
        tempEntity->x = x;
        tempEntity->y = y;
        entities.insert(entities.end(), tempEntity);
    }
    
 
    
}

void LevelLoader::createLevelData(){
    
    for(int y = 0; y < mapHeight; y++){
        for(int x = 0; x< mapWidth; x++){
            if(tileSet[y][x] != 0){
                float u = (float)(((int) tileSet[y][x]) % sprite_count_x) / (float) sprite_count_x;
                float v = (float)(((int) tileSet[y][x]) / sprite_count_x) / (float) sprite_count_y;
                
                float spriteWidth = 1.0f/(float) sprite_count_x;
                float spriteHeight = 1.0f/(float) sprite_count_y;
                
                vertexData.insert(vertexData.end(),{
                    TILE_SIZE * x, -TILE_SIZE * y,
                    TILE_SIZE * x, (-TILE_SIZE * y)-TILE_SIZE,
                    (TILE_SIZE * x) + TILE_SIZE, (-TILE_SIZE*y)-TILE_SIZE,
                    
                    TILE_SIZE * x, -TILE_SIZE * y,
                    (TILE_SIZE * x) + TILE_SIZE, (-TILE_SIZE *y)-TILE_SIZE,
                    (TILE_SIZE * x) + TILE_SIZE, -TILE_SIZE * y
                });
                
                textureData.insert(textureData.end(),{
                    u, v,
                    u, v+(spriteHeight),
                    u+spriteWidth, v+(spriteHeight),
                    u, v,
                    u+spriteWidth, v+(spriteHeight),
                    u+spriteWidth, v
                });
            }
        }
    }
    
}













