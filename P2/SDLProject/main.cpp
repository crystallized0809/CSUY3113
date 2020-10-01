#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

SDL_Window* displayWindow;
bool gameIsRunning = true;
bool start = false;
bool end = false;
ShaderProgram program;
glm::mat4 viewMatrix, leftMatrix, rightMatrix, ballMatrix, projectionMatrix;
glm::vec3 left_position, left_movement, right_position, right_movement, ball_position, ball_movement;
float player_speed = 2.0f;

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 640, 480);
    
    program.Load("shaders/vertex.glsl", "shaders/fragment.glsl");
    
    viewMatrix = glm::mat4(1.0f);
    leftMatrix = glm::mat4(1.0f);
    rightMatrix = glm::mat4(1.0f);
    ballMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    program.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
    glUseProgram(program.programID);
    
    ball_movement.x = 0.4f;
    ball_movement.y = 0.6f;
    right_position.x = 4.75f;
    left_position.x = -4.75f;
    rightMatrix = glm::mat4(1.0f);
    rightMatrix = glm::translate(rightMatrix, right_position);
    leftMatrix = glm::mat4(1.0f);
    leftMatrix = glm::translate(leftMatrix, left_position);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

}

void ProcessInput() {
    left_movement = glm::vec3(0);
    right_movement = glm::vec3(0);
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                gameIsRunning = false;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_SPACE:
                        // Start the game
                        start = true;
                }
                break; // SDL_KEYDOWN
        }
    }
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_S]) {
        left_movement.y = -1.0f;
    }
    else if (keys[SDL_SCANCODE_W]) {
        left_movement.y = 1.0f;
    }
    if (keys[SDL_SCANCODE_UP]) {
        right_movement.y = 1.0f;
    }
    else if (keys[SDL_SCANCODE_DOWN]) {
        right_movement.y = -1.0f;
    }
    if (glm::length(left_movement) > 1.0f) {
        left_movement = glm::normalize(left_movement);
    }
    if (glm::length(right_movement) > 1.0f) {
        right_movement = glm::normalize(right_movement);
    }
}

void bounce(){
    float xdistLeft = fabs(left_position.x - ball_position.x) - (0.55f);
    float ydistLeft = fabs(left_position.y - ball_position.y) - (2.5f / 2.0f);
    float xdistRight = fabs(right_position.x - ball_position.x) - (0.55f);
    float ydistRight = fabs(right_position.y - ball_position.y) - (2.5f / 2.0f);
    if ((xdistLeft < 0 && ydistLeft < 0) || (xdistRight < 0 && ydistRight < 0)){ // Colliding!
        ball_movement.x *= -1.0f;
    }
    else if (ball_position.y >= 3.5f || ball_position.y <= -3.5f){
        ball_movement.y *= -1.0f;
    }
    else if (ball_position.x >= 4.75 || ball_position.x <= -4.75){
        end = true;
        start = false;
    }
}
float lastTicks = 0.0f;
void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;
    // Add (direction * units per second * elapsed time)
    if (!end){
        left_position += left_movement * player_speed * deltaTime;
        if (left_position.y < 2.75 && left_position.y > -2.75){
            leftMatrix = glm::mat4(1.0f);
            leftMatrix = glm::translate(leftMatrix, left_position);
        }
        else{
            left_position -= left_movement * player_speed * deltaTime;
        }
        right_position += right_movement * player_speed * deltaTime;
        if (right_position.y < 2.75 && right_position.y > -2.75){
            rightMatrix = glm::mat4(1.0f);
            rightMatrix = glm::translate(rightMatrix, right_position);
        }
        else{
            right_position -= right_movement * player_speed * deltaTime;
        }
        if(start){
            ball_position += ball_movement * 4.0f * deltaTime;
            bounce();
            if (!end){
                ballMatrix = glm::mat4(1.0f);
                ballMatrix = glm::translate(ballMatrix, ball_position);
            }
            
        }
    }
}

void DrawLeft(){
    program.SetModelMatrix(leftMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
void DrawRight(){
    program.SetModelMatrix(rightMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
void DrawBall(){
    program.SetModelMatrix(ballMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    float vertices[] = { -0.25, -0.25, 0.25, -0.25, 0.25, 0.25, -0.25, -0.25, 0.25, 0.25, -0.25, 0.25 };
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    DrawBall();
   
    float paddleVertices[] = { -0.25, -1.0, 0.25, -1.0, 0.25, 1.0, -0.25, -1.0, 0.25, 1.0, -0.25, 1.0 };
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, paddleVertices);
    glEnableVertexAttribArray(program.positionAttribute);
    DrawLeft();
    
    
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, paddleVertices);
    glEnableVertexAttribArray(program.positionAttribute);
    DrawRight();

    
    glDisableVertexAttribArray(program.positionAttribute);
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
    }
    
    Shutdown();
    return 0;
}
