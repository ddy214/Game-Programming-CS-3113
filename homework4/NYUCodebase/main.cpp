#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "LevelLoader.hpp"
#include "ShaderProgram.h"
#include "Matrix.h"

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif
#define TILE_SIZE 0.5f
#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6

SDL_Window* displayWindow;
LevelLoader * levelLoader;
ShaderProgram * program;
Matrix projectionMatrix;
Matrix viewMatrix;
Matrix modelMatrix;
GLuint gameTexture;
Entity* player;
SDL_Event event;
float lastFrameTicks;
float startx, starty;
bool done;

GLuint LoadTexture(const char *image_path) {
    SDL_Surface *surface = IMG_Load(image_path);
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_BGRA,
                 GL_UNSIGNED_BYTE, surface->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    SDL_FreeSurface(surface);
    return textureID;
}

void worldToTileCoordinates(float worldX, float worldY, int*gridX, int*gridY){
    *gridX = (int) (worldX / TILE_SIZE);
    *gridY = (int) (-worldY / TILE_SIZE);
    
}

void checkForCollission(Entity *  entity){
    int gridX;
    int gridY;
    //bottom collission
    if(!entity->dead){
    worldToTileCoordinates(entity->x, entity->y - entity->height/2, &gridX, &gridY);
    if(levelLoader->tileSet[gridY][gridX] != 0){
        float entityEndpoint = entity->y - entity->height/2;
        float tileEndpoint = (gridY * -TILE_SIZE);
        float penetration = tileEndpoint - entityEndpoint;
        entity->y += (penetration+0.0001);
        entity->collidedBottom = true;
        
    } else {
        entity->collidedBottom = false;
    }
    
    //left collission
    worldToTileCoordinates(entity->x - entity->width/2, entity->y,&gridX, &gridY);
    if(levelLoader->tileSet[gridY][gridX] != 0){
        float entityEndpoint = entity->x - entity->width/2;
        float tileEndpoint = (gridX * TILE_SIZE)+TILE_SIZE;
        float penetration = tileEndpoint - entityEndpoint;
        entity->x += (penetration+0.0001);
        entity->collidedLeft = true;
    } else {
        entity->collidedLeft = false;
    }
    
    //right collission
    worldToTileCoordinates(entity->x + entity->width/2, entity->y,&gridX, &gridY);
    if(levelLoader->tileSet[gridY][gridX] != 0){
        float entityEndpoint = entity->x + entity->width/2;
        float tileEndpoint = (gridX * TILE_SIZE);
        float penetration = tileEndpoint - entityEndpoint;
        entity->x += (penetration-0.0001);
        entity->collidedRight = true;
    } else {
        entity->collidedRight = false;
    }
    
    for(int i = 0; i < levelLoader->entities.size();i++){
        if(levelLoader->entities[i] != entity){
            entity->collidesWith(levelLoader->entities[i]);
        }
    }
    }
    
}

void Setup(){
    levelLoader = new LevelLoader("Homework3.txt", 16, 8);
    glViewport(0, 0, 640, 360);
    program = new ShaderProgram(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
    projectionMatrix.setOrthoProjection(-4.0f, 4.0f, -2.25f, 2.25f, -4.5f, 4.5f);
    glUseProgram(program->programID);
    gameTexture = LoadTexture("arne_sprites.png");
    for(int i = 0; i < levelLoader->entities.size(); i++){
        if(levelLoader->entities[i]->entityType == ENTITY_PLAYER){
            SheetSprite sprite = SheetSprite(gameTexture, 16, 8, 0.25, 65, 99);
            levelLoader->entities[i]->sprite = sprite;
            levelLoader->entities[i]->height = 0.50;
            levelLoader->entities[i]->width = 0.50;
            player = levelLoader->entities[i];
            startx = player->x;
            starty = player->y;
        }
        else {
            SheetSprite sprite = SheetSprite(gameTexture, 16, 8, 0.25, 81);
            levelLoader->entities[i]->height = 0.50;
            levelLoader->entities[i]->width = 0.50;
            levelLoader->entities[i]->sprite = sprite;
        }
    }
}

void ProcessEvents(){
    const Uint8 * keys = SDL_GetKeyboardState(NULL);
    while(SDL_PollEvent(&event)){
        if(event.type == SDL_QUIT|| event.type == SDL_WINDOWEVENT_CLOSE) {
            done = true;
        }
        else if(event.type == SDL_KEYDOWN){
            if(event.key.keysym.scancode == SDL_SCANCODE_SPACE){
                if(player->collidedBottom == true){
                    player->velocity_y = 5;
                    player->collidedBottom = false;
                }
            }
        }
        else if(event.type == SDL_KEYUP){
            if(event.key.keysym.scancode == SDL_SCANCODE_LEFT){
                player->velocity_x = 0;
                player->acceleration_x = 0;
                
            } else if(event.key.keysym.scancode == SDL_SCANCODE_RIGHT){
                player->velocity_x = 0;
                player->acceleration_x = 0;
            }
        }
        if(keys[SDL_SCANCODE_RIGHT]){
            player->acceleration_x = 1.0;
        }
        else if(keys[SDL_SCANCODE_LEFT]){
            player->acceleration_x = -1.0;
        }

    }
    
}

void Update(){
    float ticks = (float) SDL_GetTicks()/1000;
    float elapsed = ticks - lastFrameTicks;
    lastFrameTicks = ticks;
    float fixedElapsed = elapsed;
    if(player->dead){
        if(player->elapsedTimeTotal > 1.5){
            player->dead = false;
            player->x = startx;
            player->y = starty;
            player->elapsedTimeTotal = 0;
        }
    }
    for(int i = 0; i < levelLoader->entities.size(); i++){
        if(levelLoader->entities[i] != player){
            if(levelLoader->entities[i]->dead){
                if(levelLoader->entities[i]->elapsedTimeTotal > 2){
                    levelLoader->entities[i]->dead = false;
                    levelLoader->entities[i]->x = startx + 3;
                    levelLoader->entities[i]->y = starty;
                    levelLoader->entities[i]->elapsedTimeTotal = 0;
                }
            }
        }
        if(fixedElapsed > FIXED_TIMESTEP * MAX_TIMESTEPS){
            fixedElapsed = FIXED_TIMESTEP * MAX_TIMESTEPS;
        }
        while(fixedElapsed >= FIXED_TIMESTEP){
            fixedElapsed -= FIXED_TIMESTEP;
            levelLoader->entities[i]->Update(fixedElapsed);
        }
        levelLoader->entities[i]->Update(FIXED_TIMESTEP);
        
        checkForCollission(levelLoader->entities[i]);
    }

}


void RenderMap(){
    program->setModelMatrix(modelMatrix);
    program->setViewMatrix(viewMatrix);
    program->setProjectionMatrix(projectionMatrix);
    glUseProgram(program->programID);
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, levelLoader->vertexData.data());
    glEnableVertexAttribArray(program->positionAttribute);
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, levelLoader->textureData.data());
    glEnableVertexAttribArray(program->texCoordAttribute);
    glBindTexture(GL_TEXTURE_2D, gameTexture);
    glDrawArrays(GL_TRIANGLES, 0, 6*levelLoader->mapHeight*levelLoader->mapWidth);
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
    
}

void RenderEntities(){
    vector<Entity*> entities = levelLoader->entities;
    for(int i = 0; i < entities.size(); i++){
        entities[i]->Render(program);
    }
}


void Render(){
    Entity * entity = levelLoader->entities[0];
    glClear(GL_COLOR_BUFFER_BIT);
    RenderMap();
    RenderEntities();
    viewMatrix.identity();
    viewMatrix.Translate(-entity->x, -entity->y, 0);
    program->setViewMatrix(viewMatrix);
    SDL_GL_SwapWindow(displayWindow);
}

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Platformer Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
#ifdef _WINDOWS
    glewInit();
#endif
    
    Setup();
    
    while (!done) {
        ProcessEvents();
        Update();
        Render();
        
    }
    
    SDL_Quit();
    return 0;
}
