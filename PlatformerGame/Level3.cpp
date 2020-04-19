#include "Level3.h"
#include "Util.h"
#include <SDL_mixer.h>
#define LEVEL3_WIDTH 30
#define LEVEL3_HEIGHT 8
#define ENEMY_COUNT 1

#define LEVEL3_ENEMY_COUNT 2
#define BULLET_COUNT 1
#define ENEMY_BULLET_COUNT 1

#define WATER_PLATFORM_COUNT 12
#define BARGE_COUNT 1

Mix_Chunk* musicDoor3;

unsigned int level3_data[] =
{
     2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0,
     2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2, 0,
     2, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 0,
     2, 2, 2, 2, 2, 0, 2, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 0,
};

void Level3::Initialize() {

    state.nextScene = -1;

    GLuint mapTextureID = Util::LoadTexture("tileset.png");
    state.map = new Map(LEVEL3_WIDTH, LEVEL3_HEIGHT, level3_data, mapTextureID, 1.0f, 4, 1);

    state.fontTexture = new Entity();
    state.fontTexture->textureID = Util::LoadTexture("font1.png");

    // Initialize Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(3, -1, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, -9.81f, 0);
    state.player->speed = 2.0f;
    state.player->textureID = Util::LoadTexture("george_0.png");

    state.level1DoorBottom = new Entity();
    state.level1DoorTop = new Entity();

    state.level1DoorBottom->textureID = Util::LoadTexture("platformPack_tile060.png");
    state.level1DoorTop->textureID = Util::LoadTexture("platformPack_tile051.png");

    state.bargePlatforms = new Entity[BARGE_COUNT];
    state.bargePlatforms[0].entityType = BARGEPLATFORM;
    state.bargePlatforms[0].textureID = Util::LoadTexture("platformPack_tile001.png");
    state.bargePlatforms[0].position = glm::vec3(17, -5, 0);
    state.bargePlatforms[0].movement = glm::vec3(1,0,0);
    state.bargePlatforms[0].speed = 1.0f;
    state.bargePlatforms[0].aiState = MOVING;
    
    state.waterPlatform = new Entity[WATER_PLATFORM_COUNT];

    state.waterPlatform[0].entityType = WATERPLATFORM;
    state.waterPlatform[0].textureID = Util::LoadTexture("platformPack_tile017.png");
    state.waterPlatform[0].position = glm::vec3(5, -6, 0);

    state.waterPlatform[1].entityType = WATERPLATFORM;
    state.waterPlatform[1].textureID = Util::LoadTexture("platformPack_tile017.png");
    state.waterPlatform[1].position = glm::vec3(7, -6, 0);

    state.waterPlatform[2].entityType = WATERPLATFORM;
    state.waterPlatform[2].textureID = Util::LoadTexture("platformPack_tile017.png");
    state.waterPlatform[2].position = glm::vec3(5, -7, 0);

    state.waterPlatform[3].entityType = WATERPLATFORM;
    state.waterPlatform[3].textureID = Util::LoadTexture("platformPack_tile017.png");
    state.waterPlatform[3].position = glm::vec3(7, -7, 0);

    state.waterPlatform[4].entityType = WATERPLATFORM;
    state.waterPlatform[4].textureID = Util::LoadTexture("platformPack_tile017.png");
    state.waterPlatform[4].position = glm::vec3(17, -6, 0);

    state.waterPlatform[5].entityType = WATERPLATFORM;
    state.waterPlatform[5].textureID = Util::LoadTexture("platformPack_tile017.png");
    state.waterPlatform[5].position = glm::vec3(18, -6, 0);

    state.waterPlatform[6].entityType = WATERPLATFORM;
    state.waterPlatform[6].textureID = Util::LoadTexture("platformPack_tile017.png");
    state.waterPlatform[6].position = glm::vec3(19, -6, 0);

    state.waterPlatform[7].entityType = WATERPLATFORM;
    state.waterPlatform[7].textureID = Util::LoadTexture("platformPack_tile017.png");
    state.waterPlatform[7].position = glm::vec3(20, -6, 0);

    state.waterPlatform[8].entityType = WATERPLATFORM;
    state.waterPlatform[8].textureID = Util::LoadTexture("platformPack_tile017.png");
    state.waterPlatform[8].position = glm::vec3(17, -7, 0);

    state.waterPlatform[9].entityType = WATERPLATFORM;
    state.waterPlatform[9].textureID = Util::LoadTexture("platformPack_tile017.png");
    state.waterPlatform[9].position = glm::vec3(18, -7, 0);

    state.waterPlatform[10].entityType = WATERPLATFORM;
    state.waterPlatform[10].textureID = Util::LoadTexture("platformPack_tile017.png");
    state.waterPlatform[10].position = glm::vec3(19, -7, 0);

    state.waterPlatform[11].entityType = WATERPLATFORM;
    state.waterPlatform[11].textureID = Util::LoadTexture("platformPack_tile017.png");
    state.waterPlatform[11].position = glm::vec3(20, -7, 0);

    state.level1DoorBottom->position = glm::vec3(27.5f, -3.0f, 0);
    state.level1DoorTop->position = glm::vec3(27.5f, -2.0f, 0);

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

    state.enemies = new Entity[LEVEL3_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("ctg.png");

    state.enemies[0].entityType = ENEMY;
    state.enemies[0].textureID = enemyTextureID;
    state.enemies[0].position = glm::vec3(18, -6.0f, 0);
    state.enemies[0].speed = 1;
    state.enemies[0].aiType = STATIONARYSHOOTER;
    state.enemies[0].aiState = IDLE;
    state.enemies[0].isActive = false;

    state.enemies[1].entityType = ENEMY;
    state.enemies[1].textureID = enemyTextureID;
    state.enemies[1].position = glm::vec3(13, -5.0f, 0);
    state.enemies[1].speed = 1;
    state.enemies[1].aiType = WALKER;
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

void Level3::Update(float deltaTime) {
    state.player->Update(deltaTime, state.bargePlatforms, state.waterPlatform, state.player, state.enemies, LEVEL3_ENEMY_COUNT, state.bullets, state.enemyBullets, BULLET_COUNT, ENEMY_BULLET_COUNT, state.map);
    state.enemies[0].Update(deltaTime, state.bargePlatforms, state.waterPlatform, state.player, state.enemies, LEVEL3_ENEMY_COUNT, state.bullets, state.enemyBullets, BULLET_COUNT, ENEMY_BULLET_COUNT, state.map);
    state.enemies[1].Update(deltaTime, state.bargePlatforms, state.waterPlatform, state.player, state.enemies, LEVEL3_ENEMY_COUNT, state.bullets, state.enemyBullets, BULLET_COUNT, ENEMY_BULLET_COUNT, state.map);
    state.bullets->Update(deltaTime, state.bargePlatforms, state.waterPlatform, state.player, state.enemies, LEVEL3_ENEMY_COUNT, state.bullets, state.enemyBullets, BULLET_COUNT, ENEMY_BULLET_COUNT, state.map);
    state.enemyBullets->Update(deltaTime, state.bargePlatforms, state.waterPlatform, state.player, state.enemies, LEVEL3_ENEMY_COUNT, state.bullets, state.enemyBullets, BULLET_COUNT, ENEMY_BULLET_COUNT, state.map);
    state.bargePlatforms[0].Update(deltaTime, state.bargePlatforms, state.waterPlatform, state.player, state.enemies, LEVEL3_ENEMY_COUNT, state.bullets, state.enemyBullets, BULLET_COUNT, ENEMY_BULLET_COUNT, state.map);

    state.level1DoorBottom->Update(deltaTime, state.bargePlatforms, state.waterPlatform, state.player, state.enemies, LEVEL3_ENEMY_COUNT, state.bullets, state.enemyBullets, BULLET_COUNT, ENEMY_BULLET_COUNT, state.map);
    state.level1DoorTop->Update(deltaTime, state.bargePlatforms, state.waterPlatform, state.player, state.enemies, LEVEL3_ENEMY_COUNT, state.bullets, state.enemyBullets, BULLET_COUNT, ENEMY_BULLET_COUNT, state.map);

    for (int j = 0; j < WATER_PLATFORM_COUNT; j++) {
        state.waterPlatform[j].Update(deltaTime, state.bargePlatforms, state.waterPlatform, state.player, state.enemies, LEVEL3_ENEMY_COUNT, state.bullets, state.enemyBullets, BULLET_COUNT, ENEMY_BULLET_COUNT, state.map);
    }
    if ((state.player->position.x >= 27.0f) && (state.player->isActive == true)) {
        musicDoor3 = Mix_LoadWAV("success.wav");
        Mix_PlayChannel(-1, musicDoor3, 0);
        state.player->hasWon = "Yes";
        state.player->isActive = false;
    }

    if ((state.player->goBack == true) && (Global::lives > 0)) {
        state.player->position = glm::vec3(3, -1, 0);
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

void Level3::Render(ShaderProgram* program) {
    state.map->Render(program);
    state.player->Render(program);
    state.enemies[0].Render(program);
    state.enemies[1].Render(program);
    state.bullets->Render(program);
    state.enemyBullets->Render(program);
    state.bargePlatforms[0].Render(program);
    state.level1DoorBottom->Render(program);
    state.level1DoorTop->Render(program);

    for (int i = 0; i < WATER_PLATFORM_COUNT; i++) {
        state.waterPlatform[i].Render(program);
    }
    
    if (state.player->hasWon == "Yes") {
        Util::DrawText(program, state.fontTexture->textureID, "YOU WIN!", 0.8f, -0.25f, glm::vec3(21.5f, -2.0f, 0));
    }
    if (state.player->hasWon == "No") {
        Util::DrawText(program, state.fontTexture->textureID, "GAME OVER!", 0.8f, -0.25f, glm::vec3(state.player->position.x - 2.2f, -2.0f, 0));
    }
}