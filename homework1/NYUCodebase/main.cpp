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

SDL_Window* displayWindow;

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

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
#ifdef _WINDOWS
    glewInit();
#endif
    
    SDL_Event event;
    bool done = false;
    glViewport(0, 0, 640, 360);
    ShaderProgram program(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
    
    GLuint shipTexture = LoadTexture("playerShip1_blue.png");
    GLuint shipTexture2 = LoadTexture("playerShip2_red.png");
    GLuint shipTexture3 = LoadTexture("playerShip3_orange.png");
    
    
    Matrix projectionMatrix;
    Matrix modelMatrix;
    Matrix viewMatrix;
    
    float lastFrameTicks = 0.0f;
    
    projectionMatrix.setOrthoProjection(-3.55, 3.55, -2.0f, 2.0f, -1.0f, 1.0f);
    glUseProgram(program.programID);
    
    
    while (!done) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
                done = true;
            }
        }
        glClear(GL_COLOR_BUFFER_BIT);
        program.setModelMatrix(modelMatrix);
        program.setProjectionMatrix(projectionMatrix);
        program.setViewMatrix(viewMatrix);
        
        //binding ship1 texture
        
        float ticks = (float) SDL_GetTicks()/1000.0f;
        
        float elapsed = ticks - lastFrameTicks;
        
        lastFrameTicks = ticks;
        
        modelMatrix.identity();
        
        modelMatrix.Translate(0,lastFrameTicks,0);
        
        program.setModelMatrix(modelMatrix);
        
        glBindTexture(GL_TEXTURE_2D, shipTexture);
        
        float ship1Vert[] = {-2.5, -0.5, -1.5, -0.5, -1.5, 0.5, -2.5, -0.5, -1.5, 0.5, -2.5, 0.5};
        
        glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, ship1Vert);
        
        glEnableVertexAttribArray(program.positionAttribute);
        
        float ship1Text[] = {0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0};
        
        glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, ship1Text);
        
        glEnableVertexAttribArray(program.texCoordAttribute);
        
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        glDisableVertexAttribArray(program.positionAttribute);
        
        glDisableVertexAttribArray(program.texCoordAttribute);
        
        //binding ship2 texture
        
        modelMatrix.identity();
        
        program.setModelMatrix(modelMatrix);
        
        glBindTexture(GL_TEXTURE_2D, shipTexture2);
        
        float ship2Vert[] = {-0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5};
        
        glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, ship2Vert);
        
        glEnableVertexAttribArray(program.positionAttribute);
        
        float ship2Text[] = {0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0};
        
        glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, ship2Text);
        
        glEnableVertexAttribArray(program.texCoordAttribute);
        
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        glDisableVertexAttribArray(program.positionAttribute);
        
        glDisableVertexAttribArray(program.texCoordAttribute);
        
        //binding ship3 texture
        
        glBindTexture(GL_TEXTURE_2D, shipTexture3);
        
        float ship3Vert[] = {1.5, -0.5, 2.5, -0.5, 2.5, 0.5, 1.5, -0.5, 2.5, 0.5, 1.5, 0.5};
        
        glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, ship3Vert);
        
        glEnableVertexAttribArray(program.positionAttribute);
        
        float ship3Text[] = {0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0};
        
        glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, ship3Text);
        
        glEnableVertexAttribArray(program.texCoordAttribute);
        
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        glDisableVertexAttribArray(program.positionAttribute);
        
        glDisableVertexAttribArray(program.texCoordAttribute);
        

        

        SDL_GL_SwapWindow(displayWindow);
    }
    
    SDL_Quit();
    return 0;
}
