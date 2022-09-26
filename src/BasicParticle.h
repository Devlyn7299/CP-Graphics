#pragma once
#include "ofMain.h"
#include "Particle.h"

class BasicParticle : public Particle
{
public:
    // Gets how much life the particle has left.  
    // When a particle's life is reduced to zero, it will be removed from the particle system.
    float getLife() const override;

    // Sets the particle's life
    void setLife(float life);

    // Gets the particle's position
    glm::vec3 getPosition() const;

    // Sets the particle's position
    void setPosition(glm::vec3 position);

    // Gets the particle's brightness
    float getBrightness() const;

    // Sets the particle's brightness
    void setBrightness(float brightness);

    // Gets the particle's scale
    glm::vec3 getScale() const;

    // Sets the particle's scale
    void setScale(glm::vec3 scale);

    // Gets the particle's color
    glm::vec4 getColor() const;

    // Sets the particle's color
    void setColor(glm::vec4 color);

    // Gets the particle's type
    string getType() const;

    // Sets the particle's type
    void setType(string type);

    // Gets the particle's texture number
    int getTextureNum() const;

    // Sets the particle's texture number
    void setTextureNum(int textureNum);

    // Updates the particle for a specified time increment (dt).
    void update(float dt) override;



private:
    float life;             // Particle's life
    glm::vec3 position;     // Particle's position
    float brightness;       // Particle's brightness
    glm::vec3 scale;        // Particle's scale
    glm::vec4 color;        // Particle's color
    string type{};          // Particle's type, either "cloud" or "lightning"
    int textureNum{1};      // Particle's texture number, determines what texture to use, also based on type

};