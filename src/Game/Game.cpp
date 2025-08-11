#include "Game.hpp"

Game::Game()
    : mapa("src/assets/bg.txt"),
      coracao("src/assets/coracao.txt"),
      camera(mapa, 0, 0, CAM_HEIGHT, CAM_WIDTH),
      hero("Hero", SpriteAnimado("src/assets/hero2.txt", 14), CAM_HEIGHT / 2, CAM_WIDTH / 2),
      moeda("Moeda", SpriteAnimado("src/assets/moeda.txt", 10), 75, 70),
      inimigo("Inimigo", SpriteAnimado("src/assets/inimigo.txt", 16), 114, 200),
      screen(CAM_WIDTH, CAM_HEIGHT, ' '),
      sprite_menu("src/assets/menu.txt"),
      input()
{
    cameraLin = 0;
    cameraCol = 0;
    running = false;
    menu_running = false;
    gameover_running = false;

    // colisões do mapa
    colisoes.emplace_back("Colisao do mapa", Sprite("src/assets/colide_piso1.txt"), 125, 0);
    colisoes.emplace_back("Colisao do mapa", Sprite("src/assets/colide_piso2.txt"), 125, 576);
    colisoes.emplace_back("Colisao do mapa", Sprite("src/assets/bloco2.txt"), 90, 50);
    colisoes.emplace_back("Colisao do mapa", Sprite("src/assets/bloco2.txt"), 90, 376);

}
Game::~Game() {
}

void Game::init() {
    system("clear");
    camera.moveTo(cameraLin, cameraCol);
} 

void Game::GameOver() {
    // Reinicia o herói
    hero = Hero("Hero", SpriteAnimado("src/assets/hero2.txt", 14),
                CAM_HEIGHT / 2, CAM_WIDTH / 2);

    // Reinicia a câmera
    cameraLin = 0;
    cameraCol = 0;
    camera.moveTo(cameraLin, cameraCol);

    // Volta ao menu
    menu_running = true;
    running = false;
    menu();
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
    if (!input.handleKey(key, hero, colisoes)) {
        running = false;
    }


    // Atualiza câmera para acompanhar o herói
    cameraLin = hero.getPosL() - CAM_HEIGHT / 2;
    cameraCol = hero.getPosC() - CAM_WIDTH / 2;

    cameraLin = std::clamp(cameraLin, 3, mapa.getAltura() - CAM_HEIGHT - 3);
    cameraCol = std::clamp(cameraCol, 3, mapa.getLarguraMax() - CAM_WIDTH - 3);

    camera.moveTo(cameraLin, cameraCol);


    // se o herói cair no buraco ele morre 
    static constexpr int FALL_LIMIT = 130;
    if (hero.getPosL() > FALL_LIMIT){
        hero.dead();
        hero.moveTo(50, 50);
    }
    // verifica se herói ainda tem vida
    if (!hero.isAlive()){
        GameOver();
    }

}

void Game::render() {
    screen.clear();

    for (auto& c : colisoes) {
        c.draw(mapa, c.getPosL() - 2, c.getPosC() - 10);
    }

    camera.draw(screen, 0, 0);

    int moedaScreenL = moeda.getPosL() - cameraLin;
    int moedaScreenC = moeda.getPosC() - cameraCol;

    if (moedaScreenL >= 0 && moedaScreenL < CAM_HEIGHT &&
        moedaScreenC >= 0 && moedaScreenC < CAM_WIDTH) {
        moeda.draw(screen, moedaScreenL, moedaScreenC);
    }

    int inimigoScreenL = inimigo.getPosL() - cameraLin;
    int inimigoScreenC = inimigo.getPosC() - cameraCol;

    if (inimigoScreenL >= 0 && inimigoScreenL < CAM_HEIGHT &&
        inimigoScreenC >= 0 && inimigoScreenC < CAM_WIDTH) {
        inimigo.draw(screen, inimigoScreenL, inimigoScreenC);
    }

    int drawL = hero.getPosL() - cameraLin;
    int drawC = hero.getPosC() - cameraCol;

    if (drawL >= 0 && drawL < CAM_HEIGHT &&
        drawC >= 0 && drawC < CAM_WIDTH) {
        hero.draw(screen, drawL - 2, drawC - 7);
    }


    hero.update();
    moeda.update();
    inimigo.update();

    
    int vidas = hero.getLifes();
    if (vidas >= 1) coracao.draw(screen, 5, 260);
    if (vidas >= 2) coracao.draw(screen, 5, 280);
    if (vidas >= 3) coracao.draw(screen, 5, 300);

    std::cout << "\033[2J\033[H" << screen << std::endl;

}

void Game::run() {
    while (true) {
        menu_running = true;
        running = false;

        menu(); // Espera apertar 'z'

        running = true;
        while (running) {
            update();
            render();
            usleep(16000);
        }
    }
}
