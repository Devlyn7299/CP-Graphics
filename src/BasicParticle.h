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

    // Updates the particle for a specified time increment (dt).
    void update(float dt) override;

private:
    float life{ 1.0f };
    glm::vec3 position;
    float brightness{ 10.0f };

};