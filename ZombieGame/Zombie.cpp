#include "Zombie.h"

#include <limits>
#include <random>
#include <ctime>

#include "Human.h"

Zombie::Zombie() {
	
}


Zombie::~Zombie() {

}

void Zombie::init(float speed, glm::vec2 pos) {
	_speed = speed;
	_position = pos;

	_color.setColor(0, 160, 0, 255);

	_health = 150;


}

void Zombie::update(const std::vector<std::string>& levelData,
	std::vector<Human*>& humans,
	std::vector<Zombie*>& zombies) {

	Human* closeestHuman = getNearestHuman(humans);

	if (closeestHuman != nullptr) {
		glm::vec2 direction = glm::normalize(closeestHuman->getPosition() - _position);
		_position += direction * _speed;
	}

	collideWithLevel(levelData);
}

Human* Zombie::getNearestHuman(std::vector<Human*>& humans) {
	Human* closestHuman = nullptr;
	float smallestDis = std::numeric_limits<float>::max();

	for (int i = 0; i < humans.size(); i++) {
		glm::vec2 distVec = humans[i]->getPosition();

		float distance = glm::length(distVec);

		if (distance < smallestDis) {
			smallestDis = distance;
			closestHuman = humans[i];
		}
	}

	return closestHuman;
}