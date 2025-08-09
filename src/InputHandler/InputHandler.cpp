#include "InputHandler.hpp"

InputHandler::InputHandler()
    : noChao(true), velocidadeX(0.0f), velY(0.0f)
{
    Keyboard::setMode(Keyboard::NONBLOCKING);
}

char InputHandler::readKey() const {
    return Keyboard::read();
}

bool InputHandler::handleKey(char key, Hero& hero,
                             const ObjetoDeJogo& col1,
                             const ObjetoDeJogo& col2) {
    if (key == 'q') {
        return false; // sinaliza pra sair do jogo
    }

    // Movimento horizontal
    if (key == 'a') {
        velocidadeX -= acel;
    } else if (key == 'd') {
        velocidadeX += acel;
    } else {
        // desaceleração no chão
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

    // Pulo
    if (key == 'w' && noChao) {
        velY = forcaPulo;
        noChao = false;
    }

    // Aplica gravidade
    velY += gravidade;
    velY = std::clamp(velY, -15.0f, 15.0f);

    // Move horizontal pixel a pixel com colisão
    if (velocidadeX < 0) {
        for (int i = 0; i < (int)std::abs(velocidadeX); ++i) {
            hero.moveLeft(1);
            // aqui pode colocar colisão lateral se quiser
        }
    } else if (velocidadeX > 0) {
        for (int i = 0; i < (int)velocidadeX; ++i) {
            hero.moveRight(1);
            // aqui pode colocar colisão lateral se quiser
        }
    }

    // Move vertical pixel a pixel com colisão
    if (velY < 0) {
        for (int i = 0; i < (int)std::abs(velY); ++i) {
            hero.moveUp(1);
            if (hero.colideCom(col1) || hero.colideCom(col2)) {
                hero.moveDown(1);
                velY = 0;
                break;
            }
        }
    } else if (velY > 0) {
        for (int i = 0; i < (int)velY; ++i) {
            hero.moveDown(1);
            if (hero.colideCom(col1) || hero.colideCom(col2)) {
                hero.moveUp(1);
                velY = 0;
                noChao = true;
                break;
            } else {
                noChao = false;
            }
        }
    }

    return true; // continuar rodando
}
