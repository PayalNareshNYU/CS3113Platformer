#include "Level1.h"
#include "Util.h"
#include <SDL_mixer.h>

#define LEVEL1_WIDTH 23
#define LEVEL1_HEIGHT 8

#define LEVEL1_ENEMY_COUNT 1
#define BULLET_COUNT 1
#define ENEMY_BULLET_COUNT 1

Mix_Chunk* musicDoor1;
GLuint mapTextureID;

unsigned int level1_data[] =
{
     3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1,
     3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2, 2,
     3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 2, 2, 2, 2,
     3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2,
     3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
};

void Level1::Initialize() {
    state.nextScene = -1;

    mapTextureID = Util::LoadTexture("tileset.png");
    state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 4, 1);

    state.fontTexture = new Entity();
    state.fontTexture->textureID = Util::LoadTexture("font1.png");

    // Initialize Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(5, 0, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, -9.81f, 0);
    state.player->speed = 2.0f;
    state.player->textureID = Util::LoadTexture("george_0.png");

    state.level1DoorBottom = new Entity();
    state.level1DoorTop = new Entity();

    state.level1DoorBottom->textureID = Util::LoadTexture("platformPack_tile060.png");
    state.level1DoorTop->textureID = Util::LoadTexture("platformPack_tile051.png");

    state.level1DoorBottom->position = glm::vec3(21.0f, -2.0f, 0);
    state.level1DoorTop->position = glm::vec3(21.0f, -1.0f, 0);

    state.player->animRight = new int[4]{ 3, 7, 11, 15 };
    state.player->animLeft = new int[4]{ 1, 5, 9, 13 };
    state.player->animUp = new int[4]{ 2, 6, 10, 14 };
    state.player->animDown = new int[4]{ 0, 4, 8, 12 };

    state.player->animIndices = state.player->animRight;
    state.player->animFrames = 4;
    state.player->animIndex = 0;
    state.player->animTime = 0;
    state.player->animCols = 4;
    state.player->animRows = 4;

    state.player->height = 0.8f;
    state.player->width = 0.4f;

    state.player->jumpPower = 5.0f;

    state.enemies = new Entity[LEVEL1_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("ctg.png");

    state.enemies[0].entityType = ENEMY;
    state.enemies[0].textureID = enemyTextureID;
    state.enemies[0].position = glm::vec3(14.5f, -5.0f, 0);
    state.enemies[0].speed = 1;
    state.enemies[0].aiType = STATIONARYSHOOTER;
    state.enemies[0].aiState = SHOOTLEFT;

    GLuint bulletID = Util::LoadTexture("ballYellowFinal.png");

    state.bullets = new Entity();
    state.enemyBullets = new Entity();

    state.bullets->entityType = BULLET;
    state.bullets->height = 0.3f;
    state.bullets->width = 0.3f;
    state.bullets->textureID = bulletID;
    state.bullets->position = state.player->position;
    state.bullets->isActive = false;

    state.enemyBullets->entityType = ENEMYBULLET;
    state.enemyBullets->height = 0.3f;
    state.enemyBullets->width = 0.3f;
    state.enemyBullets->textureID = bulletID;
    state.enemyBullets->position = state.enemies[0].position;
    state.enemyBullets->speed = 0.50f;
    state.enemyBullets->shootNow = true;
}

void Level1::Update(float deltaTime) {
    state.player->Update(deltaTime, NULL, NULL, state.player, state.enemies, LEVEL1_ENEMY_COUNT, state.bullets, state.enemyBullets, BULLET_COUNT, ENEMY_BULLET_COUNT, state.map);
    state.enemies[0].Update(deltaTime, NULL, NULL, state.player, state.enemies, LEVEL1_ENEMY_COUNT, state.bullets, state.enemyBullets, BULLET_COUNT, ENEMY_BULLET_COUNT, state.map);
    
    state.bullets->Update(deltaTime, NULL, NULL, state.player, state.enemies, LEVEL1_ENEMY_COUNT, state.bullets, state.enemyBullets, BULLET_COUNT, ENEMY_BULLET_COUNT, state.map);
    state.enemyBullets->Update(deltaTime, NULL, NULL, state.player, state.enemies, LEVEL1_ENEMY_COUNT, state.bullets, state.enemyBullets, BULLET_COUNT, ENEMY_BULLET_COUNT, state.map);
    
    state.level1DoorBottom->Update(deltaTime, NULL, NULL, state.player, state.enemies, LEVEL1_ENEMY_COUNT, state.bullets, state.enemyBullets, BULLET_COUNT, ENEMY_BULLET_COUNT, state.map);
    state.level1DoorTop->Update(deltaTime, NULL, NULL, state.player, state.enemies, LEVEL1_ENEMY_COUNT, state.bullets, state.enemyBullets, BULLET_COUNT, ENEMY_BULLET_COUNT, state.map);
    
    if (state.player->position.x >= 20.5f) {
        musicDoor1 = Mix_LoadWAV("success.wav");
        Mix_PlayChannel(-1, musicDoor1, 0);
        state.nextScene = 1;
    }

    if ((state.player->goBack == true) && (Global::lives > 0)) {
        state.player->position = glm::vec3(5, 0, 0);
        state.player->isActive = true;
        state.player->goBack = false;
    }

    if (Global::lives == 0) {
        state.player->hasWon = "No";
        state.player->isActive = false;
        state.player->goBack = false;
        state.player->livesFreeze = true;
    }
}
void Level1::Render(ShaderProgram* program) {
    state.map->Render(program);
    state.player->Render(program);
    state.enemies[0].Render(program);
    state.bullets->Render(program);
    state.enemyBullets->Render(program);
    state.level1DoorBottom->Render(program);
    state.level1DoorTop->Render(program);

    if (state.player->hasWon == "No") {
        Util::DrawText(program, state.fontTexture->textureID, "GAME OVER!", 0.8f, -0.25f, glm::vec3(state.player->position.x - 2.2f, -2.0f, 0));
    }
}