#pragma once

#include <map>
#include "glm_common.hpp"
#include "input.hpp"

class Game {
public:
    bool IsExit = false;
    unsigned int Width, Height;

    ~Game();

    void Initialize();

    void Resize(unsigned int width, unsigned int height);

    void Update(float deltaTime);

    void Render();
};