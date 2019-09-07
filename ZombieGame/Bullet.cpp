#include "Bullet.h"

#include <Bengine/ResourceManager.h>

#include "Agent.h"
#include "Human.h"
#include "Zombie.h"
#include "Level.h"

Bullet::Bullet(glm::vec2 pos, glm::vec2 direction, float damage, float speed):
	_position{pos},
	_direction{direction},
	_damage{damage},
	_speed{speed}
{

}

Bullet::~Bullet() {

}

bool Bullet::update(const std::vector<std::string>& levelData) {
	_position += _direction * _speed;

	return collideWithWorld(levelData);
}

void Bullet::addToSpriteBatch(Bengine::SpriteBatch& spritebatch) {
	glm::vec4 destRect(_position.x + BULLET_RAIDUS, _position.y + BULLET_RAIDUS, BULLET_RAIDUS * 2, BULLET_RAIDUS * 2);
	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

	Bengine::Color color(75, 80, 128, 255);

	spritebatch.addGlyph(destRect, uvRect, Bengine::ResourceManager::getTexture("Textures/circle.png").id, 0.0f, color);
}

bool Bullet::collideWithAgent(Agent* agent) {
	const float MIN_DISTANCE = AGENT_RADIUS + BULLET_RAIDUS;

	glm::vec2 centerPosA = _position;
	glm::vec2 centerPosB = agent->getPosition() + glm::vec2(AGENT_RADIUS);

	glm::vec2 distVec = centerPosA - centerPosB;

	float distance = glm::length(distVec);

	float collisionDepth = MIN_DISTANCE - distance;

	if (collisionDepth > 0) {
		return true;
	}

	return false;
}

bool Bullet::collideWithWorld(const std::vector<std::string>& levelData) {
	glm::ivec2 gridPos;
	gridPos.x = floor(_position.x / static_cast<float>(TILE_WIDTH));
	gridPos.y = floor(_position.y / static_cast<float>(TILE_WIDTH));

	if (gridPos.x < 0 || gridPos.x >= levelData[0].size() || gridPos.y < 0 || gridPos.y >= levelData.size()) {
		return true;
	}

	return levelData[gridPos.y][gridPos.x] != '.';
}