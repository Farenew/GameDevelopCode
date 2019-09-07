#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <string>

#include <Bengine/SpriteBatch.h>

class Agent;
class Human;
class Zombie;

const int BULLET_RAIDUS = 5;

class Bullet
{
public:
	Bullet(glm::vec2 pos, glm::vec2 direction, float damage, float speed);
	~Bullet();

	// when update returns true, delete bullet
	bool update(const std::vector<std::string>& levelData);

	void addToSpriteBatch(Bengine::SpriteBatch& spritebatch);

	bool collideWithAgent(Agent* agent);

	float getDamage() const { return _damage; }

private:

	bool collideWithWorld(const std::vector<std::string>& levelData);

	float _damage;
	glm::vec2 _position;
	glm::vec2 _direction;
	float _speed;
};

