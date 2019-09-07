#pragma once

#include <glm/glm.hpp>
#include <Bengine/SpriteBatch.h>

#include "Level.h"

const float AGENT_WIDTH = TILE_WIDTH * 0.8f;

const float AGENT_RADIUS = AGENT_WIDTH / 2.0f;
const float TILE_RADIUS = TILE_WIDTH / 2.0f;

// using declaraction, so we don't need to include, in case cause circular include
class Zombie;

class Human;

class Agent
{

public:
	Agent();
	virtual ~Agent();

	virtual void update(const std::vector<std::string>& levelData,
						std::vector<Human*>& humans,
						std::vector<Zombie*>& zombies) = 0;

	bool collideWithLevel(const std::vector<std::string>& levelData);

	bool collideWithAgent(Agent* agent);

	void addToSpriteBatch(Bengine::SpriteBatch& spriteBatch);

	glm::vec2 getPosition() const { return _position; }

	// return true if agent dies
	bool applyDamage(float damage);


protected:

	void collideWithTile(glm::vec2 tilePos, char type = ' ');
	bool checkTilePos(const std::vector<std::string>& levelData, std::vector<glm::vec2>& collideTilePosition, float x, float y);

	// these two functions are origin functions
	//void checkTilePos(const std::vector<std::string>& levelData, std::vector<glm::vec2>& collideTilePosition, float x, float y);
	//void collideWithTile(glm::vec2 tilePos);

	glm::vec2 _position;
	float _speed;

	Bengine::Color _color;

	float _health;
};

