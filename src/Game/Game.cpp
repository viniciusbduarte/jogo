#include "Game.hpp"
#include <algorithm>
#include <iostream>
#include <unistd.h>  // usleep()

Game::Game()
    : mapa("src/assets/bg.txt"),
      mapa_colisao("Colisao do mapa", Sprite("src/assets/colide_piso1.txt"), 125, 0),
      mapa_colisao2("Colisao do mapa", Sprite("src/assets/colide_piso2.txt"), 125, 576),
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

    // Adiciona tecla ao set se pressionada
    if (key != '\0') {
        teclasPressionadas.insert(key);
    }

    // Inicia o pulo se estiver no chão
    if (key == 'w' && noChao) {
        velY = forcaPulo;
        noChao = false;
    }

    // Movimento horizontal - aceleração funciona sempre, no ar ou no chão
    if (teclasPressionadas.count('a')) {
        velocidadeX -= acel;
    } else if (teclasPressionadas.count('d')) {
        velocidadeX += acel;
    } else {
        // desaceleração só se estiver no chão (para permitir controle mais livre no ar)
        if (noChao) {
            if (velocidadeX > 0) {
                velocidadeX -= desacel;
                if (velocidadeX < 0) velocidadeX = 0;
            } else if (velocidadeX < 0) {
                velocidadeX += desacel;
                if (velocidadeX > 0) velocidadeX = 0;
            }
        }
    }

    velocidadeX = std::clamp(velocidadeX, -velMax, velMax);

    // Move horizontal pixel a pixel
    if (velocidadeX < 0) {
        for (int i = 0; i < (int)std::abs(velocidadeX); ++i) {
            hero.moveLeft(1);
            // Aqui você pode adicionar colisão lateral
        }
    } else if (velocidadeX > 0) {
        for (int i = 0; i < (int)velocidadeX; ++i) {
            hero.moveRight(1);
            // Aqui você pode adicionar colisão lateral
        }
    }

    // Aplica gravidade no pulo
    velY += gravidade;
    velY = std::clamp(velY, -15.0f, 15.0f);

    // Move vertical pixel a pixel com colisão
    if (velY < 0) {
        for (int i = 0; i < (int)std::abs(velY); ++i) {
            hero.moveUp(1);
            if (hero.colideCom(mapa_colisao) || hero.colideCom(mapa_colisao2)) {
                hero.moveDown(1);
                velY = 0;
                break;    
            }
        }
    } else if (velY > 0) {
        for (int i = 0; i < (int)velY; ++i) {
            hero.moveDown(1);
            if (hero.colideCom(mapa_colisao) || hero.colideCom(mapa_colisao2)) {
                hero.moveUp(1);
                velY = 0;
                noChao = true;
                break;
            } else {
                noChao = false;
            }
        }
    }

    // Só limpa o set de teclas quando não tiver nenhuma tecla nova pressionada neste frame
    if (key == '\0') {
        teclasPressionadas.clear();
    }

    // Atualiza câmera
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
