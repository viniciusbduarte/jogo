#ifndef HERO_HPP
#define HERO_HPP

#include "../../ASCII_Engine/ObjetoDeJogo.hpp"

class Hero : public ObjetoDeJogo {
public:
    using ObjetoDeJogo::ObjetoDeJogo; // herda o construtor
};

#endif
