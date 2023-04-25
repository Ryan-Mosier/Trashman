//
// Created by ryanm on 4/17/2023.
//

#include "GameHandler.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "../Game/object.h"
#include "../Game/debugValues.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <queue>
#include <iostream>

#include <thread>
#include <chrono>
#include <vector>


// GameHandler-related State data
SpriteRenderer *Renderer;


GameHandler::GameHandler(unsigned int width, unsigned int height, bool de)
        : State(playing), Keys(), Width(width), Height(height), logic(&State) {
    timeSinceLastTick = 0;
    ::debug = de;
    if (::debug) {
        ::debugPath = vector<vector<Position>>(3);
        ::entityPos = vector<Position>(4);
    } else {
        ::debugPath = vector<vector<Position>>(0);
        ::entityPos = vector<Position>(0);
    }
}

GameHandler::~GameHandler() {
    delete Renderer;
}

void GameHandler::Init() {
    // load shaders
    ResourceManager::LoadShader("shaders/shader.vert", "shaders/shader.frag", nullptr, "sprite");
    // configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),
                                      static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    // set render-specific controls
    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));

    // load textures
    /*#region map*/
    ResourceManager::LoadTexture("textures/map/straight.png", true, "map_straight");
    ResourceManager::LoadTexture("textures/map/corner.png", true, "map_corner");
    ResourceManager::LoadTexture("textures/map/t.png", true, "map_t");
    ResourceManager::LoadTexture("textures/map/end.png", true, "map_end");
    /*#endregion*/

    /*#region nigel */
    /*#region normal */
    ResourceManager::LoadTexture("textures/nigel/normal/down/1.png", true, "player_norm_down1");
    ResourceManager::LoadTexture("textures/nigel/normal/down/2.png", true, "player_norm_down2");
    ResourceManager::LoadTexture("textures/nigel/normal/down/3.png", true, "player_norm_down3");
    ResourceManager::LoadTexture("textures/nigel/normal/down/4.png", true, "player_norm_down4");

    ResourceManager::LoadTexture("textures/nigel/normal/left/1.png", true, "player_norm_left1");
    ResourceManager::LoadTexture("textures/nigel/normal/left/2.png", true, "player_norm_left2");
    ResourceManager::LoadTexture("textures/nigel/normal/left/3.png", true, "player_norm_left3");
    ResourceManager::LoadTexture("textures/nigel/normal/left/4.png", true, "player_norm_left4");

    ResourceManager::LoadTexture("textures/nigel/normal/up/1.png", true, "player_norm_up1");
    ResourceManager::LoadTexture("textures/nigel/normal/up/2.png", true, "player_norm_up2");
    ResourceManager::LoadTexture("textures/nigel/normal/up/3.png", true, "player_norm_up3");
    ResourceManager::LoadTexture("textures/nigel/normal/up/4.png", true, "player_norm_up4");

    ResourceManager::LoadTexture("textures/nigel/normal/right/1.png", true, "player_norm_right1");
    ResourceManager::LoadTexture("textures/nigel/normal/right/2.png", true, "player_norm_right2");
    ResourceManager::LoadTexture("textures/nigel/normal/right/3.png", true, "player_norm_right3");
    ResourceManager::LoadTexture("textures/nigel/normal/right/4.png", true, "player_norm_right4");
    /*#endregion */
    /*#region pp */
    ResourceManager::LoadTexture("textures/nigel/power/down/1.png", true, "player_pp_down1");
    ResourceManager::LoadTexture("textures/nigel/power/down/2.png", true, "player_pp_down2");
    ResourceManager::LoadTexture("textures/nigel/power/down/3.png", true, "player_pp_down3");
    ResourceManager::LoadTexture("textures/nigel/power/down/4.png", true, "player_pp_down4");

    ResourceManager::LoadTexture("textures/nigel/power/left/1.png", true, "player_pp_left1");
    ResourceManager::LoadTexture("textures/nigel/power/left/2.png", true, "player_pp_left2");
    ResourceManager::LoadTexture("textures/nigel/power/left/3.png", true, "player_pp_left3");
    ResourceManager::LoadTexture("textures/nigel/power/left/4.png", true, "player_pp_left4");

    ResourceManager::LoadTexture("textures/nigel/power/up/1.png", true, "player_pp_up1");
    ResourceManager::LoadTexture("textures/nigel/power/up/2.png", true, "player_pp_up2");
    ResourceManager::LoadTexture("textures/nigel/power/up/3.png", true, "player_pp_up3");
    ResourceManager::LoadTexture("textures/nigel/power/up/4.png", true, "player_pp_up4");

    ResourceManager::LoadTexture("textures/nigel/power/right/1.png", true, "player_pp_right1");
    ResourceManager::LoadTexture("textures/nigel/power/right/2.png", true, "player_pp_right2");
    ResourceManager::LoadTexture("textures/nigel/power/right/3.png", true, "player_pp_right3");
    ResourceManager::LoadTexture("textures/nigel/power/right/4.png", true, "player_pp_right4");
    /*#endregion */
    /*#endregion*/

    /*#region ghosts */
    /*#region red */
    ResourceManager::LoadTexture("textures/ghosts/red/down.png", true, "ghost_red_down");
    ResourceManager::LoadTexture("textures/ghosts/red/up.png", true, "ghost_red_up");
    ResourceManager::LoadTexture("textures/ghosts/red/left.png", true, "ghost_red_left");
    ResourceManager::LoadTexture("textures/ghosts/red/right.png", true, "ghost_red_right");
    /*#endregion */
    /*#region  pink */
    ResourceManager::LoadTexture("textures/ghosts/pink/down.png", true, "ghost_pink_down");
    ResourceManager::LoadTexture("textures/ghosts/pink/up.png", true, "ghost_pink_up");
    ResourceManager::LoadTexture("textures/ghosts/pink/left.png", true, "ghost_pink_left");
    ResourceManager::LoadTexture("textures/ghosts/pink/right.png", true, "ghost_pink_right");
    /*#endregion */
    /*#region yellow */
    ResourceManager::LoadTexture("textures/ghosts/yellow/down.png", true, "ghost_yellow_down");
    ResourceManager::LoadTexture("textures/ghosts/yellow/up.png", true, "ghost_yellow_up");
    ResourceManager::LoadTexture("textures/ghosts/yellow/left.png", true, "ghost_yellow_left");
    ResourceManager::LoadTexture("textures/ghosts/yellow/right.png", true, "ghost_yellow_right");
    /*#endregion */
    /*#endregion */

    /*#region objects*/
    ResourceManager::LoadTexture("textures/pp.png", true, "pp");
    ResourceManager::LoadTexture("textures/pellet.png", true, "pellet");
    /*#endregion*/

    /*#region misc*/
    //TODO:update textures
    ResourceManager::LoadTexture("textures/pp.png", true, "waiting");
    ResourceManager::LoadTexture("textures/pellet.png", true, "dead");
    /*#endregion*/
}

void GameHandler::Update(float dt) {
    if (State == GameState::playing) {
        Render();
        if (this->timeSinceLastTick + dt > 0.5) {
            logic.tick();
            timeSinceLastTick = 0;
        }
        this->timeSinceLastTick += dt;
    } else if (State == GameState::waiting) {
        waiting();
    } else if (State == GameState::dead) {
        dead();
    }
}

void GameHandler::ProcessInput(float dt) {
    std::vector<char> pressed;
    if (this->Keys[GLFW_KEY_W].first == true) {
        pressed.emplace_back('w');
    }
    if (this->Keys[GLFW_KEY_A].first == true) {
        pressed.emplace_back('a');
    }
    if (this->Keys[GLFW_KEY_S].first == true) {
        pressed.emplace_back('s');
    }
    if (this->Keys[GLFW_KEY_D].first == true) {
        pressed.emplace_back('d');
    }
    if (!pressed.empty()) {
        char update = pressed[0];
        this->logic.recentInput = update;
        cout << update << "\n";
    }
}

void GameHandler::waiting() {
    Renderer->DrawSprite(ResourceManager::GetTexture("waiting"),
                         glm::vec2(0.0f, 0.0f),
                         glm::vec2(1366, 768),
                         0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
}

void GameHandler::dead() {
    State = GameState::waiting;
    Renderer->DrawSprite(ResourceManager::GetTexture("dead"),
                         glm::vec2(0.0f, 0.0f),
                         glm::vec2(1366, 768),
                         0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
    std::this_thread::sleep_for(std::chrono::seconds(5));
}

void GameHandler::drawMap() {
    // 0 is empty
    // 1 is straight horz
    // 2 is straight vert
    // 3 is corner left/down
    // 4 is corner left/up
    // 5 is corner right/up
    // 6 is corner right/down
    // 7 is t down
    // 8 is t left
    // 9 is t up
    //10 is t right
    //11 is end down
    //12 is end left
    //13 is end up
    //14 is end right

    std::vector<std::vector<int>> arr =
            {{6,  1, 1, 1, 7,  1, 1,  1,  1,  1,  1, 1,  1, 1, 1,  1, 1, 1,  1, 1, 1,  1, 1,  1,  1, 1,  1,  7,  1, 1,  1, 3},
             {2,  0, 0, 6, 4,  0, 0,  0,  0,  0,  0, 0,  0, 0, 0,  0, 0, 0,  0, 0, 0,  0, 0,  0,  0, 0,  0,  5,  3, 0,  0, 2},
             {2,  0, 6, 4, 0,  0, 13, 0,  0,  12, 1, 14, 0, 0, 12, 1, 1, 14, 0, 0, 12, 1, 14, 0,  0, 13, 0,  0,  5, 3,  0, 2},
             {5,  1, 4, 0, 0,  6, 9,  3,  0,  0,  0, 0,  0, 0, 0,  0, 0, 0,  0, 0, 0,  0, 0,  0,  6, 9,  3,  0,  0, 5,  1, 4},
             {0,  0, 0, 0, 12, 8, 0,  10, 14, 0,  0, 13, 0, 0, 6,  1, 1, 3,  0, 0, 13, 0, 0,  12, 8, 0,  10, 14, 0, 0,  0, 0},
             {6,  1, 3, 0, 0,  5, 7,  4,  0,  0,  0, 2,  0, 0, 2,  0, 0, 2,  0, 0, 2,  0, 0,  0,  5, 7,  4,  0,  0, 6,  1, 3},
             {2,  0, 2, 0, 0,  0, 11, 0,  0,  13, 0, 11, 0, 0, 5,  1, 1, 4,  0, 0, 11, 0, 13, 0,  0, 11, 0,  0,  0, 2,  0, 2},
             {2,  0, 2, 0, 13, 0, 0,  0,  6,  8,  0, 0,  0, 0, 0,  0, 0, 0,  0, 0, 0,  0, 10, 3,  0, 0,  0,  13, 0, 10, 1, 8},
             {10, 1, 8, 0, 10, 3, 0,  6,  4,  2,  0, 6,  1, 1, 1,  1, 1, 1,  1, 1, 3,  0, 2,  5,  3, 0,  6,  8,  0, 2,  0, 2},
             {2,  0, 2, 0, 10, 4, 0,  5,  3,  2,  0, 5,  1, 1, 1,  1, 1, 1,  1, 1, 4,  0, 2,  6,  4, 0,  5,  8,  0, 2,  0, 2},
             {10, 1, 8, 0, 11, 0, 0,  0,  5,  8,  0, 0,  0, 0, 0,  0, 0, 0,  0, 0, 0,  0, 10, 4,  0, 0,  0,  11, 0, 10, 1, 8},
             {2,  0, 2, 0, 0,  0, 13, 0,  0,  11, 0, 13, 0, 0, 6,  1, 1, 3,  0, 0, 13, 0, 11, 0,  0, 13, 0,  0,  0, 2,  0, 2},
             {5,  1, 4, 0, 0,  6, 9,  3,  0,  0,  0, 2,  0, 0, 2,  0, 0, 2,  0, 0, 2,  0, 0,  0,  6, 9,  3,  0,  0, 5,  1, 4},
             {0,  0, 0, 0, 12, 8, 0,  10, 14, 0,  0, 11, 0, 0, 5,  1, 1, 4,  0, 0, 11, 0, 0,  12, 8, 0,  10, 14, 0, 0,  0, 0},
             {6,  1, 3, 0, 0,  5, 7,  4,  0,  0,  0, 0,  0, 0, 0,  0, 0, 0,  0, 0, 0,  0, 0,  0,  5, 7,  4,  0,  0, 6,  1, 3},
             {2,  0, 5, 3, 0,  0, 11, 0,  0,  12, 1, 14, 0, 0, 12, 1, 1, 14, 0, 0, 12, 1, 14, 0,  0, 11, 0,  0,  6, 4,  0, 2},
             {2,  0, 0, 5, 3,  0, 0,  0,  0,  0,  0, 0,  0, 0, 0,  0, 0, 0,  0, 0, 0,  0, 0,  0,  0, 0,  0,  6,  4, 0,  0, 2},
             {5,  1, 1, 1, 9,  1, 1,  1,  1,  1,  1, 1,  1, 1, 1,  1, 1, 1,  1, 1, 1,  1, 1,  1,  1, 1,  1,  9,  1, 1,  1, 4},
            };
    for (int i = 0; i < 18; i++) {
        for (int j = 0; j < 32; j++) {
            switch (arr[i][j]) {
                case 1:
                    Renderer->DrawSprite(ResourceManager::GetTexture("map_straight"),
                                         glm::vec2(j * 40.0f, i * 40.0f),
                                         glm::vec2(40.0f, 40.0f),
                                         0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
                    break;
                case 2:
                    Renderer->DrawSprite(ResourceManager::GetTexture("map_straight"),
                                         glm::vec2(j * 40.0f, i * 40.0f),
                                         glm::vec2(40.0f, 40.0f),
                                         90.0f, glm::vec3(1.0f, 1.0f, 1.0f));
                    break;
                case 3:
                    Renderer->DrawSprite(ResourceManager::GetTexture("map_corner"),
                                         glm::vec2(j * 40.0f, i * 40.0f),
                                         glm::vec2(40.1f, 40.1f),
                                         0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
                    break;
                case 4:
                    Renderer->DrawSprite(ResourceManager::GetTexture("map_corner"),
                                         glm::vec2(j * 40.0f, i * 40.0f),
                                         glm::vec2(40.1f, 40.1f),
                                         90.0f, glm::vec3(1.0f, 1.0f, 1.0f));
                    break;
                case 5:
                    Renderer->DrawSprite(ResourceManager::GetTexture("map_corner"),
                                         glm::vec2(j * 40.0f, i * 40.0f),
                                         glm::vec2(40.1f, 40.1f),
                                         180.0f, glm::vec3(1.0f, 1.0f, 1.0f));
                    break;
                case 6:
                    Renderer->DrawSprite(ResourceManager::GetTexture("map_corner"),
                                         glm::vec2(j * 40.0f, i * 40.0f),
                                         glm::vec2(40.1f, 40.1f),
                                         270.0f, glm::vec3(1.0f, 1.0f, 1.0f));
                    break;
                case 7:
                    Renderer->DrawSprite(ResourceManager::GetTexture("map_t"),
                                         glm::vec2(j * 40.0f, i * 40.0f),
                                         glm::vec2(40.0f, 40.0f),
                                         0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
                    break;
                case 8:
                    Renderer->DrawSprite(ResourceManager::GetTexture("map_t"),
                                         glm::vec2(j * 40.0f, i * 40.0f),
                                         glm::vec2(40.0f, 40.0f),
                                         90.0f, glm::vec3(1.0f, 1.0f, 1.0f));
                    break;
                case 9:
                    Renderer->DrawSprite(ResourceManager::GetTexture("map_t"),
                                         glm::vec2(j * 40.0f, i * 40.0f),
                                         glm::vec2(40.0f, 40.0f),
                                         180.0f, glm::vec3(1.0f, 1.0f, 1.0f));
                    break;
                case 10:
                    Renderer->DrawSprite(ResourceManager::GetTexture("map_t"),
                                         glm::vec2(j * 40.0f, i * 40.0f),
                                         glm::vec2(40.0f, 40.0f),
                                         270.0f, glm::vec3(1.0f, 1.0f, 1.0f));
                    break;
                case 11:
                    Renderer->DrawSprite(ResourceManager::GetTexture("map_end"),
                                         glm::vec2(j * 40.0f, i * 40.0f),
                                         glm::vec2(40.0f, 40.0f),
                                         0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
                    break;
                case 12:
                    Renderer->DrawSprite(ResourceManager::GetTexture("map_end"),
                                         glm::vec2(j * 40.0f, i * 40.0f),
                                         glm::vec2(40.0f, 40.0f),
                                         90.0f, glm::vec3(1.0f, 1.0f, 1.0f));
                    break;
                case 13:
                    Renderer->DrawSprite(ResourceManager::GetTexture("map_end"),
                                         glm::vec2(j * 40.0f, i * 40.0f),
                                         glm::vec2(40.0f, 40.0f),
                                         180.0f, glm::vec3(1.0f, 1.0f, 1.0f));
                    break;
                case 14:
                    Renderer->DrawSprite(ResourceManager::GetTexture("map_end"),
                                         glm::vec2(j * 40.0f, i * 40.0f),
                                         glm::vec2(40.0f, 40.0f),
                                         270.0f, glm::vec3(1.0f, 1.0f, 1.0f));
                    break;
            }
        }
    }
}

void GameHandler::Render() {
    drawMap();

    std::queue<std::pair<string, std::pair<int, int>>> entityQueue;

    object *curr = *logic.head;
    while (curr != nullptr) {
        //render object
        string tex;
        switch (curr->obj) {
            case ghost_red:
                tex += "ghost_red";
                switch (curr->direction) {
                    case up:
                        tex += "_up";
                        break;
                    case down:
                        tex += "_down";
                        break;
                    case state::left:
                        tex += "_left";
                        break;
                    case state::right:
                        tex += "_right";
                        break;
                    case null:
                        curr = curr->next;
                        continue;
                }
                // code for ghost_red
                entityQueue.push({tex, {curr->pos.x, curr->pos.y}});
                curr = curr->next;
                continue;
            case ghost_yellow:
                tex += "ghost_yellow";
                switch (curr->direction) {
                    case up:
                        tex += "_up";
                        break;
                    case down:
                        tex += "_down";
                        break;
                    case state::left:
                        tex += "_left";
                        break;
                    case state::right:
                        tex += "_right";
                        break;
                    case null:
                        curr = curr->next;
                        continue;
                }
                // code for ghost_yellow
                entityQueue.push({tex, {curr->pos.x, curr->pos.y}});
                curr = curr->next;
                continue;
            case ghost_pink:
                tex += "ghost_pink";
                switch (curr->direction) {
                    case up:
                        tex += "_up";
                        break;
                    case down:
                        tex += "_down";
                        break;
                    case state::left:
                        tex += "_left";
                        break;
                    case state::right:
                        tex += "_right";
                        break;
                    case null:
                        curr = curr->next;
                        continue;
                }
                // code for ghost_pink
                entityQueue.push({tex, {curr->pos.x, curr->pos.y}});
                curr = curr->next;
                continue;
            case player:
                tex = "player";
                if (this->logic.powerPellet) {
                    tex += "_pp";
                } else {
                    tex += "_norm";
                }
                switch (curr->direction) {
                    case up:
                        tex += "_up";
                        break;
                    case down:
                        tex += "_down";
                        break;
                    case state::left:
                        tex += "_left";
                        break;
                    case state::right:
                        tex += "_right";
                        break;
                    case null:
                        curr = curr->next;
                        continue;
                }
                tex += std::to_string((this->logic.tickNum % 4) + 1);
                // code for player
                entityQueue.push({tex, {curr->pos.x, curr->pos.y}});
                curr = curr->next;
                continue;
            case pp:
                tex = "pp";
                break;
            case pellet:
                tex = "pellet";
                Renderer->DrawSprite(ResourceManager::GetTexture(tex),
                                     glm::vec2(curr->pos.x * 40.0f + 5.0f, curr->pos.y * 40.0f + 5.0f),
                                     glm::vec2(30.0f, 30.0f),
                                     0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
                curr = curr->next;
                continue;
            default:
                curr = curr->next;
                continue;
        }
        Renderer->DrawSprite(ResourceManager::GetTexture(tex),
                             glm::vec2(curr->pos.x * 40.0f, curr->pos.y * 40.0f),
                             glm::vec2(40.0f, 40.0f),
                             0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
        curr = curr->next;
    }
    while (!entityQueue.empty()) {
        std::pair<string, std::pair<int, int>> curr = entityQueue.front();
        entityQueue.pop();
        Renderer->DrawSprite(ResourceManager::GetTexture(curr.first),
                             glm::vec2(curr.second.first * 40.0f, curr.second.second * 40.0f),
                             glm::vec2(40.0f, 40.0f),
                             0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
    }
}