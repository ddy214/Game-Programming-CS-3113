#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "LevelLoader.hpp"
#include "ShaderProgram.h"
#include "Matrix.h"
#include "Life.hpp"

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif
#define TILE_SIZE 0.5f
#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6

enum GAME_STATE {
    INTRO,
    LEVEL_1,
    INTRO_2,
    LEVEL_2,
    INTRO_3,
    LEVEL_3,
    FINISH
    
};

bool init = false;
int player1R = 0;
int player2R = 0;
string message = "";
vector<vector<float>> starting;
vector<vector<float>> walking;
vector<vector<float>> walkingB;
vector<vector<float>> jumping;
vector<vector<float>> hit;
int currentState = INTRO;
GLuint marioSprite;
GLuint characterSprite;
GLuint fontTexture;
SDL_Window* displayWindow;
LevelLoader * levelLoader;
ShaderProgram * program;
Matrix projectionMatrix;
Matrix viewMatrix;
Matrix modelMatrix;
GLuint gameTexture;
Entity* player;
Entity* player2;
SDL_Event event;
float lastFrameTicks;
float startx, starty;
Mix_Music * music;
Mix_Chunk * jump;
Mix_Chunk * punch;
Mix_Chunk * kick;
bool done=false;


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

void Setup1(){
    if(init){
        Life * life1 = new Life(fontTexture);
        Life * life2 = new Life(fontTexture);
        levelLoader = new LevelLoader("NYUCodebase.app/Contents/Resources/Homework3.txt", 16, 8);
        SheetSprite sprite = SheetSprite(characterSprite,0.5f);
        levelLoader->entities[0]->sprite = sprite;
        levelLoader->entities[0]->height = 1.00;
        levelLoader->entities[0]->width = 1.00;
        player = levelLoader->entities[0];
        startx = player->x;
        starty = player->y;
        SheetSprite sprite2 = SheetSprite(characterSprite, 0.5f);
        levelLoader->entities[1]->sprite = sprite2;
        levelLoader->entities[1]->height = 1.00;
        levelLoader->entities[1]->width = 1.00;
        player2 = levelLoader->entities[1];
        player2->direction = -1;
        
        player->actions["stand"] = starting;
        player->actions["walking"] = walking;
        player->actions["walkingB"] = walkingB;
        player->actions["jumping"] = jumping;
        player->actions["hit"] = hit;
        player->life = life1;
        
        player2->actions["stand"] = starting;
        player2->actions["walking"] = walking;
        player2->actions["walkingB"] = walkingB;
        player2->actions["jumping"] = jumping;
        player2->actions["hit"] = hit;
        player2->life = life2;
    }
    init = false;
}

void Setup2(){
    if(init){
        Life * life1 = new Life(fontTexture);
        Life * life2 = new Life(fontTexture);
        levelLoader = new LevelLoader("NYUCodebase.app/Contents/Resources/final_level.txt", 16, 8);
        SheetSprite sprite = SheetSprite(characterSprite,0.5f);
        levelLoader->entities[0]->sprite = sprite;
        levelLoader->entities[0]->height = 1.00;
        levelLoader->entities[0]->width = 1.00;
        player = levelLoader->entities[0];
        startx = player->x;
        starty = player->y;
        SheetSprite sprite2 = SheetSprite(characterSprite, 0.5f);
        levelLoader->entities[1]->sprite = sprite2;
        levelLoader->entities[1]->height = 1.00;
        levelLoader->entities[1]->width = 1.00;
        player2 = levelLoader->entities[1];
        player2->direction = -1;
        
        player->actions["stand"] = starting;
        player->actions["walking"] = walking;
        player->actions["walkingB"] = walkingB;
        player->actions["jumping"] = jumping;
        player->actions["hit"] = hit;
        player->life = life1;
        
        player2->actions["stand"] = starting;
        player2->actions["walking"] = walking;
        player2->actions["walkingB"] = walkingB;
        player2->actions["jumping"] = jumping;
        player2->actions["hit"] = hit;
        player2->life = life2;
    }
    init = false;
}

void Setup3(){
    if(init){
        Life * life1 = new Life(fontTexture);
        Life * life2 = new Life(fontTexture);
        levelLoader = new LevelLoader("NYUCodebase.app/Contents/Resources/final_level2.txt", 16, 8);
        SheetSprite sprite = SheetSprite(characterSprite,0.5f);
        levelLoader->entities[0]->sprite = sprite;
        levelLoader->entities[0]->height = 1.00;
        levelLoader->entities[0]->width = 1.00;
        player = levelLoader->entities[0];
        startx = player->x;
        starty = player->y;
        SheetSprite sprite2 = SheetSprite(characterSprite, 0.5f);
        levelLoader->entities[1]->sprite = sprite2;
        levelLoader->entities[1]->height = 1.00;
        levelLoader->entities[1]->width = 1.00;
        player2 = levelLoader->entities[1];
        player2->direction = -1;
        
        player->actions["stand"] = starting;
        player->actions["walking"] = walking;
        player->actions["walkingB"] = walkingB;
        player->actions["jumping"] = jumping;
        player->actions["hit"] = hit;
        player->life = life1;
        
        player2->actions["stand"] = starting;
        player2->actions["walking"] = walking;
        player2->actions["walkingB"] = walkingB;
        player2->actions["jumping"] = jumping;
        player2->actions["hit"] = hit;
        player2->life = life2;
    }
    init = false;
}



void ProcessEventsGame(){
    const Uint8 * keys = SDL_GetKeyboardState(NULL);
    while(SDL_PollEvent(&event)){
        if(event.type == SDL_QUIT|| event.type == SDL_WINDOWEVENT_CLOSE) {
            done = true;
        }
        else if(event.type == SDL_KEYDOWN){
            if(event.key.keysym.scancode == SDL_SCANCODE_W){
                if(player->collidedBottom == true){
                    player->velocity_y = 7;
                    player->collidedBottom = false;
                    player->action = JUMPING;
                    Mix_PlayChannel(-1, jump, 0);
                }
            }
            if(event.key.keysym.scancode == SDL_SCANCODE_C){
                player->action = PUNCH;
                Mix_PlayChannel(-1, punch, 0);
            }
            
            if(event.key.keysym.scancode == SDL_SCANCODE_V){
                player->action = KICK;
                Mix_PlayChannel(-1, kick, 0);
            }
            
            if(event.key.keysym.scancode == SDL_SCANCODE_H){
                
                if(player2->collidedBottom == true){
                    player2->velocity_y = 7;
                    player2->collidedBottom = false;
                    player2->action = JUMPING;
                    Mix_PlayChannel(-1, jump, 0);

                }
            }
            if(event.key.keysym.scancode == SDL_SCANCODE_PERIOD){
                player2->action = PUNCH;
                Mix_PlayChannel(-1, punch, 0);
            }
            
            if(event.key.keysym.scancode == SDL_SCANCODE_SLASH){
                player2->action = KICK;
                Mix_PlayChannel(-1, kick, 0);
            }
            
            if(event.key.keysym.scancode == SDL_SCANCODE_0){
                done = true;
            }
            
            if(event.key.keysym.scancode == SDL_SCANCODE_9){
                currentState += 1;
            }
        }
        else if(event.type == SDL_KEYUP){
            if(event.key.keysym.scancode == SDL_SCANCODE_A){
                player->velocity_x = 0;
                player->acceleration_x = 0;
                if(player->collidedBottom){
                    player->action = NO_ATTACK;
                }
                
            } else if(event.key.keysym.scancode == SDL_SCANCODE_D){
                player->velocity_x = 0;
                player->acceleration_x = 0;
                if(player->collidedBottom){
                    player->action = NO_ATTACK;
                }
            }
            if(event.key.keysym.scancode == SDL_SCANCODE_B){
                player2->velocity_x = 0;
                player2->acceleration_x = 0;
                if(player2->collidedBottom){
                    player2->action = NO_ATTACK;
                }
                
            } else if(event.key.keysym.scancode == SDL_SCANCODE_M){
                player2->velocity_x = 0;
                player2->acceleration_x = 0;
                if(player2->collidedBottom){
                    player2->action = NO_ATTACK;
                }
            }
            
            
        }
        if(keys[SDL_SCANCODE_D]){
            player->acceleration_x = 3;
            if(player->collidedBottom){
                if(player->direction==1){
                    player->action = MOVING;
                }
                else{
                    player->action = MOVING_B;
                }
            }
        }
        else if(keys[SDL_SCANCODE_A]){
            player->acceleration_x = -3;
            if(player->collidedBottom){
                if(player->direction==-1){
                    player->action = MOVING;
                }
                else{
                    player->action = MOVING_B;
                }
            }
        }
        if(keys[SDL_SCANCODE_M]){
            player2->acceleration_x = 3;
            if(player2->collidedBottom){
                if(player2->direction==1){
                    player2->action = MOVING;
                }
                else{
                    player2->action = MOVING_B;
                }
            }
        }
        else if(keys[SDL_SCANCODE_B]){
            player2->acceleration_x = -3;
            if(player2->collidedBottom){
                if(player2->direction==-1){
                    player2->action = MOVING;
                }
                else{
                    player2->action = MOVING_B;
                }
            }
        }

    }
    
}
void ProcessEventsIntro(){
    while(SDL_PollEvent(&event)){
        if(event.type == SDL_QUIT|| event.type == SDL_WINDOWEVENT_CLOSE) {
            done = true;
        }
        else if (event.type == SDL_KEYDOWN){
            if(event.key.keysym.scancode == SDL_SCANCODE_SPACE){
                currentState = LEVEL_1;
                init = true;
            }
        }
    }
}

void ProcessEventsIntro2(){
    while(SDL_PollEvent(&event)){
        if(event.type == SDL_QUIT|| event.type == SDL_WINDOWEVENT_CLOSE) {
            done = true;
        }
        else if (event.type == SDL_KEYDOWN){
            if(event.key.keysym.scancode == SDL_SCANCODE_SPACE){
                currentState = LEVEL_2;
                init = true;
            }
        }
    }
}

void ProcessEventsIntro3(){
    while(SDL_PollEvent(&event)){
        if(event.type == SDL_QUIT|| event.type == SDL_WINDOWEVENT_CLOSE) {
            done = true;
        }
        else if (event.type == SDL_KEYDOWN){
            if(event.key.keysym.scancode == SDL_SCANCODE_SPACE){
                currentState = LEVEL_3;
                init = true;
            }
        }
    }
}

void ProcessEventsFinal(){
    while(SDL_PollEvent(&event)){
        if(event.type == SDL_QUIT|| event.type == SDL_WINDOWEVENT_CLOSE) {
            done = true;
        }
        else if (event.type == SDL_KEYDOWN){
            if(event.key.keysym.scancode == SDL_SCANCODE_SPACE){
                currentState = LEVEL_1;
                init = true;
                player1R = 0;
                player2R = 0;
            }
        }
    }
}

void UpdateGame(){
    float ticks = (float) SDL_GetTicks()/1000;
    float elapsed = ticks - lastFrameTicks;
    lastFrameTicks = ticks;
    float fixedElapsed = elapsed;
    for(int i = 0; i < levelLoader->entities.size(); i++){
        if(fixedElapsed > FIXED_TIMESTEP * MAX_TIMESTEPS){
            fixedElapsed = FIXED_TIMESTEP * MAX_TIMESTEPS;
        }
        while(fixedElapsed >= FIXED_TIMESTEP){
            fixedElapsed -= FIXED_TIMESTEP;
            levelLoader->entities[i]->Update(fixedElapsed);
        }
        levelLoader->entities[i]->Update(FIXED_TIMESTEP);
        
        checkForCollission(levelLoader->entities[i]);
        

        if(player->x > player2->x){
            player->direction = -1;
            player2->direction = 1;
        } else {
            player->direction = 1;
            player2->direction = -1;
        }
        if(levelLoader->entities[i]->dead){
            if(levelLoader->entities[i] == player){
                player2R += 1;
                message = "Player 2 won";
            } else {
                player1R += 1;
                message = "Player 1 won";
            }
            currentState += 1;
        }
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

void RenderIntro(){
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(program->programID);
    viewMatrix.identity();
    modelMatrix.identity();
    modelMatrix.Translate(-3.2, 0.5, 0);
    program->setProjectionMatrix(projectionMatrix);
    program->setViewMatrix(viewMatrix);
    program->setModelMatrix(modelMatrix);
    std::string welcome = "Welcome to Mario Fighters";
    DrawText(program, fontTexture, welcome, 0.25f, 0.015f);
    modelMatrix.identity();
    modelMatrix.Translate(-3.2, -0.5, 0);
    program->setModelMatrix(modelMatrix);
    std::string pressSpace = "Press Space to Play";
    DrawText(program, fontTexture, pressSpace, 0.25f, 0.015f);
    SDL_GL_SwapWindow(displayWindow);

}

void RenderIntro2(){
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(program->programID);
    viewMatrix.identity();
    modelMatrix.identity();
    modelMatrix.Translate(-3.2, 0.5, 0);
    program->setProjectionMatrix(projectionMatrix);
    program->setViewMatrix(viewMatrix);
    program->setModelMatrix(modelMatrix);
    DrawText(program, fontTexture, message, 0.25f, 0.015f);
    modelMatrix.identity();
    modelMatrix.Translate(-3.2, -0.5, 0);
    program->setModelMatrix(modelMatrix);
    std::string pressSpace = "Press Space to Play Round 2";
    DrawText(program, fontTexture, pressSpace, 0.25f, 0.015f);
    SDL_GL_SwapWindow(displayWindow);
    
}

void RenderIntro3(){
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(program->programID);
    viewMatrix.identity();
    modelMatrix.identity();
    modelMatrix.Translate(-3.2, 0.5, 0);
    program->setProjectionMatrix(projectionMatrix);
    program->setViewMatrix(viewMatrix);
    program->setModelMatrix(modelMatrix);
    DrawText(program, fontTexture, message, 0.25f, 0.015f);
    modelMatrix.identity();
    modelMatrix.Translate(-3.2, -0.5, 0);
    program->setModelMatrix(modelMatrix);
    std::string pressSpace = "Press Space to Play Round 3";
    DrawText(program, fontTexture, pressSpace, 0.25f, 0.015f);
    SDL_GL_SwapWindow(displayWindow);
    
}

void RenderFinal(){
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(program->programID);
    viewMatrix.identity();
    modelMatrix.identity();
    modelMatrix.Translate(-3.2, 0.5, 0);
    program->setProjectionMatrix(projectionMatrix);
    program->setViewMatrix(viewMatrix);
    program->setModelMatrix(modelMatrix);
    std::string welcome = "Game Over";
    DrawText(program, fontTexture, welcome, 0.25f, 0.015f);
    modelMatrix.identity();
    modelMatrix.Translate(-3.2, 0, 0);
    program->setModelMatrix(modelMatrix);
    std::string pressSpace = "Press Space to Play Again";
    DrawText(program, fontTexture, pressSpace, 0.25f, 0.015f);
    modelMatrix.identity();
    modelMatrix.Translate(-3.2, -1.0, 0);
    program->setModelMatrix(modelMatrix);
    std::string score = "Player 1: " + to_string(player1R) + "    Player2: " + to_string(player2R);
    DrawText(program, fontTexture, score, 0.25f, 0.015f);
    SDL_GL_SwapWindow(displayWindow);
    
}

void RenderGame(){
    modelMatrix.identity();
    program->setModelMatrix(modelMatrix);
    Entity * entity = levelLoader->entities[0];
    Entity * entity2 = levelLoader->entities[1];
    glClear(GL_COLOR_BUFFER_BIT);
    RenderMap();
    RenderEntities();
    viewMatrix.identity();
    viewMatrix.Translate(-(entity2->x+entity->x)/2 , -((entity2->y+entity->y)/2)-0.20, 1);
    program->setViewMatrix(viewMatrix);
    SDL_GL_SwapWindow(displayWindow);
}

void SetupInit(){
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Mario Fighter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    #ifdef _WINDOWS
        glewInit();
    #endif
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    fontTexture = LoadTexture("font1.png");
    music = Mix_LoadMUS("megaman.mp3");
    jump =  Mix_LoadWAV("jump.wav");
    kick =  Mix_LoadWAV("kick.wav");
    punch = Mix_LoadWAV("punch.wav");
    Mix_PlayMusic(music, -1);
    glViewport(0, 0, 640, 360);
    program = new ShaderProgram(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
    projectionMatrix.setOrthoProjection(-4.0f, 4.0f, -2.25f, 2.25f, -4.5f, 4.5f);
    glUseProgram(program->programID);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    gameTexture = LoadTexture("arne_sprites.png");
    characterSprite = LoadTexture("mario_sprites.png");
    starting = {
        {348.0f/1024.0f, 425.0f/1024.0f, 76.0f/1024.0f, 90.0f/1024.0f},
        {283.0f/1024.0f, 75.0f/1024.0f, 76.0f/1024.0f, 93.0f/1024.0f},
        {349.0f/1024.0f, 517.0f/1024.0f, 76.0f/1024.0f, 93.0f/1024.0f},
        {271.0f/1024.0f, 876.0f/1024.0f, 76.0f/1024.0f, 90.0f/1024.0f},
        
    };
    
    walking = {
        {272.0f/1024.0f, 624.0f/1024.0f, 76.0f/1024.0f, 89.0f/1024.0f},
        {206.0f/1024.0f, 0.0f/1024.0f, 78.0f/1024.0f, 73.0f/1024.0f},
        {190.0f/1024.0f, 687.0f/1024.0f, 80.0f/1024.0f, 89.0f/1024.0f},
        {426.0f/1024.0f, 381.0f/1024.0f, 74.0f/1024.0f, 94.0f/1024.0f}
    };
    
    walkingB = {
        {427.0f/1024.0f, 477.0f/1024.0f, 74.0f/1024.0f, 99.0f/1024.0f},
        {353.0f/1024.0f, 170.0f/1024.0f, 75.0f/1024.0f, 101.0f/1024.0f},
        {351.0f/1024.0f, 280.0f/1024.0f, 75.0f/1024.0f, 99.0f/1024.0f},
        {275.0f/1024.0f, 185.0f/1024.0f, 76.0f/1024.0f, 93.0f/1024.0f},
        {191.0f/1024.0f, 350.0f/1024.0f, 80.0f/1024.0f, 89.0f/1024.0f}
    };
    
    jumping = {
        {558.0f/1024.0f, 786.0f/1024.0f, 53.0f/1024.0f, 136.0f/1024.0f},
        {428.0f/1024.0f, 273.0f/1024.0f, 65.0f/1024.0f, 100.0f/1024.0f},
        {557.0f/1024.0f, 90.0f/1024.0f, 55.0f/1024.0f, 68.0f/1024.0f}
    };
    
    hit = {
        {427.0f/1024.0f, 821.0f/1024.0f, 67.0f/1024.0f, 102.0f/1024.0f},
        {123.0f/1024.0f, 0.0f/1024.0f, 81.0f/1024.0f, 89.0f/1024.0f},
        {493.0f/1024.0f, 925.0f/1024.0f, 58.0f/1024.0f, 90.0f/1024.0f}
    };


    
}

void Setup(){
    switch(currentState){
        case INTRO:
            break;
        case LEVEL_1:
            Setup1();
            break;
        case INTRO_2:
            break;
        case LEVEL_2:
            Setup2();
            break;
        case INTRO_3:
            break;
        case LEVEL_3:
            Setup3();
            break;
        case FINISH:
            break;
    }
}

void ProcessEvents(){
    switch(currentState){
        case INTRO:
            ProcessEventsIntro();
            break;
        case LEVEL_1:
            if(!init){
                ProcessEventsGame();
            }
            break;
        case INTRO_2:
            ProcessEventsIntro2();
            break;
        case LEVEL_2:
            if(!init){
                ProcessEventsGame();
            }
            break;
        case INTRO_3:
            ProcessEventsIntro3();
            break;
        case LEVEL_3:
            if(!init){
                ProcessEventsGame();
            }
            break;
        case FINISH:
            ProcessEventsFinal();
            break;
    }
}

void Update(){
    switch(currentState){
        case INTRO:
            break;
        case LEVEL_1:
            if(!init){
                UpdateGame();
            }
            break;
        case INTRO_2:
            break;
        case LEVEL_2:
            if(!init){
                UpdateGame();
            }
            break;
        case INTRO_3:
            break;
        case LEVEL_3:
            if(!init){
                UpdateGame();
            }
            break;
        case FINISH:
            break;
    }
}

void Render(){
    switch(currentState){
        case INTRO:
            RenderIntro();
            break;
        case LEVEL_1:
            if(!init){
                RenderGame();
            }
            break;
        case INTRO_2:
            RenderIntro2();
        case LEVEL_2:
            if(!init){
                RenderGame();
            }
            break;
        case INTRO_3:
            RenderIntro3();
            break;
        case LEVEL_3:
            if(!init){
                RenderGame();
            }
            break;
        case FINISH:
            RenderFinal();
            break;
    }
}


int main(int argc, char *argv[])
{
    SetupInit();
    
    while (!done) {
        Setup();
        ProcessEvents();
        Update();
        Render();
        
    }
    Mix_FreeMusic(music);
    Mix_FreeChunk(kick);
    Mix_FreeChunk(punch);
    Mix_FreeChunk(jump);
    SDL_Quit();
    return 0;
}
