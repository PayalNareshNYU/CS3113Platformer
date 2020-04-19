#include "Level2.h"
#include "Util.h"
#include <SDL_mixer.h>
#define LEVEL2_WIDTH 25
#define LEVEL2_HEIGHT 8
#define ENEMY_COUNT 1

#define LEVEL2_ENEMY_COUNT 2
#define BULLET_COUNT 1
#define ENEMY_BULLET_COUNT 1

Mix_Chunk* musicDoor2;

unsigned int level2_data[] =
{
     3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     3, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     3, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3,
     3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 3, 3, 3,
     3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3,
     3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
};

void Level2::Initialize() {

    state.nextScene = -1;

    GLuint mapTextureID = Util::LoadTexture("tileset.png");
    state.map = new Map(LEVEL2_WIDTH, LEVEL2_HEIGHT, level2_data, mapTextureID, 1.0f, 4, 1);

    state.fontTexture = new Entity();
    state.fontTexture->textureID = Util::LoadTexture("font1.png");

    // Initialize Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(2, 0, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, -9.81f, 0);
    state.player->speed = 2.0f;
    state.player->textureID = Util::LoadTexture("george_0.png");

    state.level1DoorBottom = new Entity();
    state.level1DoorTop = new Entity();

    state.level1DoorBottom->textureID = Util::LoadTexture("platformPack_tile060.png");
    state.level1DoorTop->textureID = Util::LoadTexture("platformPack_tile051.png");

    state.level1DoorBottom->position = glm::vec3(23.5f, -3.0f, 0);
    state.level1DoorTop->position = glm::vec3(23.5f, -2.0f, 0);

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

    state.player->jumpPower = 6.0f;

    state.enemies = new Entity[LEVEL2_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("ctg.png");

    state.enemies[0].entityType = ENEMY;
    state.enemies[0].textureID = enemyTextureID;
    state.enemies[0].position = glm::vec3(18, -6.0f, 0);
    state.enemies[0].speed = 1;
    state.enemies[0].aiType = STATIONARYSHOOTER;
    state.enemies[0].aiState = IDLE;
    
    state.enemies[1].entityType = ENEMY;
    state.enemies[1].textureID = enemyTextureID;
    state.enemies[1].position = glm::vec3(5, -5.0f, 0);
    state.enemies[1].speed = 1;
    state.enemies[1].aiType = WALKER1;
    state.enemies[1].aiState = WALKING;

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
    state.enemyBullets->position = glm::vec3(18, -6.0f, 0);
    state.enemyBullets->speed = 0.80f;
    state.enemyBullets->shootNow = true;
    state.enemyBullets->isActive = true;
}

void Level2::Update(float deltaTime) {
    state.player->Update(deltaTime, NULL, NULL, state.player, state.enemies, LEVEL2_ENEMY_COUNT, state.bullets, state.enemyBullets, BULLET_COUNT, ENEMY_BULLET_COUNT, state.map);
    state.enemies[0].Update(deltaTime, NULL, NULL, state.player, state.enemies, LEVEL2_ENEMY_COUNT, state.bullets, state.enemyBullets, BULLET_COUNT, ENEMY_BULLET_COUNT, state.map);
    state.enemies[1].Update(deltaTime, NULL, NULL, state.player, state.enemies, LEVEL2_ENEMY_COUNT, state.bullets, state.enemyBullets, BULLET_COUNT, ENEMY_BULLET_COUNT, state.map);
    state.bullets->Update(deltaTime, NULL, NULL, state.player, state.enemies, LEVEL2_ENEMY_COUNT, state.bullets, state.enemyBullets, BULLET_COUNT, ENEMY_BULLET_COUNT, state.map);
    state.enemyBullets->Update(deltaTime, NULL, NULL, state.player, state.enemies, LEVEL2_ENEMY_COUNT, state.bullets, state.enemyBullets, BULLET_COUNT, ENEMY_BULLET_COUNT, state.map);

    state.level1DoorBottom->Update(deltaTime, NULL, NULL, state.player, state.enemies, LEVEL2_ENEMY_COUNT, state.bullets, state.enemyBullets, BULLET_COUNT, ENEMY_BULLET_COUNT, state.map);
    state.level1DoorTop->Update(deltaTime, NULL, NULL, state.player, state.enemies, LEVEL2_ENEMY_COUNT, state.bullets, state.enemyBullets, BULLET_COUNT, ENEMY_BULLET_COUNT, state.map);

    if (state.player->position.x >= 23.5f) {
        musicDoor2 = Mix_LoadWAV("success.wav");
        Mix_PlayChannel(-1, musicDoor2, 0);
        state.nextScene = 2;
    }

    if ((state.player->goBack == true) && (Global::lives > 0)) {
        state.player->position = glm::vec3(2, 0, 0);
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

void Level2::Render(ShaderProgram* program) {
    state.map->Render(program);
    state.player->Render(program);
    state.enemies[0].Render(program);
    state.enemies[1].Render(program);
    state.bullets->Render(program);
    state.enemyBullets->Render(program);
    state.level1DoorBottom->Render(program);
    state.level1DoorTop->Render(program);

    if (state.player->hasWon == "No") {
        Util::DrawText(program, state.fontTexture->textureID, "GAME OVER!", 0.8f, -0.25f, glm::vec3(state.player->position.x - 2.2f, -2.0f, 0));
    }
}