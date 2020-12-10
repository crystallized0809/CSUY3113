#include "Level2.h"
#define LEVEL2_WIDTH 14
#define LEVEL2_HEIGHT 8
unsigned int level2_data[] =
{
    12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
    12, 11, 11, 11, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
    12, 11, 11, 11, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
    12, 11, 11, 11, 12, 11, 11, 11, 11, 12, 12, 12, 12, 12,
    12, 11, 11, 11, 11, 11, 11, 11, 11, 12, 12, 12, 12, 12,
    12, 11, 11, 11, 12, 11, 11, 11, 11, 12, 12, 12, 12, 12,
    12, 11, 11, 11, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
    12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12
};
void Level2::Initialize() {
    state.nextScene = -1;
    GLuint mapTextureID = Util::LoadTexture("tileset.png");
    state.map = new Map(LEVEL2_WIDTH, LEVEL2_HEIGHT, level2_data, mapTextureID, 1.0f, 8, 15);
    // Initialize Game Objects
    state.queen = new Entity();
    state.queen -> position = glm::vec3(7, -4, 0);
    state.queen->textureID = Util::LoadTexture("Sara-VT.png");
    state.queen->height = 1.5f;
    state.queen -> width = 0.8f;
    
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(1, -5, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, 0, 0);
    state.player->speed = 2;
    state.player->textureID = Util::LoadTexture("ada_0.png");
    
    state.player->animRight = new int[4] {3, 7, 11, 15};
    state.player->animLeft = new int[4] {1, 5, 9, 13};
    state.player->animUp = new int[4] {2, 6, 10, 14};
    state.player->animDown = new int[4] {0, 4, 8, 12};
    state.player->animIndices = state.player->animRight;
    state.player->animFrames = 4;
    state.player->animIndex = 0;
    state.player->animTime = 0;
    state.player->animCols = 4;
    state.player->animRows = 4;
    
    state.player -> height = 0.8f;
    state.player -> width = 0.5f;
   
}
void Level2::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.enemies, state.coins, state.queen, 0, state.map);
    state.queen -> Update(deltaTime, state.player, state.enemies, state.coins, state.queen, 0, state.map);
    if(state.player->missionAccomplished){
        state.nextScene = 2;
    }
}
void Level2::Render(ShaderProgram *program) {
    state.map->Render(program);
    state.player->Render(program);
    state.queen ->Render(program);
}
