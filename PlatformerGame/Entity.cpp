#include "Entity.h"
#include "Util.h"
#include <SDL_mixer.h>

Mix_Chunk* musicShoot;
Mix_Chunk* musicFail;

Entity::Entity()
{
    position = glm::vec3(0);
    movement = glm::vec3(0);
    acceleration = glm::vec3(0);
    velocity = glm::vec3(0);
    speed = 0;

    modelMatrix = glm::mat4(1.0f);
}

bool Entity::CheckCollision(Entity* other) {
    if (isActive == false || other->isActive == false) return false;

    float xdist = fabs(position.x - other->position.x) - ((width + other->width) / 2.0f);
    float ydist = fabs(position.y - other->position.y) - ((height + other->height) / 2.0f);

    if (xdist < 0 && ydist < 0) return true;

    return false;
}

bool Entity::CheckCollisionsYBarge(Entity* objects, int objectCount)
{
    for (int i = 0; i < objectCount; i++)
    {
        Entity* object = &objects[i];
        if (CheckCollision(object))
        {
            float ydist = fabs(position.y - object->position.y);
            float penetrationY = fabs(ydist - (height / 2.0f) - (object->height / 2.0f));
            if (velocity.y > 0) {
                position.y -= penetrationY;
                velocity.y = 0;
                collidedTop = true;
            }
            else if (velocity.y < 0) {
                position.y += penetrationY;
                velocity.y = 0;
                collidedBottom = true;
            }
            if (collidedTop || collidedBottom) {
                position.x = object->position.x;
                return true;
            }
        }
    }
    return false;
}

bool Entity::CheckCollisionsXBarge(Entity* objects, int objectCount)
{
    for (int i = 0; i < objectCount; i++)
    {
        Entity* object = &objects[i];
        if (CheckCollision(object))
        {
            float xdist = fabs(position.x - object->position.x);
            float penetrationX = fabs(xdist - (width / 2.0f) - (object->width / 2.0f));
            if (velocity.x > 0) {
                position.x -= penetrationX;
                velocity.x = 0;
                collidedRight = true;
            }
            else if (velocity.x < 0) {
                position.x += penetrationX;
                velocity.x = 0;
                collidedLeft = true;
            }
            if (collidedRight || collidedLeft) {
                position.x = object->position.x;
                return true;
            }
        }
    }
    return false;
}

bool Entity::CheckCollisionWater(Entity* other) {
    if (isActive == false || other->isActive == false) return false;

    float xdist = fabs(position.x - other->position.x) - ((width - other->width) / 2.0f);
    float ydist = fabs(position.y - other->position.y) - ((height - other->height) / 2.0f);

    if (xdist < 0 && ydist < 0) return true;

    return false;
}

bool Entity::CheckCollisionsY(Map* map)
{
    // Probes for tiles
    glm::vec3 top = glm::vec3(position.x, position.y + (height / 2), position.z);
    glm::vec3 top_left = glm::vec3(position.x - (width / 2), position.y + (height / 2), position.z);
    glm::vec3 top_right = glm::vec3(position.x + (width / 2), position.y + (height / 2), position.z);

    glm::vec3 bottom = glm::vec3(position.x, position.y - (height / 2), position.z);
    glm::vec3 bottom_left = glm::vec3(position.x - (width / 2), position.y - (height / 2), position.z);
    glm::vec3 bottom_right = glm::vec3(position.x + (width / 2), position.y - (height / 2), position.z);

    float penetration_x = 0;
    float penetration_y = 0;
    if (map->IsSolid(top, &penetration_x, &penetration_y) && velocity.y > 0) {
        position.y -= penetration_y;
        velocity.y = 0;
        collidedTop = true;
    }
    else if (map->IsSolid(top_left, &penetration_x, &penetration_y) && velocity.y > 0) {
        position.y -= penetration_y;
        velocity.y = 0;
        collidedTop = true;
    }
    else if (map->IsSolid(top_right, &penetration_x, &penetration_y) && velocity.y > 0) {
        position.y -= penetration_y;
        velocity.y = 0;
        collidedTop = true;
    }

    if (map->IsSolid(bottom, &penetration_x, &penetration_y) && velocity.y < 0) {
        position.y += penetration_y;
        velocity.y = 0;
        collidedBottom = true;
    }
    else if (map->IsSolid(bottom_left, &penetration_x, &penetration_y) && velocity.y < 0) {
        position.y += penetration_y;
        velocity.y = 0;
        collidedBottom = true;
    }
    else if (map->IsSolid(bottom_right, &penetration_x, &penetration_y) && velocity.y < 0) {
        position.y += penetration_y;
        velocity.y = 0;
        collidedBottom = true;
    }

    if (collidedTop || collidedBottom) {
        return true;
    }
    else {
        return false;
    }
}

bool Entity::CheckCollisionsX(Map* map)
{
    // Probes for tiles
    glm::vec3 left = glm::vec3(position.x - (width / 2), position.y, position.z);
    glm::vec3 right = glm::vec3(position.x + (width / 2), position.y, position.z);

    float penetration_x = 0;
    float penetration_y = 0;
    if (map->IsSolid(left, &penetration_x, &penetration_y) && velocity.x < 0) {
        position.x += penetration_x;
        velocity.x = 0;
        collidedLeft = true;
    }

    if (map->IsSolid(right, &penetration_x, &penetration_y) && velocity.x > 0) {
        position.x -= penetration_x;
        velocity.x = 0;
        collidedRight = true;
    }

    if (collidedLeft || collidedRight) {
        return true;
    }
    else {
        return false;
    }
}

bool Entity::CheckCollisionsXEnemy(Entity* enemies, int enemyCount, Entity* player)
{
    for (int i = 0; i < enemyCount; i++)
    {
        Entity* object = &enemies[i];
        if (player->CheckCollision(object))
        {
            float xdist = fabs(position.x - object->position.x);
            float penetrationX = fabs(xdist - (width / 2.0f) - (object->width / 2.0f));
            if (velocity.x > 0) {
                position.x -= penetrationX;
                velocity.x = 0;
                collidedRight = true;
            }
            else if (velocity.x < 0) {
                position.x += penetrationX;
                velocity.x = 0;
                collidedLeft = true;
            }

            if (collidedRight || collidedLeft) {
                return true;
            }
        }
    }
    return false;
}

bool Entity::CheckCollisionsYEnemy(Entity* enemies, int enemyCount, Entity* player)
{
    for (int i = 0; i < enemyCount; i++)
    {
        Entity* object = &enemies[i];
        if (player->CheckCollision(object))
        {
            float ydist = fabs(position.y - object->position.y);
            float penetrationY = fabs(ydist - (height / 2.0f) - (object->height / 2.0f));
            if (velocity.y > 0) {
                position.y -= penetrationY;
                velocity.y = 0;
                collidedTop = true;
            }
            else if (velocity.y < 0) {
                position.y += penetrationY;
                velocity.y = 0;
                collidedBottom = true;
            }

            if (collidedTop || collidedBottom) {
                return true;
            }
        }
    }
    return false;
}

void Entity::AIWalker() {
    switch (aiState) {
    case WALKING:
        speed = 1.0f;
        movement = glm::vec3(1, 0, 0);
        if (position.x <= 11.0f) {
            aiState = MOVE_RIGHT;
        }
        else if (position.x >= 15.0f) {
            aiState = MOVE_LEFT;
        }
        break;
    case MOVE_RIGHT:
        movement = glm::vec3(1, 0, 0);
        if (position.x >= 15.0f) {
            aiState = MOVE_LEFT;
        }
        break;
    case MOVE_LEFT:
        movement = glm::vec3(-1, 0, 0);
        if (position.x <= 11.0f) {
            aiState = MOVE_RIGHT;
        }
        break;
    }
}

void Entity::AIWalker1() {
    switch (aiState) {
    case WALKING:
        speed = 1.0f;
        movement = glm::vec3(1, 0, 0);
        if (position.x <= 3.5f) {
            aiState = MOVE_RIGHT;
        }
        else if (position.x >= 8.0f) {
            aiState = MOVE_LEFT;
        }
        break;
    case MOVE_RIGHT:
        movement = glm::vec3(1, 0, 0);
        if (position.x >= 8.0f) {
            aiState = MOVE_LEFT;
        }
        break;
    case MOVE_LEFT:
        movement = glm::vec3(-1, 0, 0);
        if (position.x <= 3.5f) {
            aiState = MOVE_RIGHT;
        }
        break;
    }
}

void Entity::AIWaitAndGo(Entity* player) {
    switch (aiState) {
    case IDLEWAIT:
        if (glm::distance(position.x, player->position.x) < 4.5f && glm::distance(position.y, player->position.y) < 0.5f) {
            aiState = WALKING;
        }
        break;
    case WALKING:
        if (player->position.x < position.x) {
            movement = glm::vec3(-1, 0, 0);
        }
        else {
            movement = glm::vec3(1, 0, 0);
        }
        break;
    case ATTACKING:
        break;
    }
}

void Entity::AIShooter(Entity* player, Entity* enemyBullets, int enemyBulletCount, Map* map) {
    switch (aiState) {
    
    case SHOOTRIGHT:
        if (gunReady == true) {
            gunReady = false;
            enemyBullets->isActive = true;
            enemyBullets->movement = glm::vec3(1, 0, 0);
            enemyBullets->speed = 0.5f;

            if (enemyBullets->CheckCollisionsX(map) || enemyBullets->CheckCollisionsY(map)) {
                aiState = SHOOTLEFT;
            }
        }
        
        break;
    
    case SHOOTLEFT:
        if (gunReady == true) {
            gunReady = false;
            enemyBullets->isActive = true;
            enemyBullets->movement = glm::vec3(-1, 0, 0);
            enemyBullets->speed = 0.5f;

            if (enemyBullets->CheckCollisionsX(map) || enemyBullets->CheckCollisionsY(map)) {
                aiState = SHOOTRIGHT;
            }  
        }
        
        break;
    case IDLE:
        aiState = SHOOTLEFT;
        break;
    }
}

void Entity::AI(Entity* player, Entity* enemyBullets, int enemyBulletCount, Map* map) {
    switch (aiType) {
    case WALKER:
        AIWalker();
        break;

    case WALKER1:
        AIWalker1();
        break;

    case WAITANDGO:
        AIWaitAndGo(player);
        break;
    
    case STATIONARYSHOOTER:
        AIShooter(player, enemyBullets, enemyBulletCount, map);
        break;
    }
}

void Entity::shootBullet(Entity* player, Entity* bullets, int bulletCount) {
    if (isActive == false) return;

    if (player->gunReady == true) {
        musicShoot = Mix_LoadWAV("bounce.wav");
        Mix_PlayChannel(-1, musicShoot, 0);

        player->gunReady = false;
        bullets->isActive = true;
        bullets->position = position;
        if (animIndices == animLeft) {
            bullets->movement.x = -0.5f;
        }
        else {
            bullets->movement.x = 0.5f;
        }
        bullets->speed = 2.5f;
    }
}

void Entity::Barge(Entity* player, Entity* barge) {
    switch (aiState) {
    case MOVING:
        speed = 1.0f;
        movement = glm::vec3(1, 0, 0);
        if (position.x <= 17.0f) {
            aiState = BARGE_MOVE_RIGHT;
        }
        else if (position.x >= 20.0f) {
            aiState = BARGE_MOVE_LEFT;
        }
        break;
    
    case BARGE_MOVE_RIGHT:
        movement = glm::vec3(1, 0, 0);
        if (position.x >= 20.0f) {
            aiState = BARGE_MOVE_LEFT;
        }
        break;
    
    case BARGE_MOVE_LEFT:
        movement = glm::vec3(-1, 0, 0);
        if (position.x <= 17.0f) {
            aiState = BARGE_MOVE_RIGHT;
        }
        break;
    }
}

void Entity::Update(float deltaTime, Entity* bargePlatforms, Entity* waterPlatform, Entity* player, Entity* enemies, int enemyCount, Entity* bullets, Entity* enemyBullets, int bulletCount, int enemyBulletCount, Map *map)
{
    if (isActive == false) return;

    collidedTop = false;
    collidedBottom = false;
    collidedLeft = false;
    collidedRight = false;

    if (entityType == ENEMY) {
        AI(player, enemyBullets, enemyBulletCount, map);
    }

    if (entityType == BARGEPLATFORM) {
        Barge(player, bargePlatforms);
        player->CheckCollisionsYBarge(bargePlatforms, 1);
        player->CheckCollisionsXBarge(bargePlatforms, 1);
    }

    if (entityType == WATERPLATFORM) {
        for (int i = 0; i < 12; i++)
        {
            Entity* object = &waterPlatform[i];
            if (player->CheckCollision(object)) {
                musicFail = Mix_LoadWAV("failure.wav");
                Mix_PlayChannel(-1, musicFail, 0);
                player->isActive = false;
                player->goBack = true;
                if (player->livesFreeze == false) {
                    Global::lives -= 1;
                }    
            }
        }
    }

    if (entityType == PLAYER) {
        player->CheckCollisionsXEnemy(enemies, enemyCount, player);
        player->CheckCollisionsYEnemy(enemies, enemyCount, player);  
        if ((player->CheckCollisionsXEnemy(enemies, enemyCount, player)) || (player->CheckCollisionsYEnemy(enemies, enemyCount, player))) {
            musicFail = Mix_LoadWAV("failure.wav");
            Mix_PlayChannel(-1, musicFail, 0);

            player->isActive = false;
            player->goBack = true;   
            if (player->livesFreeze == false) {
                Global::lives -= 1;
            }
        }
    }

    if (entityType == BULLET) {
        velocity.x = movement.x * speed;
        velocity += acceleration * deltaTime;
        position.x += velocity.x * deltaTime;

        for (int i = 0; i < enemyCount; i++) {
            if (CheckCollision(&enemies[i])) {
                player->gunReady = true;
                enemies[i].isActive = false;
                isActive = false;
                if (i == 0) {
                    enemyBullets[i].isActive = false;
                }             
            }
        }
        if (CheckCollisionsY(map) || CheckCollisionsX(map)) {
            player->gunReady = true;
            isActive = false;
        }

        if (player->position.x > 5.0f) {
            if (position.x < player->position.x -5.0f || position.x > player->position.x+5.0f) {
                player->gunReady = true;
            }
        }
        if (player->position.x < 5.0f) {
            if (position.x < (-player->position.x) || position.x >(15.0 - player->position.x)) {
                player->gunReady = true;
            }
        }
    }

    if (entityType == ENEMYBULLET) {
        velocity.x = movement.x * speed;
        velocity += acceleration * deltaTime;
        position.x += velocity.x * deltaTime;

        if (CheckCollisionsY(map) || CheckCollisionsX(map)) {
            for (int i = 0; i < enemyCount; i++) {
                enemies[i].gunReady = true;
            }
            isActive = false;
            position = enemies[0].position;
        }
        if (glm::distance(enemyBullets->position, player->position) < 0.5f) {
            musicFail = Mix_LoadWAV("failure.wav");
            Mix_PlayChannel(-1, musicFail, 0);
            player->isActive = false;
            player->goBack = true;
            if (player->livesFreeze == false) {
                Global::lives -= 1;
            }
        }
    }

    if (animIndices != NULL) {
        if (glm::length(movement) != 0) {
            animTime += deltaTime;

            if (animTime >= 0.25f)
            {
                animTime = 0.0f;
                animIndex++;
                if (animIndex >= animFrames)
                {
                    animIndex = 0;
                }
            }
        }
        else {
            animIndex = 0;
        }
    }

    if (jump) {
        jump = false;
        velocity.y += jumpPower;
    }

    velocity.x = movement.x * speed;
    velocity += acceleration * deltaTime;

    position.y += velocity.y * deltaTime; // Move on Y
    player->CheckCollisionsY(map);

    position.x += velocity.x * deltaTime; // Move on X
    player->CheckCollisionsX(map);

    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
}

void Entity::DrawSpriteFromTextureAtlas(ShaderProgram* program, GLuint textureID, int index)
{
    float u = (float)(index % animCols) / (float)animCols;
    float v = (float)(index / animCols) / (float)animRows;

    float width = 1.0f / (float)animCols;
    float height = 1.0f / (float)animRows;

    float texCoords[] = { u, v + height, u + width, v + height, u + width, v,
        u, v + height, u + width, v, u, v };

    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };

    glBindTexture(GL_TEXTURE_2D, textureID);

    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);

    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

void Entity::Render(ShaderProgram* program) {

    if (isActive == false) return;

    program->SetModelMatrix(modelMatrix);

    if (animIndices != NULL) {
        DrawSpriteFromTextureAtlas(program, textureID, animIndices[animIndex]);
        return;
    }

    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };

    glBindTexture(GL_TEXTURE_2D, textureID);

    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);

    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}