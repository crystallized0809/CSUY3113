#include "Level1.h"
#define LEVEL1_WIDTH 40
#define LEVEL1_HEIGHT 8
#define LEVEL1_ENEMY_COUNT 5
unsigned int level1_data[] =
{
    14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
    14, 11, 14, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 14, 14, 11, 11, 11, 11, 11, 11, 12, 11, 12, 14, 11, 11, 11, 11, 12, 14,
    14, 11, 14, 11, 11, 11, 11, 14, 14, 11, 14, 14, 14, 14, 11, 14, 14, 14, 11, 14, 14, 11, 14, 11, 11, 14, 14, 14, 14, 11, 12, 11, 11, 14, 11, 11, 11, 11, 12, 14,
    14, 11, 11, 11, 14, 14, 11, 11, 11, 11, 11, 14, 11, 14, 11, 14, 11, 11, 11, 14, 14, 11, 14, 14, 14, 11, 11, 11, 14, 11, 12, 12, 11, 11, 11, 11, 11, 11, 11, 50,
    14, 11, 11, 14, 14, 14, 14, 14, 14, 11, 14, 14, 11, 11, 11, 14, 14, 14, 14, 14, 11, 11, 11, 11, 11, 11, 11, 11, 14, 11, 11, 11, 11, 14, 11, 11, 11, 11, 11, 50,
    14, 11, 11, 11, 11, 11, 11, 11, 14, 11, 11, 11, 11, 14, 14, 14, 14, 14, 11, 11, 11, 14, 14, 14, 14, 14, 14, 11, 14, 14, 14, 14, 14, 14, 11, 11, 11, 11, 12, 14,
    14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 11, 11, 14, 14, 11, 11, 11, 11, 14, 14, 14, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 12, 14,
    14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14
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
    
    //initialize coins
    state.coins= new Entity[10];
    GLuint coinTextureID = Util::LoadTexture("coin.png");
    for (int i = 0; i < 10; i++){
        state.coins[i].entityType = COIN;
        state.coins[i].textureID = coinTextureID;
        state.coins[i].width = 0.8f;
    }
    state.coins[0].position = glm::vec3(7, -5, 0);
    state.coins[1].position = glm::vec3(1, -1, 0);
    state.coins[2].position = glm::vec3(10, -3, 0);
    state.coins[3].position = glm::vec3(16, -3, 0);
    state.coins[4].position = glm::vec3(15, -6, 0);
    state.coins[5].position = glm::vec3(31, -1, 0);
    state.coins[6].position = glm::vec3(21, -2, 0);
    state.coins[7].position = glm::vec3(13, -4, 0);
    state.coins[8].position = glm::vec3(23, -2, 0);
    state.coins[9].position = glm::vec3(22, -6, 0);
    //initialize enemies
    state.enemies = new Entity[LEVEL1_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("enemy.png");
    for(int i = 0; i < LEVEL1_ENEMY_COUNT; i++){
        state.enemies[i].entityType = ENEMY;
        state.enemies[i].textureID = enemyTextureID;
        state.enemies[i].movement = glm::vec3(0);
        state.enemies[i].speed = 0.3;
        state.enemies[i].width = 0.8f;
    }
    state.enemies[0].position = glm::vec3(1, -2, 0);
    state.enemies[0].aiType = WALKER;
    state.enemies[1].aiType = PATROL;
    state.enemies[1].aiState = WALKING;
    state.enemies[1].position = glm::vec3(6, -3, 0);
    state.enemies[2].position = glm::vec3(12, -3, 0);
    state.enemies[2].aiType = WALKER;
    state.enemies[3].aiType = PATROL;
    state.enemies[3].aiState = WALKING;
    state.enemies[3].position = glm::vec3(11, -1, 0);
    state.enemies[3].aiType = PATROL;
    state.enemies[3].aiState = WALKING;
    state.enemies[3].position = glm::vec3(11, -1, 0);
    state.enemies[4].aiType = PATROL;
    state.enemies[4].aiState = WALKING;
    state.enemies[4].position = glm::vec3(27, -4, 0);

}
void Level1::Update(float deltaTime) {
    for(int i = 0; i < LEVEL1_ENEMY_COUNT; i++){
        if(state.enemies[i].alive){
            state.enemies[i].Update(deltaTime, state.player, state.enemies, state.coins, state.queen, LEVEL1_ENEMY_COUNT, state.map);
        }
    }
    for(int i = 0; i < 10; i++){
        if(state.coins[i].isActive){
            state.coins[i].Update(deltaTime, state.player, state.enemies, state.coins, state.queen, LEVEL1_ENEMY_COUNT, state.map);
        }
    }
    state.player->Update(deltaTime, state.player, state.enemies, state.coins, state.queen, LEVEL1_ENEMY_COUNT, state.map);
    
    if(state.player->coinsCollected == 10 && state.map->end){
        state.nextScene = 1;
    }
}
void Level1::Render(ShaderProgram *program) {
    state.map->Render(program);
    state.player->Render(program);
    for(int i = 0; i < LEVEL1_ENEMY_COUNT; i++){
        state.enemies[i].Render(program);
    }
    for(int i = 0; i < 10; i++){
        state.coins[i].Render(program);
    }
}
