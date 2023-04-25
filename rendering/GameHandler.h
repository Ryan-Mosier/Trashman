//
// Created by ryanm on 4/17/2023.
//

#ifndef MYPROJECT_GAMEHANDLER_H
#define MYPROJECT_GAMEHANDLER_H

#include "../Game/Game.h"

class GameHandler {
public:
    GameState State;
    std::pair<bool, double> Keys[1024];
    float timeSinceLastTick;
    unsigned int Width, Height;
    gameLogic logic;

    // constructor/destructor
    GameHandler(unsigned int width, unsigned int height, bool debug);

    ~GameHandler();

    // initialize game state (load all shaders/textures/levels)
    void Init();

    //waiting
    void waiting();

    //dead
    void dead();

    // game loop
    void ProcessInput(float dt);

    void Update(float dt);

    void drawMap();

    void Render();
};


#endif //MYPROJECT_GAMEHANDLER_H
