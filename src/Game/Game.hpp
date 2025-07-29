#ifndef GAME_HPP
#define GAME_HPP

#include "../../ASCII_Engine/SpriteBuffer.hpp"
#include "../../ASCII_Engine/SpriteView.hpp"
#include "../../ASCII_Engine/Sprite.hpp"
#include "../classes/Hero.hpp"
#include "../InputHandler/InputHandler.hpp"

class Game {
private:
    static constexpr int CAM_HEIGHT = 24;
    static constexpr int CAM_WIDTH = 80;

    Sprite mapa;
    SpriteView camera;
    SpriteBuffer screen;
    Hero hero;
    InputHandler input;

    int cameraLin, cameraCol;
    bool running;

public:
    Game();
    void init();
    void update();
    void render();
    void run();
};

#endif // GAME_HPP
