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
enum EntityType {SHIP, GATE, WALL};
class Entity {
public:
    EntityType entityType;
    glm::vec3 position;
    glm::vec3 movement;
    glm::vec3 acceleration;
    glm::vec3 velocity;
    float speed;
    EntityType lastCollision;
    float width = 1;
    float height = 1;
    GLuint textureID;
    
    glm::mat4 modelMatrix;
    
    int *animIndices = NULL;
    int animFrames = 0;
    int animIndex = 0;
    float animTime = 0;
    int animCols = 0;
    int animRows = 0;
    bool isActive = true;
    
    Entity();
    
    bool CheckCollision(Entity *other);
    void Update(float deltaTime, Entity *platforms, int platformCount);
    void Render(ShaderProgram *program);
    void CheckCollisionsY(Entity *objects, int objectCount);
    void CheckCollisionsX(Entity *objects, int objectCount);
};
