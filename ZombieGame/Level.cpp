#include "Level.h"

#include <fstream>
#include <Bengine/errors.h>
#include <Bengine/ResourceManager.h>

#include <iostream>


Level::Level(const std::string& fileName) {

	std::ifstream file;
	file.open(fileName);

	if (file.fail()) 
		fatalError("Failed to open file: " + fileName);

	std::string tmp;
	file >> tmp >> _numHumans;

	// discard first line, we don't need to push_back this line
	std::getline(file, tmp);
	while (std::getline(file, tmp)) {
		_levelData.push_back(tmp);
	}

	file.close();

	_spriteBatch.init();
	_spriteBatch.bufferClear();


	// add texture data
	for (int y = 0; y < _levelData.size(); y++) {
		for (int x = 0; x < _levelData[y].size(); x++) {
			char tile = _levelData[y][x];

			glm::vec4 destRect(x * TILE_WIDTH, y * TILE_WIDTH, TILE_WIDTH, TILE_WIDTH);
			glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

			switch (tile)
			{
			case 'R':
				_spriteBatch.addGlyph(destRect,
					uvRect,
					Bengine::ResourceManager::getTexture("Textures/red_bricks.png").id,
					0.0f,
					Bengine::Color());
				break;
			case 'G':
				_spriteBatch.addGlyph(destRect,
					uvRect,
					Bengine::ResourceManager::getTexture("Textures/glass.png").id,
					0.0f,
					Bengine::Color());
				break;
			case 'L':
				_spriteBatch.addGlyph(destRect,
					uvRect,
					Bengine::ResourceManager::getTexture("Textures/light_bricks.png").id,
					0.0f,
					Bengine::Color());
				break;
			case '@':
				_levelData[y][x] = '.';	 // so we don't collide
				_playerPos.x = x * TILE_WIDTH;
				_playerPos.y = y * TILE_WIDTH;
				break;
			case 'Z':
				_levelData[y][x] = '.';
				_zombiePos.emplace_back(x * TILE_WIDTH, y * TILE_WIDTH);
				break;
			case '.':
				break;
			default:
				std::cout << "Unexpected tile " << tile << " at: " << x << ", " << y << std::endl;
				break;
			}
		}
	}
	_spriteBatch.bufferUpdate();
}


Level::~Level() {

}

void Level::RenderLevelObject() {

	_spriteBatch.renderBatch();

}