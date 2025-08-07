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
    bool pulando;
    int pulo;

    // Física e movimento
    std::set<char> teclasPressionadas;

    float velocidadeX = 0.0f;
    const float acel = 0.5f;
    const float desacel = 0.2f;
    const float velMax = 5.0f;

    float velY = 0.0f;
    const float gravidade = 1.0f;
    const float forcaPulo = -10.0f;

    bool noChao = true;

public:
    Game();
    void menu();
    void init();
    void update();
    void render();
    void run();
};

#endif // GAME_HPP
