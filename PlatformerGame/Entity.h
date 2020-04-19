#pragma once
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
#include "Map.h"

enum EntityType { PLAYER, WATERPLATFORM, ENEMY, BULLET, ENEMYBULLET, BARGEPLATFORM};

enum AIType { WALKER, WALKER1, WAITANDGO, STATIONARYSHOOTER};
enum AIState { IDLE, IDLEWAIT, WALKING, MOVE_RIGHT, MOVE_LEFT, ATTACKING, SHOOTRIGHT, SHOOTLEFT, MOVING, BARGE_MOVE_RIGHT, BARGE_MOVE_LEFT};

class Entity {
public:
    EntityType entityType;
    AIType aiType;
    AIState aiState;

    glm::vec3 position;
    glm::vec3 movement;
    glm::vec3 acceleration;
    glm::vec3 velocity;

    float width = 0.8f;
    float height = 0.8f;

    bool jump = false;
    float jumpPower = 0;

    float speed;

    GLuint textureID;

    glm::mat4 modelMatrix;

    int* animRight = NULL;
    int* animLeft = NULL;
    int* animUp = NULL;
    int* animDown = NULL;

    int* animIndices = NULL;
    int animFrames = 0;
    int animIndex = 0;
    float animTime = 0;
    int animCols = 0;
    int animRows = 0;

    bool isActive = true;
    bool shootNow = true;
    bool gunReady = true;
    bool changeDirection = false;
    bool livesFreeze = false;
    std::string hasWon;

    bool collidedTop = false;
    bool collidedBottom = false;
    bool collidedLeft = false;
    bool collidedRight = false;

    bool goBack = false;

    Entity();

    bool CheckCollision(Entity* other);
    bool CheckCollisionWater(Entity* other);
    bool CheckCollisionsYBarge(Entity* objects, int objectCount);
    bool CheckCollisionsXBarge(Entity* objects, int objectCount);
    bool CheckCollisionsX(Map* map);
    bool CheckCollisionsY(Map* map);
    bool CheckCollisionsXEnemy(Entity* enemies, int enemyCount, Entity* player);
    bool CheckCollisionsYEnemy(Entity* enemies, int enemyCount, Entity* player);

    void Update(float deltaTime, Entity* bargePlatforms, Entity* waterPlatform, Entity* player, Entity* enemies, int enemyCount, Entity* bullets, Entity* enemyBullets, int bulletCount, int enemyBulletCount, Map* map);
    void Render(ShaderProgram* program);
    void DrawSpriteFromTextureAtlas(ShaderProgram* program, GLuint textureID, int index);

    void AI(Entity* player, Entity* enemyBullets, int enemyBulletCount, Map* map);
    void AIWalker();
    void AIWalker1();
    void AIWaitAndGo(Entity* player);
    void shootBullet(Entity* player, Entity* bullets, int bulletCount);
    void AIShooter(Entity* player, Entity* enemyBullets, int enemyBulletCount, Map* map);
    void Barge(Entity* player, Entity* barge);
};