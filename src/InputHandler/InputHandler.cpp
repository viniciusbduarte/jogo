#include "InputHandler.hpp"

char InputHandler::readKey() const {
    return Keyboard::read();
}

void InputHandler::handleKey(char key, ObjetoDeJogo& obj) const {
    switch (key) {
        case 'w': obj.moveUp(1); break;
        case 's': obj.moveDown(1); break;
        case 'a': obj.moveLeft(1); break;
        case 'd': obj.moveRight(1); break;
        default: break;
    }
}
