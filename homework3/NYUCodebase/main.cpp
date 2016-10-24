#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "ShaderProgram.h"
#include "Matrix.h"
#include "vector"
#include "Entity.hpp"

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

//class for game entity
//enum to manage state of game
enum GameState {STATE_INRO, STATE_GAME};

//global variables to hold transformation matrices and entities and shader program
SDL_Window* displayWindow;
SDL_Event event;
int state = STATE_INRO;
Matrix projectionMatrix;
Matrix modelMatrix;
Matrix viewMatrix;
ShaderProgram * program;
Entity * ship;
std::vector<Entity*> enemies;
std::vector<Entity*> enemyMissiles;
std::vector<Entity*> playerMissiles;
Entity* firedMissile;
bool done = false;
float lastFrameTicks = 0;
GLuint letterTextures;
float missileCounter = 0;
float missileTimer = 0;
int missileNum = 0;
std::vector<int> firingAliens = {18,19,20,21,22,23};




GLuint LoadTexture(const char *image_path);

void DrawText(ShaderProgram *program, int fontTexture, std::string text, float size, float spacing);

void Setup();

void ProcessEventsIntro();

void ProcessEventsGame();

void ProcessEvents();

void RenderIntro();

void RenderGame();

void Render();

void UpdateIntro();

void UpdateGame();

void Update();

void CleanUp();

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Space Invaders", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
#ifdef _WINDOWS
    glewInit();
#endif
    
    Setup();
    while(!done){
        ProcessEvents();
        Update();
        Render();
    }
    CleanUp();
    SDL_Quit();
    return 0;
}


void Setup(){
    glViewport(0, 0, 640, 360);
    program = new ShaderProgram(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
    projectionMatrix.setOrthoProjection(-3.55f, 3.55f, -2.0f, 2.0f, -1.0f, 1.0f);
    glUseProgram(program->programID);
   letterTextures = LoadTexture("font1.png");
    GLuint shipTextures = LoadTexture("sheet.png");
    
    SheetSprite shipSprite(shipTextures, 325.0f/1024.0f, 0.0f/1024.0f, 98.0f/1024.0f, 75.0f/1024.0f,0.4);
    SheetSprite enemySprite(shipTextures, 425.0f/1024.0f,384.0f/1024.0f, 93.0f/1024.0f, 84.0f/1024.0f,0.4);
    SheetSprite enemyMissileSprite(shipTextures,843.0f/1024.0f, 62.0f/1024.0f,13.0f/1024.0f, 54.0f/1024.0f, 0.2);
    SheetSprite playerMissileSprite(shipTextures,843.0f/1024.0f, 62.0f/1024.0f,13.0f/1024.0f, 54.0f/1024.0f, 0.2);
    ship = new Entity(0, -1.75, shipSprite, PLAYER);
    
    for(int i = 1; i < 5; i++){
        for (int j = 1; j < 7; j++){
            Entity* newEntity = new Entity(-3.55 + (0.74*j), 2.10+(-0.60 * i), enemySprite, ALIEN );
            newEntity->velocity_x = 4;
            enemies.push_back(newEntity);
        }
    }
    
    for(int i = 0; i < 30; i++){
        Entity* enemyMissile = new Entity(-2000.0, 3.0, enemyMissileSprite, ALIEN_MISSILE );
        Entity* playerMissile = new Entity(-2000, .5, playerMissileSprite, PLAYER_MISSILE );
        enemyMissiles.push_back(enemyMissile);
        playerMissiles.push_back(playerMissile);
        
    }
    
}
void ProcessEventsIntro(){
    while(SDL_PollEvent(&event)){
        if(event.type == SDL_QUIT|| event.type == SDL_WINDOWEVENT_CLOSE) {
            done = true;
        }
        else if (event.type == SDL_KEYDOWN){
            if(event.key.keysym.scancode == SDL_SCANCODE_SPACE){
                state = STATE_GAME;
            }
        }
    }
}

void ProcessEventsGame(){
    const Uint8 * keys = SDL_GetKeyboardState(NULL);
    while(SDL_PollEvent(&event)){         if(event.type == SDL_QUIT|| event.type == SDL_WINDOWEVENT_CLOSE) {
            done = true;
        }
        else if (event.type == SDL_KEYDOWN){
            if(event.key.keysym.scancode == SDL_SCANCODE_SPACE){
                if(!firedMissile){
                    Entity * bullet = playerMissiles[playerMissiles.size()-1];
                    playerMissiles.pop_back();
                    bullet->x = ship->x;
                    bullet->y = ship->y+0.20;
                    bullet->velocity_y = 4.0;
                    firedMissile = bullet;
                }
            }
        }
        
        else if (event.type == SDL_KEYUP){
            if(event.key.keysym.scancode == SDL_SCANCODE_LEFT){
                ship->velocity_x = 0;
            }
            else if(event.key.keysym.scancode == SDL_SCANCODE_RIGHT){
                ship->velocity_x = 0;
            }
        }
    }
    
    if(keys[SDL_SCANCODE_RIGHT]){
        ship->velocity_x = 1.0;
    }
    
    else if(keys[SDL_SCANCODE_LEFT]){
        ship->velocity_x = -1.0;
    }
    
    
}

void ProcessEvents(){
    switch(state){
        case STATE_INRO:
            ProcessEventsIntro();
            break;
        case STATE_GAME:
            ProcessEventsGame();
            break;
    }
}

void UpdateIntro(){
    float ticks = (float) SDL_GetTicks()/1000;
    lastFrameTicks = ticks;
}

void UpdateGame(){
    float ticks = (float) SDL_GetTicks()/1000;
    float elapsed = ticks - lastFrameTicks;
    bool collidedWithShip = false;
    lastFrameTicks = ticks;
    ship->Update(elapsed);
    for(int i = 0; i < enemies.size(); i++){
        if(enemies[i]){
            enemies[i]->Update(elapsed);
        }
    }
    if(firedMissile){
        firedMissile->Update(elapsed);
        for(int i = 0; i < enemies.size(); i++){
            if(firedMissile->collidesWith(enemies[i])){
                delete firedMissile;
                firedMissile = nullptr;
                delete enemies[i];
                enemies[i] = nullptr;
                collidedWithShip = true;
                break;
            }
        }
        if(!collidedWithShip){
            if(firedMissile->collidesWith(nullptr)){
                delete firedMissile;
                firedMissile = nullptr;
            }
        }
    }
    
    ship->collidesWith(nullptr);
    for(int i = 0; i < enemies.size(); i++){
        if(enemies[i] != nullptr){
            if(enemies[i]->collidesWith(nullptr)){
                for(int i = 0; i < enemies.size(); i++){
                    if(enemies[i]){
                        enemies[i]->velocity_x *= -1.0;
                    //enemies[i]->y += -0.005;
                    }
                }
                break;
            }
        }
    }
    missileCounter += elapsed;
    if(missileCounter >= 1.0){
        
        missileCounter = 0;
    }
    
    for(int i = 0; i < firingAliens.size(); i++){
        if(firingAliens[i] >= 0){
            if(enemies[firingAliens[i]] == nullptr){
                firingAliens[i] -= 6;
            }
            
        }
    }
    missileTimer += elapsed;
    int randomIndex = rand() % firingAliens.size();
    if(missileTimer >= 2){
        int alienFire = firingAliens[randomIndex];
        if(alienFire >= 0){
            enemyMissiles[missileNum]->x = enemies[alienFire]->x;
            enemyMissiles[missileNum]->y = enemies[alienFire]->y;
            enemyMissiles[missileNum]->velocity_y = -1.0;
            missileTimer = 0;
        }
    }
    
    for(int i = 0; i < enemyMissiles.size();i++){
        if(enemyMissiles[i]->collidesWith(ship)){
            std::cout<<"crashed";
        }
        
        else if(firedMissile != nullptr){
            if(enemyMissiles[i]->collidesWith(firedMissile)){
                std::cout<<"crashed missile";
            }
        }
    }
    


    for(int i = 0; i < enemyMissiles.size(); i++){
        enemyMissiles[i]->Update(elapsed);
    }
    
    
    
    
}

void Update(){
    switch(state){
        case STATE_INRO:
            UpdateIntro();
            break;
        case STATE_GAME:
            UpdateGame();
            break;
    }
}

void RenderIntro(){
    modelMatrix.identity();
    modelMatrix.Translate(-3.2, 0.5, 0);
    program->setModelMatrix(modelMatrix);
    std::string welcome = "Welcome to Space Invaders";
    DrawText(program, letterTextures, welcome, 0.25f, 0.015f);
    modelMatrix.identity();
    modelMatrix.Translate(-2.45, -0.5, 0);
    program->setModelMatrix(modelMatrix);
    std::string pressSpace = "Press Space to Play";
    DrawText(program, letterTextures, pressSpace, 0.25f, 0.015f);
}

void RenderGame(){
    ship->Render(program);
    for(int i = 0; i < enemies.size(); i++){
        if(enemies[i]){
            enemies[i]->Render(program);
        }
    }
    if(firedMissile){
        firedMissile->Render(program);
    }
    
    for(int i = 0; i < enemyMissiles.size(); i++){
        enemyMissiles[i]->Render(program);
    }
}

void Render(){
    glClear(GL_COLOR_BUFFER_BIT);
    program->setProjectionMatrix(projectionMatrix);
    program->setViewMatrix(viewMatrix);
    switch(state){
        case STATE_INRO:
            RenderIntro();
            break;
        case STATE_GAME:
            RenderGame();
            break;
    }
    SDL_GL_SwapWindow(displayWindow);
}


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

void DrawText(ShaderProgram *program, int fontTexture, std::string text, float size, float spacing) {
    float texture_size = 1.0/16.0f;
    std::vector<float> vertexData;
    std::vector<float> texCoordData;
    for(int i=0; i < text.size(); i++) {
        float texture_x = (float)(((int)text[i]) % 16) / 16.0f;
        float texture_y = (float)(((int)text[i]) / 16) / 16.0f;
        vertexData.insert(vertexData.end(), {
            ((size+spacing) * i) + (-0.5f * size), 0.5f * size,
            ((size+spacing) * i) + (-0.5f * size), -0.5f * size,
            ((size+spacing) * i) + (0.5f * size), 0.5f * size,
            ((size+spacing) * i) + (0.5f * size), -0.5f * size,
            ((size+spacing) * i) + (0.5f * size), 0.5f * size,
            ((size+spacing) * i) + (-0.5f * size), -0.5f * size,
        });
        texCoordData.insert(texCoordData.end(), {
            texture_x, texture_y,
            texture_x, texture_y + texture_size,
            texture_x + texture_size, texture_y,
            texture_x + texture_size, texture_y + texture_size,
            texture_x + texture_size, texture_y,
            texture_x, texture_y + texture_size,
        }); }
    glUseProgram(program->programID);
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertexData.data());
    glEnableVertexAttribArray(program->positionAttribute);
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoordData.data());
    glEnableVertexAttribArray(program->texCoordAttribute);
    glBindTexture(GL_TEXTURE_2D, fontTexture);
    glDrawArrays(GL_TRIANGLES, 0, text.size() * 6);
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}



void CleanUp(){
    
}

