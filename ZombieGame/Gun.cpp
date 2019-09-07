#include "Gun.h"

#include <random>
#include <ctime>
#include <glm/gtx/rotate_vector.hpp>

Gun::Gun(std::string name, int fireRate, int bulletsPerShot, float spread, float bulletSpeed, float bulletDamage) :
	_gunName{ name },
	_fireRate { fireRate },
	_bulletPerShot{bulletsPerShot},
	_spread{spread},
	_bulletSpeed {bulletSpeed},
	_bulletDamage {bulletDamage},
	_frameCounter{0}
{

}

Gun::~Gun() {

}



void Gun::triggerFire(const glm::vec2& pos, const glm::vec2& direction, std::vector<Bullet>& bullets) {
	_frameCounter++;
	if (_frameCounter >= _fireRate) {
		fire(pos, direction, bullets);
		_frameCounter = 0;
	}
}

void Gun::fire(const glm::vec2& pos, const glm::vec2& direction, std::vector<Bullet>& bullets) {

	std::mt19937 randomEngine;
	randomEngine.seed(time(nullptr));
	std::uniform_real_distribution<float> randRotate(-_spread, _spread);

	for (int i = 0; i < _bulletPerShot; i++) {
		bullets.emplace_back(pos, glm::rotate(direction, glm::radians(randRotate(randomEngine))), _bulletDamage, _bulletSpeed);
	}
}