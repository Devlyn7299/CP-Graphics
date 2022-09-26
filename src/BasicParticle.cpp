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

glm::vec3 BasicParticle::getScale() const
{
    return scale;
}

void BasicParticle::setScale(glm::vec3 scale)
{
    this->scale = scale;
}

glm::vec4 BasicParticle::getColor() const
{
    return color;
}

void BasicParticle::setColor(glm::vec4 color)
{
    this->color = color;
}

string BasicParticle::getType() const
{
    return type;
}

void BasicParticle::setType(string type)
{
    this->type = type;
}



void BasicParticle::update(float dt)
{
    //cout << " life " << life << endl;
    // Life decreases by 1 every second
    life -= dt;

    // update position, velocity, and/or others
    if (type == "lightning")
    {
        if (life > 2.5f)
        {
            brightness -= 8.5f * dt;
            if (brightness < 1.0f)
            {
                brightness = 1.0f;
            }
        }
        if (life < 3.5f)
        {
            brightness -= 0.5f * dt;
        }
    }

    if (type == "cloud")
    {
        position += glm::vec3(0.25f * dt, 0.0, 0.0);
    }
}

int BasicParticle::getTextureNum() const
{
    return textureNum;
}

void BasicParticle::setTextureNum(int textureNum)
{
    this->textureNum = textureNum;
    //cout << textureNum << endl;
}
