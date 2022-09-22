#include "BasicParticleGenerator.h"

void BasicParticleGenerator::respawn(BasicParticle& particle) const
{
	// Life is initialized to 1
	particle.setLife(1.0f);

	// randomize things (position, color, texture, etc)
	particle.setPosition(glm::vec3(0.0, 0.0, 0.0));

	//particle.setBrightness(10.0f);

	//cout << "Respawning particle" << endl;
}
