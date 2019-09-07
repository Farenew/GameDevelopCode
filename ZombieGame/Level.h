#pragma once

#include <string>
#include <vector>

#include <Bengine/SpriteBatch.h>


const int TILE_WIDTH = 64;
const float PLAYER_SPEED = 3.0f;
const float HUMAN_SPEED = 1.0f;
const float ZOMBIE_SPEED = 1.4f;

class Level
{
public:
	// Load the level
	Level(const std::string& fileName);
	~Level();

	void RenderLevelObject();

	// getters
	glm::vec2 getPlayerPos() const { return _playerPos; }
	const std::vector<glm::ivec2>& getZombiePos() const { return _zombiePos; }

	const std::vector<std::string>& getLevelData() { return _levelData; }

	int getNumHumans() const { return _numHumans; }

	int getWidth() const { return _levelData[0].size(); }
	int getHeight() const { return _levelData.size(); }

private:
	std::vector<std::string> _levelData;
	int _numHumans;

	Bengine::SpriteBatch _spriteBatch;

	glm::ivec2 _playerPos;
	std::vector<glm::ivec2> _zombiePos;
};

