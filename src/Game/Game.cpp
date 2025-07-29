#include "Game.hpp"
#include <algorithm>
#include <iostream>
#include <unistd.h>  // usleep()

Game::Game()
    : mapa("src/assets/bg.txt"),
      camera(mapa, 0, 0, CAM_HEIGHT, CAM_WIDTH),
      hero("Hero", Sprite("src/assets/hero2.txt"), CAM_HEIGHT/2, CAM_WIDTH/2),
      screen(CAM_WIDTH, CAM_HEIGHT, ' ')
{
    cameraLin = 0;
    cameraCol = 0;
    running = false;
}

void Game::init() {
    system("clear");
    camera.moveTo(cameraLin, cameraCol);
}

void Game::update() {
    char key = input.readKey();

    if (key == 'q') {
        running = false;
        return;
    }

    input.handleKey(key, hero);

    cameraLin = hero.getPosL() - CAM_HEIGHT / 2;
    cameraCol = hero.getPosC() - CAM_WIDTH / 2;

    cameraLin = std::clamp(cameraLin, 0, mapa.getAltura() - CAM_HEIGHT);
    cameraCol = std::clamp(cameraCol, 0, mapa.getLarguraMax() - CAM_WIDTH);

    camera.moveTo(cameraLin, cameraCol);
}

void Game::render() {
    screen.clear();

    camera.draw(screen, 0, 0);

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
    running = true;
    while (running) {
        update();
        render();
        usleep(16000); // ~60 FPS
    }
}
