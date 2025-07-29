#include <iostream>
#include <unistd.h>
#include "../ASCII_Engine/SpriteBuffer.hpp"
#include "../ASCII_Engine/SpriteView.hpp"
#include "../ASCII_Engine/Sprite.hpp"
#include "../ASCII_Engine/ObjetoDeJogo.hpp"
#include "../ASCII_Engine/input/Keyboard.hpp"
#include <algorithm>

int main() {
    system("clear");

    // Configurações da tela
    const int ALTURA = 24;
    const int LARGURA = 80;
    
    SpriteBuffer tela(LARGURA, ALTURA, ' ');

    Sprite mapa("src/bg.txt");

    ObjetoDeJogo heroi("Heroi", Sprite("src/hero2.txt"), ALTURA / 2, LARGURA / 2);

    SpriteView camera(mapa, 0, 0, ALTURA, LARGURA);

    Keyboard::setMode(Keyboard::NONBLOCKING);

    // Posição alvo da câmera
    int cam_l = heroi.getPosL() - ALTURA / 2;
    int cam_c = heroi.getPosC() - LARGURA / 2;

    while (true) {
        char tecla = Keyboard::read();
        if (tecla == 'q') break;

        if (tecla == 'w') heroi.moveUp(1);
        if (tecla == 's') heroi.moveDown(1);
        if (tecla == 'a') heroi.moveLeft(1);
        if (tecla == 'd') heroi.moveRight(1);

        // Atualiza posição da câmera com base no herói
        cam_l = heroi.getPosL() - ALTURA / 2;
        cam_c = heroi.getPosC() - LARGURA / 2;

        // Limita a câmera aos limites do mapa (opcional)
        // cam_l = std::clamp(cam_l, 0, mapa.getAltura() - ALTURA);
        // cam_c = std::clamp(cam_c, 0, mapa.getLarguraMax() - LARGURA);

        camera.moveTo(cam_l, cam_c);

        tela.clear();
        camera.draw(tela, 0, 0);

        int desenho_l = heroi.getPosL() - cam_l;
        int desenho_c = heroi.getPosC() - cam_c;

        if (desenho_l >= 0 && desenho_l < ALTURA &&
            desenho_c >= 0 && desenho_c < LARGURA) {
            heroi.draw(tela, desenho_l - 2, desenho_c - 7);
        }

        std::cout << "\033[2J\033[H";
        std::cout << tela << std::endl;

        std::cout << "Herói: (" << heroi.getPosL() << "," << heroi.getPosC() << ") | "
                  << "Câmera: (" << cam_l << "," << cam_c << ") | "
                  << "Desenho: (" << desenho_l << "," << desenho_c << ")\n";

        usleep(16000);
    }

    return 0;
}
