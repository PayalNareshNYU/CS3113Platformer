#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1

#include <SDL_mixer.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include <sstream> 

#include "Util.h"
#include "Entity.h"
#include "Map.h"
#include "Scene.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"

#define BULLET_COUNT 1

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

Scene* currentScene;
Scene* sceneList[3];

GLuint fontTextureID;

Mix_Music* musicGame;

bool showInstructionsPage1 = true;
bool showInstructionsPage2 = false;
bool doneWithInstructions = false;
bool gameHasStarted = false;

void SwitchToScene(Scene* scene) {
    currentScene = scene;
    currentScene->Initialize();
}

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    displayWindow = SDL_CreateWindow("Platformer!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif
    glViewport(0, 0, 640, 480);

    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    musicGame = Mix_LoadMUS("dooblydoo.mp3");
    Mix_PlayMusic(musicGame, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 4);

    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);

    glUseProgram(program.programID);

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    fontTextureID = Util::LoadTexture("font1.png");

    Global::lives = 3;

    sceneList[0] = new Level1();
    sceneList[1] = new Level2();
    sceneList[2] = new Level3();
    SwitchToScene(sceneList[0]);
}

void ProcessInput() {
    currentScene->state.player->movement = glm::vec3(0);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
        case SDL_WINDOWEVENT_CLOSE:
            gameIsRunning = false;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_SPACE:
                if (gameHasStarted == true) {
                    if (currentScene->state.player->collidedBottom) {
                        currentScene->state.player->jump = true;
                    }
                }
                break;

            case SDLK_RETURN:
                if (showInstructionsPage1 == true && showInstructionsPage2 == false && doneWithInstructions == false) {
                    showInstructionsPage1 = false;
                    showInstructionsPage2 = true;
                }
                else if (showInstructionsPage1 == false && showInstructionsPage2 == true && doneWithInstructions == false) {
                    showInstructionsPage2 = false;
                    doneWithInstructions = true;
                    gameHasStarted = true;
                }
                break;
            }
            break; // SDL_KEYDOWN
        }
    }

    const Uint8* keys = SDL_GetKeyboardState(NULL);

    if (keys[SDL_SCANCODE_LEFT]) {
        if (gameHasStarted == true) {
            currentScene->state.player->movement.x = -1.0f;
            currentScene->state.player->animIndices = currentScene->state.player->animLeft;
        }
    }
    else if (keys[SDL_SCANCODE_RIGHT]) {
        if (gameHasStarted == true) {
            currentScene->state.player->movement.x = 1.0f;
            currentScene->state.player->animIndices = currentScene->state.player->animRight;
        }
    }
    if (keys[SDL_SCANCODE_S]) {
        if (gameHasStarted == true) {
            currentScene->state.player->shootBullet(currentScene->state.player, currentScene->state.bullets, BULLET_COUNT);
        }
    }

    if (glm::length(currentScene->state.player->movement) > 1.0f) {
        currentScene->state.player->movement = glm::normalize(currentScene->state.player->movement);
    }
}

#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0f;

void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;

    deltaTime += accumulator;
    if (deltaTime < FIXED_TIMESTEP) {
        accumulator = deltaTime;
        return;
    }

    while (deltaTime >= FIXED_TIMESTEP) {
        // Update. Notice it's FIXED_TIMESTEP. Not deltaTime
        currentScene->Update(FIXED_TIMESTEP);
        deltaTime -= FIXED_TIMESTEP;
    }
    accumulator = deltaTime;

    viewMatrix = glm::mat4(1.0f);

    if (currentScene == sceneList[0]) {
        if ((currentScene->state.player->position.x > 5) && (currentScene->state.player->position.x < 17.5)) {
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-currentScene->state.player->position.x, 3.75, 0));

        }
        else if ((currentScene->state.player->position.x >= 17.5)) {
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-17.5, 3.75, 0));
        }
        else {
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-5, 3.75, 0));
        }
    }
    else if (currentScene == sceneList[1]) {
        if ((currentScene->state.player->position.x > 5) && (currentScene->state.player->position.x < 19.5)) {
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-currentScene->state.player->position.x, 3.75, 0));

        }
        else if ((currentScene->state.player->position.x >= 19.5)) {
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-19.5, 3.75, 0));
        }
        else {
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-5, 3.75, 0));
        }
    }
    else if (currentScene == sceneList[2]) {
        if ((currentScene->state.player->position.x > 5) && (currentScene->state.player->position.x < 23.5)) {
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-currentScene->state.player->position.x, 3.75, 0));

        }
        else if ((currentScene->state.player->position.x >= 23.5)) {
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-23.5, 3.75, 0));
        }

        else {
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-5, 3.75, 0));
        }
    }
}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);

    if (showInstructionsPage1 == true) {
        Util::DrawText(&program, fontTextureID, "Instructions: ", 0.8f, -0.25f, glm::vec3(-3.3f, 2.9f, 0));
        Util::DrawText(&program, fontTextureID, "Press the RIGHT ARROW key (->) to move right", 0.40f, -0.20f, glm::vec3(-4.3f, 1.8f, 0));
        Util::DrawText(&program, fontTextureID, "Press the LEFT ARROW key (<-) to move left", 0.40f, -0.20f, glm::vec3(-4.3f, 0.9f, 0));
        Util::DrawText(&program, fontTextureID, "Press the SPACEBAR to jump", 0.45f, -0.15f, glm::vec3(-4.3f, 0.1f, 0));
        Util::DrawText(&program, fontTextureID, "Press the 'S' key to shoot", 0.45f, -0.15f, glm::vec3(-4.3f, -0.7f, 0));
        Util::DrawText(&program, fontTextureID, "You can only shoot 1 bullet at a time!", 0.40f, -0.15f, glm::vec3(-4.6f, -1.5f, 0));
        Util::DrawText(&program, fontTextureID, "Press 'Enter' to go", 0.40f, -0.15f, glm::vec3(0.0f, -2.7f, 0));
        Util::DrawText(&program, fontTextureID, "to the next page...", 0.40f, -0.15f, glm::vec3(0.0f, -3.2f, 0));
    }

    if (showInstructionsPage2 == true) {
        Util::DrawText(&program, fontTextureID, "Instructions: ", 0.8f, -0.25f, glm::vec3(-3.3f, 2.9f, 0));
        Util::DrawText(&program, fontTextureID, "Your goal:", 0.47f, -0.15f, glm::vec3(-4.3f, 1.8f, 0));
        Util::DrawText(&program, fontTextureID, "Shoot all of the enemies", 0.47f, -0.15f, glm::vec3(-4.3f, 0.5f, 0));
        Util::DrawText(&program, fontTextureID, "...before they get to you", 0.47f, -0.15f, glm::vec3(-4.3f, -0.8f, 0));
        Util::DrawText(&program, fontTextureID, "Press 'Enter' to", 0.4f, -0.15f, glm::vec3(0.4f, -2.7f, 0));
        Util::DrawText(&program, fontTextureID, "begin the game...", 0.4f, -0.15f, glm::vec3(0.4f, -3.2f, 0));
    }

    if (doneWithInstructions == true) {
        std::stringstream s1;
        s1 << Global::lives;
        std::string s2;
        s1 >> s2;
        if (currentScene == sceneList[0]) {
            if ((currentScene->state.player->position.x > 5) && (currentScene->state.player->position.x < 17.5)) {
                Util::DrawText(&program, fontTextureID, "Lives Left: " + s2, 0.5f, -0.25f, glm::vec3(currentScene->state.player->position.x - 4.2f, -0.5f, 0));
            }
            else if (currentScene->state.player->position.x <= 5) {
                Util::DrawText(&program, fontTextureID, "Lives Left: " + s2, 0.5f, -0.25f, glm::vec3(0.8f, -0.5f, 0));
            }
            else if (currentScene->state.player->position.x >= 17.5) {
                Util::DrawText(&program, fontTextureID, "Lives Left: " + s2, 0.5f, -0.25f, glm::vec3(13.3f, -0.5f, 0));
            }
        }
        else if (currentScene == sceneList[1]) {
            if ((currentScene->state.player->position.x > 5) && (currentScene->state.player->position.x < 19.5)) {
                Util::DrawText(&program, fontTextureID, "Lives Left: " + s2, 0.5f, -0.25f, glm::vec3(currentScene->state.player->position.x - 4.2f, -0.5f, 0));
            }
            else if (currentScene->state.player->position.x <= 5) {
                Util::DrawText(&program, fontTextureID, "Lives Left: " + s2, 0.5f, -0.25f, glm::vec3(0.8f, -0.5f, 0));
            }
            else if (currentScene->state.player->position.x >= 19.5) {
                Util::DrawText(&program, fontTextureID, "Lives Left: " + s2, 0.5f, -0.25f, glm::vec3(15.3f, -0.5f, 0));
            }
        }
        else if (currentScene == sceneList[2]) {
            if ((currentScene->state.player->position.x > 5) && (currentScene->state.player->position.x < 23.5)) {
                Util::DrawText(&program, fontTextureID, "Lives Left: " + s2, 0.5f, -0.25f, glm::vec3(currentScene->state.player->position.x - 4.2f, -0.5f, 0));
            }
            else if (currentScene->state.player->position.x <= 5) {
                Util::DrawText(&program, fontTextureID, "Lives Left: " + s2, 0.5f, -0.25f, glm::vec3(0.8f, -0.5f, 0));
            }
            else if (currentScene->state.player->position.x >= 23.5) {
                Util::DrawText(&program, fontTextureID, "Lives Left: " + s2, 0.5f, -0.25f, glm::vec3(19.3f, -0.5f, 0));
            }
        }

        program.SetViewMatrix(viewMatrix);

        currentScene->Render(&program);
    }

    SDL_GL_SwapWindow(displayWindow);
}

void Shutdown() {
    Mix_FreeMusic(musicGame);
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();

    while (gameIsRunning) {
        ProcessInput();
        Update();

        if (currentScene->state.nextScene >= 0) SwitchToScene(sceneList[currentScene->state.nextScene]);

        Render();
    }

    Shutdown();
    return 0;
}