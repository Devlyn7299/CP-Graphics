#include "BasicParticle.h"

float BasicParticle::getLife() const
{
    return life;
}

void BasicParticle::setLife(float life)
{
    this->life = life;
}

glm::vec3 BasicParticle::getPosition() const
{
    return position;
}

void BasicParticle::setPosition(glm::vec3 position)
{
    this->position = position;
}

float BasicParticle::getBrightness() const
{
    return brightness;
}

void BasicParticle::setBrightness(float brightness)
{
    this->brightness = brightness;
}

void BasicParticle::update(float dt)
{
    // Life decreases by 1 every second
    life = life - dt;

    // update position, velocity, and/or others
    if (life > 0.75f)
    {
        cout << "yeah" << endl;
        brightness -= 50.0f * dt;
        if (brightness < 0.5f)
        {
            brightness = 0.5f;
        }
    }
}
