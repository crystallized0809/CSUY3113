#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL_mixer.h>
#define GL_GLEXT_PROTOTYPES 1
#include <iostream>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include <vector>
#include "Map.h"
#include "Entity.h"
#include "Util.h"
#include "Scene.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"
int lives = 3;
bool gameover = false;
Mix_Chunk *jump;
Mix_Music *music;
GLuint fontTextureID;
SDL_Window* displayWindow;
bool gameIsRunning = true;
ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;
Scene *currentScene;
Scene *sceneList[3];
bool menu = true;
void SwitchToScene(Scene *scene) {
    currentScene = scene;
    currentScene->Initialize();
}
void Initialize() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    displayWindow = SDL_CreateWindow("George's Adventure", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 640, 480);
    
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    
    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    
    glUseProgram(program.programID);
    
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_BLEND);
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //Audio
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    music = Mix_LoadMUS("crypto.mp3");
    Mix_PlayMusic(music, -1);
    jump = Mix_LoadWAV("jump.wav");
    Mix_VolumeMusic(MIX_MAX_VOLUME/4);
    
    fontTextureID = Util::LoadTexture("font1.png");
    sceneList[0] = new Level1();
    sceneList[1] = new Level2();
    sceneList[2] = new Level3();
}

void ProcessInput() {
    if(!menu){
        currentScene->state.player->movement = glm::vec3(0);
    }
    
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                gameIsRunning = false;
                break;
                
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_LEFT:
                        // Move the player left
                        break;
                        
                    case SDLK_RIGHT:
                        // Move the player right
                        break;
                    case SDLK_RETURN:
                        if (menu){
                            SwitchToScene(sceneList[0]);
                            menu = false;
                        }
                        break;
                    case SDLK_SPACE:
                        if(!menu){
                            if(currentScene->state.player -> collidedBottom){
                                currentScene->state.player -> jump = true;
                                Mix_PlayChannel(-1, jump, 0);
                            }
                        }
                        break;
                }
                break; // SDL_KEYDOWN
        }
    }
    
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    
    if (keys[SDL_SCANCODE_LEFT]) {
        if(!menu){
            currentScene->state.player->movement.x = -1.0f;
            currentScene->state.player->animIndices = currentScene->state.player->animLeft;
        }
        
    }
    else if (keys[SDL_SCANCODE_RIGHT]) {
        if(!menu){
            currentScene->state.player->movement.x = 1.0f;
            currentScene->state.player->animIndices = currentScene->state.player->animRight;
        }
    }
    if(!menu){
        if (glm::length(currentScene->state.player->movement) > 1.0f) {
            currentScene->state.player->movement = glm::normalize(currentScene->state.player->movement);
        }
    }
    
}

#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0f;
void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;
    deltaTime += accumulator;
    if (deltaTime < FIXED_TIMESTEP) {
        accumulator = deltaTime;
        return;
    }
    if(!menu){
        while (deltaTime >= FIXED_TIMESTEP) {
            // Update. Notice it's FIXED_TIMESTEP. Not deltaTime
            currentScene->Update(FIXED_TIMESTEP);
            deltaTime -= FIXED_TIMESTEP;
            
        }
        accumulator = deltaTime;
        viewMatrix = glm::mat4(1.0f);
        if (currentScene->state.player->position.x > 5) {
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-currentScene->state.player->position.x, 3.75, 0));
        } else {
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-5, 3.75, 0));
        }
        if(!currentScene->state.player->alive){
            if(lives > 1){
                lives -=1;
                currentScene -> Initialize();
            }
            else{
                gameover = true;
            }
        }
    }
    
}


void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    program.SetViewMatrix(viewMatrix);
    if(!menu){
        currentScene->Render(&program);
    }

    if (menu){
        Util::DrawText(&program, fontTextureID, "George's Adventure", 1, -0.5f, glm::vec3(-4.25f, 2, 0));
        Util::DrawText(&program, fontTextureID, "Press Enter to Start", 0.75, -0.5f, glm::vec3(-2.5f, -1, 0));
    }
    if(!menu){
        if(gameover){
            Util::DrawText(&program, fontTextureID, "YOU LOSE!", 1, -0.5f, currentScene->state.player->position);
        }
        else if (!gameover && currentScene->state.nextScene == 3){
            Util::DrawText(&program, fontTextureID, "YOU WIN!", 1, -0.5f, currentScene->state.player->position);
        }
    }
    SDL_GL_SwapWindow(displayWindow);
}


void Shutdown() {
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();
    
    while (gameIsRunning) {
        ProcessInput();
        Update();
        Render();
        if(!menu && currentScene->state.nextScene < 3){
            if(currentScene -> state.nextScene >= 0) SwitchToScene(sceneList[currentScene->state.nextScene]);
        }
    }
    
    Shutdown();
    return 0;
}
