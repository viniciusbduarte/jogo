#include "Game.hpp"

Game::Game()
    : mapa("src/assets/bg.txt"),
      coracao("src/assets/coracao.txt", COR::VERMELHA),
      vida("src/assets/vida_template.txt", COR::CINZA),
      barra_vida("src/assets/barra_vida.txt", COR::VERMELHA),
      camera(mapa, 0, 0, CAM_HEIGHT, CAM_WIDTH),
      hero("Hero", SpriteAnimado("src/assets/hero2.txt", 14), 100, 20),
      chave("Chave", SpriteAnimado("src/assets/chave.txt", 14, COR::AMARELA), 27, 305),
      bau("Baú", Sprite("src/assets/bau_fechado.txt"), 110, 630),
      inimigo("Inimigo", SpriteAnimado("src/assets/inimigo.txt", 16, COR::MARROM), 114, 200),
      screen(CAM_WIDTH, CAM_HEIGHT, ' '),
      sprite_menu("src/assets/menu.txt"),
      input()
{
    cameraLin = 0;
    cameraCol = 0;
    running = false;
    menu_running = false;
    gameover_running = false;
    move_inimigo = false;

    // moedas
    moedas.emplace_back("Moeda", SpriteAnimado("src/assets/moeda.txt", 10, COR::AMARELA), 77, 70);
    moedas.emplace_back("Moeda", SpriteAnimado("src/assets/moeda.txt", 10, COR::AMARELA), 77, 385);
    moedas.emplace_back("Moeda", SpriteAnimado("src/assets/moeda.txt", 10, COR::AMARELA), 47, 500);
    moedas.emplace_back("Moeda", SpriteAnimado("src/assets/moeda.txt", 10, COR::AMARELA), 112, 150);
    moedas.emplace_back("Moeda", SpriteAnimado("src/assets/moeda.txt", 10, COR::AMARELA), 112, 600);

    // colisões do mapa
    colisoes.emplace_back("Colisao do mapa", Sprite("src/assets/colide_piso1.txt"), 125, 0);
    colisoes.emplace_back("Colisao do mapa", Sprite("src/assets/colide_piso2.txt"), 125, 576);
    colisoes.emplace_back("Colisao do mapa", Sprite("src/assets/bloco2.txt"), 90, 50);
    colisoes.emplace_back("Colisao do mapa", Sprite("src/assets/bloco1.txt"), 60, 200);
    colisoes.emplace_back("Colisao do mapa", Sprite("src/assets/bloco1.txt"), 40, 300);
    colisoes.emplace_back("Colisao do mapa", Sprite("src/assets/bloco1.txt"), 60, 490);
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
    hero = Hero("Hero", SpriteAnimado("src/assets/hero2.txt", 14), 100, 20);

    // Reinicia a câmera
    cameraLin = 0;
    cameraCol = 0;
    camera.moveTo(cameraLin, cameraCol);

    for (auto& moeda : moedas) {

        moeda.ativarObj();

    }


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

    if(inimigo.getPosC() == 5){
        move_inimigo = false;
    }
    else if(inimigo.getPosC() == 350 ){
        move_inimigo = true;
    }


    if (move_inimigo)inimigo.moveLeft(1);
    if (!move_inimigo)inimigo.moveRight(1);

    // variáveis estáticas para manter estado entre frames
    static bool podeLevarDano = true;
    static int danoCooldown = 0; // contador em frames

    // verifica colisão com inimigo
    if (hero.colideCom(inimigo) && podeLevarDano) {
        hero.damage();
        podeLevarDano = false;
        danoCooldown = 60; // delay de 60 frames (~1 segundo a 60 FPS)
    }

    // controla o cooldown
    if (!podeLevarDano) {
        if (--danoCooldown <= 0) {
            podeLevarDano = true;
        }
    }

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

    //verifica se o herói pegou a chave
    if(hero.colideCom(chave)){
        chave.desativarObj();
        hero.pegouChave();
    }

    /*if(hero.colideCom(bau) && hero.isKey()){
        bau.
    }
*/
    for (auto& moeda : moedas) {
        if (hero.colideCom(moeda)) {
            moeda.desativarObj();
        }
        moeda.update();
    }
    inimigo.update();
    chave.update();
    bau.update();
    hero.update();

}

void Game::render() {
    screen.clear();

    auto drawIfVisible = [&](auto& obj, int offsetL = 0, int offsetC = 0) {
        int screenL = obj.getPosL() - cameraLin;
        int screenC = obj.getPosC() - cameraCol;

        if (screenL >= 0 && screenL < CAM_HEIGHT &&
            screenC >= 0 && screenC < CAM_WIDTH) {
            obj.draw(screen, screenL + offsetL, screenC + offsetC);
        }
    };

    // Colisões (desenhadas diretamente no mapa, sem offset de câmera)
    for (auto& c : colisoes) {
        c.draw(mapa, c.getPosL() - 2, c.getPosC() - 10);
    }

    // Mapa/câmera
    camera.draw(screen, 0, 0);

    // Objetos
    drawIfVisible(chave);
    drawIfVisible(bau);
    drawIfVisible(inimigo, 0, -8);
    drawIfVisible(hero, -2, -7);
    for (auto& moeda : moedas) {
        drawIfVisible(moeda);
    }

    // HUD - Vidas
    int vidas = hero.getLifes();
    if (vidas >= 1) coracao.draw(screen, 5, 260);
    if (vidas >= 2) coracao.draw(screen, 5, 280);
    if (vidas >= 3) coracao.draw(screen, 5, 300);

    // HUD - Barra de vida
    vida.draw(screen, 3, 5);
    int HP = hero.getHP();
    int baseX = 23;      
    int spriteWidth = 6;
    int numBarras = HP / 10;

    for (int i = 0; i < numBarras; ++i) {
        barra_vida.draw(screen, 7, baseX + i * (spriteWidth + 1));
    }

    // Output final
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
