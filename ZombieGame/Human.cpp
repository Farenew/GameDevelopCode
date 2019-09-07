#include "Human.h"
#include <random>
#include <ctime>

#include <glm/gtx/rotate_vector.hpp>

Human::Human() : _frames{ 0 } {
}

Human::~Human() {

}

static std::mt19937 randomEngine(time(nullptr));

void Human::init(float speed, glm::vec2 pos) {
	_speed = speed;
	_position = pos;

	static std::uniform_real_distribution<float> randDirection(-1.0f, 1.0f);

	_health = 20;

	_color.setColor(200, 0, 200, 255);

	// get random direction
	_direction = glm::vec2(randDirection(randomEngine), randDirection(randomEngine));
	if (_direction.length() == 0)
		_direction = glm::vec2(1.0f, 0.0f);

	_direction = glm::normalize(_direction);
}

void Human::update(const std::vector<std::string>& levelData,
	std::vector<Human*>& humans,
	std::vector<Zombie*>& zombies) {

	_position += _direction * _speed;

	static std::uniform_real_distribution<float> randRotate(-40.0f, 40.0f);

	if (_frames == 20) {
		_direction = glm::rotate(_direction, glm::radians(randRotate(randomEngine)));
		_frames = 0;
	}

	_frames++;
	

	if (collideWithLevel(levelData)) {
		_direction = glm::rotate(_direction, glm::radians(randRotate(randomEngine)));
	}
}