#include "Game.hpp"

Game::Game()
    : mapa("src/assets/bg.txt"),
      mapa_colisao("Colisao do mapa", Sprite("src/assets/colide_piso1.txt"), 125, 0),
      mapa_colisao2("Colisao do mapa", Sprite("src/assets/colide_piso2.txt"), 125, 576),
      camera(mapa, 0, 0, CAM_HEIGHT, CAM_WIDTH),
      hero("Hero", SpriteAnimado("src/assets/hero2.txt", 14), CAM_HEIGHT / 2, CAM_WIDTH / 2),
      screen(CAM_WIDTH, CAM_HEIGHT, ' '),
      sprite_menu("src/assets/menu.txt"),
      input()
{
    cameraLin = 0;
    cameraCol = 0;
    running = false;
    menu_running = false;
}
Game::~Game() {
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

    // Se handleKey retornar false, para o jogo
    if (!input.handleKey(key, hero, mapa_colisao, mapa_colisao2)) {
        running = false;
    }

    // Atualiza câmera para acompanhar o herói
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
    hero.update();
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