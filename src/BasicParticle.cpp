#include "BasicParticle.h"

float BasicParticle::getLife() const
{
    return life;
}

void BasicParticle::setLife(float life)
{
    this->life = life;
}

void BasicParticle::update(float dt)
{
    // Life decreases by 0.5 every second
    life -= dt * 0.5;

    // update position, velocity, and/or others
}
