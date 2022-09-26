#include "BasicParticleGenerator.h"

void BasicParticleGenerator::respawn(BasicParticle& particle) const
{
	if (particle.getType() == "lightning")
	{
		int r = rand() % 4 + 1;
		particle.setTextureNum(r);
		// Life is initialized to 1
		particle.setLife(5.0f);

		// randomize things (position, color, texture, etc)
		r = rand() % 11;
		double posX = 2 * r / 10.0 - 1;
		r = rand() % 11;
		double posY = 2 * r / 10.0 - 1;
		particle.setPosition(glm::vec3(posX, posY, 0.0));//(particle.getPosition() + glm::vec3(0.0, 0.0, 0.0));

		particle.setBrightness(10.0f);

		particle.setColor(particle.getColor() + glm::vec4(0.025, 0.025, 1, 0));

		r = rand() % 1001;
		double d = r / 1000.0;
		r = rand() % 11;
		double scaleY = 0.5 + r / 10.0;
		r = rand() % 11;
		double scaleX = 0.5 + r / 10.0;
		particle.setScale(glm::vec3(scaleX, scaleY, 0.0));
		//cout << particle.getScale();

		//cout << "Respawning particle" << particle.getColor() << endl;
	}
	if (particle.getType() == "cloud")
	{
		int r = rand() % 8 + 1;
		particle.setTextureNum(r);
		particle.setLife(10.0f);
		r = rand() % 11;
		double posY = 2 * r / 10.0 - 1;
		particle.setColor(glm::vec4(0.25, 0.25, 0.25, 0.0));
		particle.setBrightness(0.6f);

		r = rand() % 4 + 1;
		double d = r * 0.75;

		particle.setScale(glm::vec3(d, d, 0.0));
		particle.setPosition(particle.getScale() * glm::vec3(-1.25f, posY, 0.0f));

	}
	//cout << particle.getPosition() << endl;
}


