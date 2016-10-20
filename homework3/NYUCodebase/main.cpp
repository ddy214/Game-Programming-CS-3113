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
std::vector<Entity> enemies;
bool done = false;
GLuint letterTextures;
GLuint shipTextures;

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
    projectionMatrix.setOrthoProjection(-5.33f, 5.33f, -3.0f, 3.0f, -1.0f, 1.0f);
    glUseProgram(program->programID);
    letterTextures = LoadTexture("font1.png");
    shipTextures = LoadTexture("sheet.png");
    SheetSprite shipSprite(shipTextures, 211.0f/1024.0f, 941.0f/1024.0f, 99.0f/1024.0f, 75.0f/1024.0f,0.2);
    ship = new Entity(0, 0, shipSprite);
    
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
    while(SDL_PollEvent(&event)){
        if(event.type == SDL_QUIT|| event.type == SDL_WINDOWEVENT_CLOSE) {
            done = true;
        }
        else if (event.type == SDL_KEYDOWN){
            if(event.key.keysym.scancode == SDL_SCANCODE_SPACE){
                
            }
        }
    }
    
    if(keys[SDL_SCANCODE_RIGHT]){
        
    }
    
    else if(keys[SDL_SCANCODE_LEFT]){
        
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
    
}

void UpdateGame(){
    
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
    program->setModelMatrix(modelMatrix);
    std::string welcome = "Welcome to Space Invaders";
    DrawText(program, letterTextures, welcome, .5f, 0.05f);
}

void RenderGame(){
    ship->Render(program);
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

