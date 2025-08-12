#ifndef GAME_HPP
#define GAME_HPP

#include "../../ASCII_Engine/SpriteBuffer.hpp"
#include "../../ASCII_Engine/SpriteView.hpp"
#include "../../ASCII_Engine/ObjetoDeJogo.hpp"
#include "../../ASCII_Engine/Sprite.hpp"
#include "../classes/Hero.hpp"
#include "../classes/Moeda.hpp"
#include "../classes/Inimigo.hpp"
#include "../classes/Chest.hpp"
#include "../classes/Chave.hpp"
#include "../InputHandler/InputHandler.hpp"
#include <set>
#include <cmath>
#include <algorithm> // clamp()
#include <iostream>
#include <unistd.h>  // usleep()
#include <vector> // vector<>

class Game {
private:
    static constexpr int CAM_HEIGHT = 96;
    static constexpr int CAM_WIDTH = 320;

    Sprite mapa;
    Sprite coracao;
    Sprite vida;
    Sprite barra_vida;
    Sprite bau_aberto;
    std::vector<ObjetoDeJogo> colisoes; 
    std::vector<ObjetoDeJogo> moedas; 
    Chave chave;
    Chest bau;
    Sprite sprite_menu;
    SpriteView camera;
    SpriteBuffer screen;
    Hero hero;
    Inimigo inimigo;
    InputHandler input;

    int cameraLin, cameraCol;
    bool running, menu_running, gameover_running, move_inimigo;


public:
    Game();
    ~Game();

    void menu();
    void init();
    void update();
    void render();
    void run();

    void GameOver();
};

#endif // GAME_HPP