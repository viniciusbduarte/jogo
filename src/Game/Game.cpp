#include "Game.hpp"
#include <algorithm>
#include <iostream>
#include <unistd.h>  // usleep()

Game::Game()
    : mapa("src/assets/bg.txt"),
      mapa_colisao("Colisao do mapa", Sprite("src/assets/colide.txt"), 80, 80),
      camera(mapa, 0, 0, CAM_HEIGHT, CAM_WIDTH),
      hero("Hero", Sprite("src/assets/hero2.txt"), CAM_HEIGHT / 2, CAM_WIDTH / 2),
      screen(CAM_WIDTH, CAM_HEIGHT, ' '),
      sprite_menu("src/assets/menu.txt"),
      input()
{
    cameraLin = 0;
    cameraCol = 0;
    running = false;
    menu_running = false;
    pulando = false;
    pulo = 0;
}

void Game::init() {
    system("clear");
    camera.moveTo(cameraLin, cameraCol);
}

void Game::menu() {
    while (menu_running) {
        screen.clear();
        sprite_menu.draw(screen, 0, 0);

        std::cout << "\033[2J\033[H" << screen << std::endl;

        char key = input.readKey();
        if (key == 'z') {
            running = true;
            menu_running = false;
        }

        usleep(16000); // ~60 FPS
    }
}

void Game::update() {
    char key = input.readKey();

    if (key == 'q') {
        running = false;
        return;
    }

    // Inicia o pulo se estiver no chão
    if (key == 'w' && pulo == 0 && !pulando) {
        pulando = true;
        pulo = 5; // Altura máxima do pulo
    }

    // Lógica de pulo
    if (pulando) {
        if (pulo > 0) {
            // Subindo
            hero.moveUp(1);
            pulo--;
            if (pulo == 0) pulo = -1; // Começa a cair
        } else {
            // Caindo
            hero.moveDown(1);

            if (hero.colideComBordas(mapa_colisao)) {
                pulando = false;
                pulo = 0;
            } else {
                pulo--; // Continua caindo
            }
        }
    } else {
        // Movimento normal se não estiver pulando
        switch (key) {
            case 'w': hero.moveUp(1); break;
            case 's': hero.moveDown(1); break;
            case 'a': hero.moveLeft(1); break;
            case 'd': hero.moveRight(1); break;
            default: break;
        }
    }

    // Atualiza posição da câmera
    cameraLin = hero.getPosL() - CAM_HEIGHT / 2;
    cameraCol = hero.getPosC() - CAM_WIDTH / 2;

    cameraLin = std::clamp(cameraLin, 0, mapa.getAltura() - CAM_HEIGHT);
    cameraCol = std::clamp(cameraCol, 0, mapa.getLarguraMax() - CAM_WIDTH);

    camera.moveTo(cameraLin, cameraCol);
}

void Game::render() {
    screen.clear();
    camera.draw(screen, 0, 0);
//  mapa_colisao.draw(mapa, 80, 80);
 //   mapa_colisao.draw(mapa, 120, 120);  

    int drawL = hero.getPosL() - cameraLin;
    int drawC = hero.getPosC() - cameraCol;

    if (drawL >= 0 && drawL < CAM_HEIGHT &&
        drawC >= 0 && drawC < CAM_WIDTH) {
        hero.draw(screen, drawL - 2, drawC - 7);
    }

    std::cout << "\033[2J\033[H" << screen << std::endl;

    std::cout << "Hero: (" << hero.getPosL() << "," << hero.getPosC() << ") | "
              << "Camera: (" << cameraLin << "," << cameraCol << ") | "
              << "DrawPos: (" << drawL << "," << drawC << ")" << std::endl;
}

void Game::run() {
    menu_running = true;
    running = false;

    menu(); // Exibe menu até apertar 'z'

    while (running) {
        update();
        render();
        usleep(16000); // ~60 FPS
    }
}
