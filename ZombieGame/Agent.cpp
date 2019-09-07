#include "Agent.h"
#include "Level.h"

#include <Bengine/ResourceManager.h>
#include <algorithm>

#include <iostream>



Agent::Agent() : _speed{ 0.0 }, _position{ 0,0 } {

}

Agent::~Agent() {

}

void Agent::addToSpriteBatch(Bengine::SpriteBatch& spriteBatch) {
	glm::vec4 destRect;
	destRect.x = _position.x;
	destRect.y = _position.y;
	destRect.z = AGENT_WIDTH;
	destRect.w = AGENT_WIDTH;

	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

	static int textureID = Bengine::ResourceManager::getTexture("Textures/circle.png").id;

	spriteBatch.addGlyph(destRect, uvRect, textureID, 0.0f, _color);
}


bool Agent::applyDamage(float damage) {
	_health -= damage;
	if (_health <= 0.0f)
		return true;
	return false;
}

// origin collide function
//void Agent::collideWithLevel(const std::vector<std::string>& levelData) {
//	std::vector<glm::vec2> collideTilePosition;
//
//	checkTilePos(levelData, collideTilePosition, _position.x, _position.y);
//	checkTilePos(levelData, collideTilePosition, _position.x + AGENT_WIDTH, _position.y);
//	checkTilePos(levelData, collideTilePosition, _position.x, _position.y + AGENT_WIDTH);
//	checkTilePos(levelData, collideTilePosition, _position.x + AGENT_WIDTH, _position.y + AGENT_WIDTH);
//
//	for (int i = 0; i < collideTilePosition.size(); i++) {
//		collideWithTile(collideTilePosition[i]);
//	}
//}

bool Agent::collideWithLevel(const std::vector<std::string>& levelData) {
	std::vector<glm::vec2> collideTilePosition;

	/// if collide with left_bottom
	if (checkTilePos(levelData, collideTilePosition, _position.x, _position.y)) {
		// check left line
		if (checkTilePos(levelData, collideTilePosition, _position.x, _position.y + AGENT_WIDTH)) {
			for (int i = 0; i < collideTilePosition.size(); i++) {
				collideWithTile(collideTilePosition[i], 'Y');
			}
		}
		// check bottom line
		else if (checkTilePos(levelData, collideTilePosition, _position.x + AGENT_WIDTH, _position.y)) {
			for (int i = 0; i < collideTilePosition.size(); i++) {
				collideWithTile(collideTilePosition[i], 'X');
			}
		}
		// default collide
		else {
			for (int i = 0; i < collideTilePosition.size(); i++) {
				collideWithTile(collideTilePosition[i]);
			}
		}
	}
	/// if collide with right bottom
	if (checkTilePos(levelData, collideTilePosition, _position.x + AGENT_WIDTH, _position.y)) {
		// check right line
		if (checkTilePos(levelData, collideTilePosition, _position.x + AGENT_WIDTH, _position.y + AGENT_WIDTH)) {
			for (int i = 0; i < collideTilePosition.size(); i++) {
				collideWithTile(collideTilePosition[i], 'Y');
			}
		}
		// check bottom line
		else if (checkTilePos(levelData, collideTilePosition, _position.x, _position.y)) {
			for (int i = 0; i < collideTilePosition.size(); i++) {
				collideWithTile(collideTilePosition[i], 'X');
			}
		}
		// default collide
		else {
			for (int i = 0; i < collideTilePosition.size(); i++) {
				collideWithTile(collideTilePosition[i]);
			}
		}
	}
	/// if collide with left_top
	if (checkTilePos(levelData, collideTilePosition, _position.x, _position.y + AGENT_WIDTH)) {
		// check left line
		if (checkTilePos(levelData, collideTilePosition, _position.x, _position.y)) {
			for (int i = 0; i < collideTilePosition.size(); i++) {
				collideWithTile(collideTilePosition[i], 'Y');
			}
		}
		// check top line
		else if (checkTilePos(levelData, collideTilePosition, _position.x + AGENT_WIDTH, _position.y + AGENT_WIDTH)) {
			for (int i = 0; i < collideTilePosition.size(); i++) {
				collideWithTile(collideTilePosition[i], 'X');
			}
		}
		// default collide
		else {
			for (int i = 0; i < collideTilePosition.size(); i++) {
				collideWithTile(collideTilePosition[i]);
			}
		}
	}
	/// if collide with right_top
	if (checkTilePos(levelData, collideTilePosition, _position.x + AGENT_WIDTH, _position.y + AGENT_WIDTH)) {
		// check right line
		if (checkTilePos(levelData, collideTilePosition, _position.x + AGENT_WIDTH, _position.y)) {
			for (int i = 0; i < collideTilePosition.size(); i++) {
				collideWithTile(collideTilePosition[i], 'Y');
			}
		}
		// check top line
		else if (checkTilePos(levelData, collideTilePosition, _position.x, _position.y + AGENT_WIDTH)) {
			for (int i = 0; i < collideTilePosition.size(); i++) {
				collideWithTile(collideTilePosition[i], 'X');
			}
		}
		// default collide
		else {
			for (int i = 0; i < collideTilePosition.size(); i++) {
				collideWithTile(collideTilePosition[i]);
			}
		}
	}

	if (collideTilePosition.size() != 0)
		return true;
	else
		return false;
}

bool Agent::collideWithAgent(Agent* agent) {

	const float MIN_DISTANCE = AGENT_RADIUS * 2.0f;

	glm::vec2 centerPosA = _position + glm::vec2(AGENT_RADIUS);
	glm::vec2 centerPosB = agent->getPosition() + glm::vec2(AGENT_RADIUS);

	glm::vec2 distVec = centerPosA - centerPosB;

	float distance = glm::length(distVec);

	float collisionDepth = MIN_DISTANCE - distance;

	if (collisionDepth > 0) {

		glm::vec2 collisionDepthVec = glm::normalize(distVec) * collisionDepth;

		_position += collisionDepthVec / 2.0f;
		agent->_position -= collisionDepthVec / 2.0f;
		
		return true;
	}

	return false;
}

void Agent::collideWithTile(glm::vec2 tilePos, char type) {

	const float MIN_DISTANCE = AGENT_RADIUS + TILE_RADIUS;

	glm::vec2 centerPlayerPos = _position + glm::vec2(AGENT_RADIUS);
	glm::vec2 disVec = centerPlayerPos - tilePos;

	float xdepth = MIN_DISTANCE - abs(disVec.x);
	float ydepth = MIN_DISTANCE - abs(disVec.y);

	// when dealing with x-axis collide, change y value
	if (type == 'X' && xdepth > 0) {
		if (disVec.y < 0)
			_position.y -= ydepth;
		else
			_position.y += ydepth;
	}
	// when dealing with y-axis collide, change x value
	else if (type == 'Y' && ydepth > 0) {
		if (disVec.x < 0)
			_position.x -= xdepth;
		else
			_position.x += xdepth;
	}
	// default situation, change value based on their offset
	else {
		if (std::max(xdepth, 0.0f) < std::max(ydepth, 0.0f)) {
			if (disVec.x < 0)
				_position.x -= xdepth;
			else
				_position.x += xdepth;
		}
		else {
			if (disVec.y < 0)
				_position.y -= ydepth;
			else
				_position.y += ydepth;
		}
	}
}

bool Agent::checkTilePos(const std::vector<std::string>& levelData, std::vector<glm::vec2>& collideTilePosition, float x, float y) {
	// check the four corners
	glm::vec2 cornerPos = glm::vec2(floor(x / static_cast<float>(TILE_WIDTH)),
		floor(y / static_cast<float>(TILE_WIDTH)));

	if (levelData[cornerPos.y][cornerPos.x] != '.') {
		collideTilePosition.push_back(cornerPos * static_cast<float>(TILE_WIDTH) + glm::vec2(TILE_WIDTH / 2.0f, TILE_WIDTH / 2.0f));
		return true;
	}
	return false;
}

//void Agent::checkTilePos(const std::vector<std::string>& levelData, std::vector<glm::vec2>& collideTilePosition, float x, float y) {
//	// check the four corners
//	glm::vec2 cornerPos = glm::vec2(floor(x / static_cast<float>(TILE_WIDTH)),
//									floor(y / static_cast<float>(TILE_WIDTH)));
//
//	if (levelData[cornerPos.y][cornerPos.x] != '.') {
//		collideTilePosition.push_back(cornerPos * static_cast<float>(TILE_WIDTH) + glm::vec2(TILE_WIDTH / 2.0f, TILE_WIDTH / 2.0f));
//	}
//}

// AABB collision
//void Agent::collideWithTile(glm::vec2 tilePos) {
//
//	glm::vec2 centerPlayerPos = _position + glm::vec2(AGENT_RADIUS);
//	glm::vec2 disVec = centerPlayerPos - tilePos;
//
//	float xdepth = MIN_DISTANCE - abs(disVec.x);
//	float ydepth = MIN_DISTANCE - abs(disVec.y);
//
//	if (xdepth > 0 || ydepth > 0) {
//		if (std::max(xdepth, 0.0f) < std::max(ydepth, 0.0f)) {
//			if(disVec.x < 0)
//				_position.x -= xdepth;
//			else 
//				_position.x += xdepth;
//		}
//		else {
//			if (disVec.y < 0)
//				_position.y -= ydepth;
//			else
//				_position.y += ydepth;
//		}
//	}
//}