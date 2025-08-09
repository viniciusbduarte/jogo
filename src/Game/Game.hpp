#ifndef GAME_HPP
#define GAME_HPP

#include "../../ASCII_Engine/SpriteBuffer.hpp"
#include "../../ASCII_Engine/SpriteView.hpp"
#include "../../ASCII_Engine/ObjetoDeJogo.hpp"
#include "../../ASCII_Engine/Sprite.hpp"
#include "../classes/Hero.hpp"
#include "../InputHandler/InputHandler.hpp"
#include <set>
#include <cmath>
#include <algorithm> // clamp()
#include <iostream>
#include <unistd.h>  // usleep()

class Game {
private:
    static constexpr int CAM_HEIGHT = 96;
    static constexpr int CAM_WIDTH = 320;

    Sprite mapa;
    ObjetoDeJogo mapa_colisao;
    ObjetoDeJogo mapa_colisao2;
    Sprite sprite_menu;
    SpriteView camera;
    SpriteBuffer screen;
    Hero hero;
    InputHandler input;

    int cameraLin, cameraCol;
    bool running;
    bool menu_running;


public:
    Game();
    ~Game();

    void menu();
    void init();
    void update();
    void render();
    void run();
};

#endif // GAME_HPP