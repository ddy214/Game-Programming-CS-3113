#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

#include "ShaderProgram.h"
#include "Matrix.h"

class Entity{
public:
    float x;
    float y;
    float width;
    float height;
    float y_velocity;
    float x_velocity;
    Entity(const float x, const float y, const float width, const float height):x(x), y(y), width(width), height(height){}
};



SDL_Window* displayWindow;
SDL_Event event;
bool done = false;
Matrix projectionMatrix;
Matrix modelMatrix;
Matrix viewMatrix;
ShaderProgram * program;
float lastFrameTicks = 0.0f;
Entity player1 = Entity(-3.35, 0, 0.20, 0.7);
Entity player2 = Entity(3.35, 0, 0.20, 0.7);
Entity ball = Entity(0, 0, 0.10, 0.10);


void Setup(){
    glViewport(0, 0, 640, 360);
    program = new ShaderProgram(RESOURCE_FOLDER"vertex.glsl", RESOURCE_FOLDER"fragment.glsl");
    projectionMatrix.setOrthoProjection(-3.55, 3.55, -2.0f, 2.0f, -1.0f, 1.0f);
    glUseProgram(program->programID);
    ball.x_velocity = 2;
    ball.y_velocity = 2;
    
}

void ProcessEvents(){
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    while(SDL_PollEvent(&event)){
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            done = true;
        }
        else if(event.type == SDL_KEYUP){
            if(event.key.keysym.scancode == SDL_SCANCODE_W){
                player1.y_velocity = 0;
            }
            else if(event.key.keysym.scancode == SDL_SCANCODE_S){
                player1.y_velocity = 0;
            }
            else if(event.key.keysym.scancode == SDL_SCANCODE_UP){
                player2.y_velocity = 0;
            }
            
            else if(event.key.keysym.scancode == SDL_SCANCODE_DOWN){
                player2.y_velocity = 0;
            }
        }
    }
    if(keys[SDL_SCANCODE_UP] && keys[SDL_SCANCODE_W]){
        player2.y_velocity = 4;
        player1.y_velocity = 4;

    }
    else if(keys[SDL_SCANCODE_DOWN] && keys[SDL_SCANCODE_S]){
        player1.y_velocity = -4;
        player2.y_velocity = -4;

        
    }
    else if(keys[SDL_SCANCODE_UP] && keys[SDL_SCANCODE_S]){
        player1.y_velocity = -4;
        player2.y_velocity = 4;

    }
    
    else if(keys[SDL_SCANCODE_DOWN] && keys[SDL_SCANCODE_W]){
        player1.y_velocity = 4;
        player2.y_velocity = -4;

    }
    
    else if(keys[SDL_SCANCODE_UP]){
        player2.y_velocity = 4;
    }
    else if (keys[SDL_SCANCODE_DOWN]){
        player2.y_velocity = -4;
    }
    
    else if(keys[SDL_SCANCODE_W]){
        player1.y_velocity = 4;
    }
    
    else if(keys[SDL_SCANCODE_S]){
        player1.y_velocity = -4;
    }
}

void Update(){
    float ticks = (float) SDL_GetTicks()/1000.0f;
    float elapsed = ticks - lastFrameTicks;
    lastFrameTicks = ticks;
    float ballVelo = -1;
    
    
    if((player1.y + elapsed*player1.y_velocity+player1.height/2)<1.85 && player1.y_velocity > 0){
        player1.y += elapsed*player1.y_velocity;
    }
    if((player2.y + elapsed*player2.y_velocity+player2.height/2)<1.85 && player2.y_velocity > 0){
        player2.y += elapsed*player2.y_velocity;
    }
    
    if((player1.y + elapsed*player1.y_velocity-player1.height/2) > -1.85 && player1.y_velocity < 0){
        player1.y += elapsed*player1.y_velocity;
    }
    
    if((player2.y + elapsed*player2.y_velocity-player2.height/2) > -1.85 && player2.y_velocity < 0){
        player2.y += elapsed*player2.y_velocity;
    }
    
    if(ball.x - ball.width/2 < (player1.x - player1.width/2)){
        ball.x = 0;
        ball.y = 0;
        ball.x_velocity*= ballVelo;
    }
    else if(ball.x + ball.width/2 > (player2.x + player2.width/2)){
        ball.x = 0;
        ball.y = 0;
        ball.x_velocity*= ballVelo;
    }
    
    else if(ball.x + ball.width/2 > (player2.x-player2.width/2) &&  (ball.y - ball.height/2  < (player2.y + player2.height/2) && ball.y + ball.height/2 > (player2.y - player2.height/2))){
        ball.x_velocity *= ballVelo;
        ball.x += elapsed*ball.x_velocity;
    }
    
    else if(ball.x - ball.width/2 < (player1.x+player1.width/2) &&  (ball.y - ball.height/2  < (player1.y + player1.height/2) && ball.y + ball.height/2 > (player1.y - player1.height/2))){
        ball.x_velocity *= ballVelo;
        ball.x += elapsed*ball.x_velocity;
    }
    
    else if(ball.y + ball.height/2 > 1.80 || ball.y-ball.height/2 < -1.80){
        ball.y_velocity *= ballVelo;
        ball.y += ball.y_velocity * elapsed;
    }
    else{
    
        ball.x += elapsed*ball.x_velocity;
        ball.y += elapsed*ball.y_velocity;
    }
    
    
}

void Render(){
    glClear(GL_COLOR_BUFFER_BIT);
    //Set Matrices
    program->setProjectionMatrix(projectionMatrix);
    program->setViewMatrix(viewMatrix);
    
    modelMatrix.Translate(0.0, player1.y, 0.0 );
    program->setModelMatrix(modelMatrix);
    modelMatrix.identity();
    
    //Draw triangles for left bar
    float leftBar[] = {-3.25,-0.35, -3.45, -0.35, -3.45, 0.35, -3.25, -0.35, -3.45, 0.35, -3.25, 0.35};
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, leftBar);
    glEnableVertexAttribArray(program->positionAttribute);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(program->positionAttribute);
    
    modelMatrix.Translate(0.0, player2.y, 0.0 );
    program->setModelMatrix(modelMatrix);
    modelMatrix.identity();
    
    //Draw triangles for right bar
    float rightBar[] = {3.25, -0.35, 3.45, -0.35, 3.45, 0.35, 3.25, -0.35, 3.45, 0.35, 3.25, 0.35 };
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, rightBar);
    glEnableVertexAttribArray(program->positionAttribute);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(program->positionAttribute);
    
    program->setModelMatrix(modelMatrix);
    //Draw Top Border
    float borderTop[] = {-3.45, 1.85, 3.45, 1.85, 3.45, 2.0, -3.45, 1.85, 3.45, 2.0, -3.45, 2.0 };
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, borderTop);
    glEnableVertexAttribArray(program->positionAttribute);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(program->positionAttribute);
    
    
    //Draw Bot Border
    float borderBot[] = {-3.45, -1.85, 3.45, -1.85, 3.45, -2.0, -3.45, -1.85, 3.45, -2.0, -3.45, -2.0 };
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, borderBot);
    glEnableVertexAttribArray(program->positionAttribute);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(program->positionAttribute);
    
    modelMatrix.Translate(ball.x, ball.y, 0.0);
    program->setModelMatrix(modelMatrix);
    modelMatrix.identity();
    //Draw Ball
    float ballVertices[] = {-0.1, -0.1, 0.1, -0.1, 0.1, 0.1, -0.1, -0.1, 0.1, 0.1, -0.1, 0.1 };
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, ballVertices);
    glEnableVertexAttribArray(program->positionAttribute);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(program->positionAttribute);
    
    program->setModelMatrix(modelMatrix);
    //Create middle line
    float startingPoint = 2.0f;
    for(int i = 0; i < 15; i++){
        float lineSquare[] = {-0.1, startingPoint-0.2f, 0.1, startingPoint-0.2f, 0.1, startingPoint, -0.1, startingPoint-0.2f, 0.1, startingPoint, -0.1, startingPoint};
        startingPoint-=0.3;    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, lineSquare);
        glEnableVertexAttribArray(program->positionAttribute);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDisableVertexAttribArray(program->positionAttribute);
        
    }
    
    SDL_GL_SwapWindow(displayWindow);
    
}

void CleanUp(){
    
}

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
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
    CleanUp();
    SDL_Quit();
    return 0;
}
