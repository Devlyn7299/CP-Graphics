#pragma once
#include "ofMain.h"
#include "Particle.h"

class BasicParticle : public Particle
{
public:
    // Gets how much life the particle has left.  
    // When a particle's life is reduced to zero, it will be removed from the particle system.
    float getLife() const override;

    void setLife(float life);

    glm::vec3 getPosition() const;

    void setPosition(glm::vec3 position);

    float getBrightness() const;

    void setBrightness(float brightness);

    glm::vec3 getScale() const;

    void setScale(glm::vec3 scale);

    glm::vec4 getColor() const;

    void setColor(glm::vec4 color);

    string getType() const;

    void setType(string type);

    // Updates the particle for a specified time increment (dt).
    void update(float dt) override;

    int getTextureNum() const;

    void setTextureNum(int textureNum);

private:
    float life{ 1.0f };// { 10.0f };
    glm::vec3 position {10.0, 0.0, 0.0};
    float brightness;// { 10.0f };
    glm::vec3 scale;// {0.5, 0.5, 0.0};
    glm::vec4 color;// { 0, 0, 1, 0 };
    string type{};
    int textureNum{1};

};