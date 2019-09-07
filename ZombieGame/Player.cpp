#include "Player.h"

#include <SDL/SDL.h>

#include "Bullet.h"
#include "Gun.h"

Player::Player() : _currentGun{ -1 } {
	/// Empty
}


Player::~Player() {
	/// Empty
}

void Player::init(float speed, glm::vec2 pos, Bengine::Color color, Bengine::InputManager* inputManager, Bengine::Camera2D* camera, std::vector<Bullet>* bullets) {
	_speed = speed;
	_position = pos;
	_color = color;
	_inputManager = inputManager;
	_camera = camera;
	_bullets = bullets;

	_health = 150;
}

void Player::update(const std::vector<std::string>& levelData,
	std::vector<Human*>& humans,
	std::vector<Zombie*>& zombies) {

	// detect moving 
	if (_inputManager->isKeyPressed(SDLK_w))
		_position.y += _speed;
	if (_inputManager->isKeyPressed(SDLK_s))
		_position.y -= _speed;
	if (_inputManager->isKeyPressed(SDLK_a))
		_position.x -= _speed;
	if (_inputManager->isKeyPressed(SDLK_d))
		_position.x += _speed;	

	// detect weapons switch
	if (_inputManager->isKeyPressed(SDLK_1) && _guns.size() >= 0)
		_currentGun = 0;
	if (_inputManager->isKeyPressed(SDLK_2) && _guns.size() >= 1)
		_currentGun = 1;
	if (_inputManager->isKeyPressed(SDLK_3) && _guns.size() >= 2)
		_currentGun = 2;

	
	if (_currentGun != -1 && _inputManager->isKeyPressed(SDL_BUTTON_LEFT)) {
		
		glm::vec2 mouseCoords = _inputManager->getMouseCoords();
		mouseCoords = _camera->getWorldCoordsFromScreenCoords(mouseCoords);

		glm::vec2 playerCenter = _position + glm::vec2(AGENT_RADIUS);
		glm::vec2 direction = glm::normalize(mouseCoords - playerCenter);

		_guns[_currentGun]->triggerFire(playerCenter, direction, *_bullets);
	}

	collideWithLevel(levelData);
}


void  Player::addGun(Gun* gun) {
	_guns.push_back(gun);

	/// if their is no gun, add gun
	if (_currentGun == -1)
		_currentGun = 0;
}