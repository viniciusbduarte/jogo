#ifndef INPUT_HANDLER_HPP
#define INPUT_HANDLER_HPP

#include "../../ASCII_Engine/input/Keyboard.hpp"
#include "../classes/Hero.hpp"

class InputHandler {
private:
    int jump{5};
public:
    InputHandler(); 

    char readKey() const;
    void handleKey(char key, ObjetoDeJogo& obj) const;
};

#endif
