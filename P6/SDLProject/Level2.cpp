#include "Level2.h"
#define LEVEL2_WIDTH 14
#define LEVEL2_HEIGHT 8
#define LEVEL2_ENEMY_COUNT 2
unsigned int level2_data[] =
{
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 0, 0,
    3, 0, 0, 0, 0, 3, 3, 0, 0, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3
};
void Level2::Initialize() {
    state.nextScene = -1;
    GLuint mapTextureID = Util::LoadTexture("tileset.png");
    state.map = new Map(LEVEL2_WIDTH, LEVEL2_HEIGHT, level2_data, mapTextureID, 1.0f, 4, 1);
    // Initialize Game Objects
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(5, 0, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, -9.81, 0);
    state.player->speed = 2;
    state.player->textureID = Util::LoadTexture("character_sprite.png");
    
    state.player->animRight = new int[4] {12, 13, 14, 15};
    state.player->animLeft = new int[4] {8, 9, 10, 11};
    state.player->animUp = new int[4] {4, 5, 6, 7};
    state.player->animDown = new int[4] {0, 1, 2, 3};
    
    state.player->animIndices = state.player->animRight;
    state.player->animFrames = 4;
    state.player->animIndex = 0;
    state.player->animTime = 0;
    state.player->animCols = 4;
    state.player->animRows = 4;
    
    state.player -> height = 0.9f;
    state.player -> width = 0.8f;
    
    //initialize enemies
    state.enemies = new Entity[LEVEL2_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("enemy.png");
    for(int i = 0; i < LEVEL2_ENEMY_COUNT; i++){
        state.enemies[i].entityType = ENEMY;
        state.enemies[i].textureID = enemyTextureID;
        state.enemies[i].acceleration = glm::vec3(0, -9.81, 0);
        state.enemies[i].movement = glm::vec3(0);
        state.enemies[i].speed = 0.5;
        state.enemies[i].width = 0.8f;
    }
    state.enemies[0].position = glm::vec3(7, 0, 0);
    state.enemies[0].aiType = PATROL;
    state.enemies[0].aiState = WALKING;
    state.enemies[0].movement = glm::vec3(-1, 0, 0);
    state.enemies[1].position = glm::vec3(9, 0, 0);
    state.enemies[1].aiType = WALKER;
    state.enemies[1].aiState = WALKING;

}
void Level2::Update(float deltaTime) {
    for(int i = 0; i < LEVEL2_ENEMY_COUNT; i++){
        if(state.enemies[i].alive){
            state.enemies[i].Update(deltaTime, state.player, state.enemies, state.coins, state.queen, LEVEL2_ENEMY_COUNT, state.map);
        }
    }
    state.player->Update(deltaTime, state.player, state.enemies, state.coins, state.queen, LEVEL2_ENEMY_COUNT, state.map);
    if(state.player->position.x >= 12){
        state.nextScene = 2;
    }
}
void Level2::Render(ShaderProgram *program) {
    state.map->Render(program);
    state.player->Render(program);
    for(int i = 0; i < LEVEL2_ENEMY_COUNT; i++){
        state.enemies[i].Render(program);
    }
}
