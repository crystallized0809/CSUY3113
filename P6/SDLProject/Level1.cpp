#include "Level1.h"
#define LEVEL1_WIDTH 30
#define LEVEL1_HEIGHT 8
#define LEVEL1_ENEMY_COUNT 3
unsigned int level1_data[] =
{
    14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
    14, 11, 14, 14, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 12, 12, 12, 12, 12, 14,
    14, 11, 11, 11, 11, 11, 11, 14, 14, 11, 14, 14, 14, 14, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 12, 14,
    14, 11, 11, 11, 14, 14, 11, 11, 11, 11, 11, 14, 11, 14, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 12, 14,
    14, 11, 11, 14, 14, 14, 14, 11, 11, 11, 14, 14, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 50,
    14, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 14, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 12, 14,
    14, 11, 11, 11, 11, 11, 11, 11, 11, 14, 14, 11, 11, 14, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 12, 12, 12, 12, 12, 14,
    14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
};
void Level1::Initialize() {
    state.nextScene = -1;
    GLuint mapTextureID = Util::LoadTexture("tileset.png");
    state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 8, 15);
    // Initialize Game Objects
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(1, -5, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, 0, 0);
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
    
    //initialize coins
    state.coins= new Entity[5];
    GLuint coinTextureID = Util::LoadTexture("coin.png");
    for (int i = 0; i < 5; i++){
        state.coins[i].entityType = COIN;
        state.coins[i].textureID = coinTextureID;
        state.coins[i].width = 0.8f;
    }
    state.coins[0].position = glm::vec3(5, -2, 0);
    state.coins[1].position = glm::vec3(9, -1, 0);
    state.coins[2].position = glm::vec3(13, -4, 0);
    state.coins[3].position = glm::vec3(15, -3, 0);
    state.coins[4].position = glm::vec3(20, -2, 0);
    //initialize enemies
    state.enemies = new Entity[LEVEL1_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("enemy.png");
    for(int i = 0; i < LEVEL1_ENEMY_COUNT; i++){
        state.enemies[i].entityType = ENEMY;
        state.enemies[i].textureID = enemyTextureID;
        state.enemies[i].movement = glm::vec3(0);
        state.enemies[i].speed = 0.5;
        state.enemies[i].width = 0.8f;
    }
    state.enemies[0].position = glm::vec3(10, -3, 0);
    state.enemies[0].aiType = PATROL;
    state.enemies[0].movement = glm::vec3(-1, 0, 0);
    state.enemies[0].aiState = WALKING;
}
void Level1::Update(float deltaTime) {
    for(int i = 0; i < LEVEL1_ENEMY_COUNT; i++){
        if(state.enemies[i].alive){
            state.enemies[i].Update(deltaTime, state.player, state.enemies, state.coins, state.queen, LEVEL1_ENEMY_COUNT, state.map);
        }
    }
    for(int i = 0; i < 5; i++){
        if(state.coins[i].isActive){
            state.coins[i].Update(deltaTime, state.player, state.enemies, state.coins, state.queen, LEVEL1_ENEMY_COUNT, state.map);
        }
    }
    state.player->Update(deltaTime, state.player, state.enemies, state.coins, state.queen, LEVEL1_ENEMY_COUNT, state.map);
    
    if(state.player->coinsCollected == 5){
        state.nextScene = 1;
    }
}
void Level1::Render(ShaderProgram *program) {
    state.map->Render(program);
    state.player->Render(program);
    for(int i = 0; i < LEVEL1_ENEMY_COUNT; i++){
        state.enemies[i].Render(program);
    }
    for(int i = 0; i < 5; i++){
        state.coins[i].Render(program);
//        if(state.coins[i].isActive){
//            state.coins[i].Render(program);
//        }
    }
}
