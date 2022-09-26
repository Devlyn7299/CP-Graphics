#include "BasicParticleGenerator.h"

void BasicParticleGenerator::respawn(BasicParticle& particle) const
{
	// If the particle is lightning
	if (particle.getType() == "lightning")
	{
		// Choosing a texture out of the available lightning textures
		int r = rand() % 4 + 1;
		particle.setTextureNum(r);

		// Life is initialized to 5
		particle.setLife(5.0f);

		// Randomizing lightning particle's position
		r = rand() % 11;
		double posX = 2 * r / 10.0 - 1;
		r = rand() % 11;
		double posY = 2 * r / 10.0 - 1;
		particle.setPosition(glm::vec3(posX, posY, 1.0));

		// Setting starting brightness
		particle.setBrightness(10.0f);

		// Setting lightning color
		particle.setColor(particle.getColor() + glm::vec4(0.025, 0.025, 1, 0));

		// Randomizing lightning particle's scale
		r = rand() % 11;
		double scaleY = 0.5 + r / 10.0;
		r = rand() % 11;
		double scaleX = 0.5 + r / 10.0;
		particle.setScale(glm::vec3(scaleX, scaleY, 0.0));
	}

	// If the particle is a cloud
	else if (particle.getType() == "cloud")
	{
		// Choosing a texture out of the available cloud textures
		int r = rand() % 8 + 1;
		particle.setTextureNum(r);

		// Life is initialized to 5
		particle.setLife(5.0f);

		// Setting the cloud particle's position. Clouds will always start in the same place
		particle.setPosition(glm::vec3(-1.0f, 0.0, 0.0f));

		// Setting cloud brightness
		particle.setBrightness(0.6f);

		// Setting cloud color
		particle.setColor(glm::vec4(0.25, 0.25, 0.25, 0.0));

		// Setting cloud scale. Clouds will always be the same size
		particle.setScale(glm::vec3(2.0, 2.0, 0.0));
	}
}


