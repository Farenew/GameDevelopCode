#pragma once

#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "Bullet.h"

class Gun
{
public:
	Gun(std::string name, int fireRate, int bulletsPerShot, float spread, float bulletSpeed, float bulletDamage);
	~Gun();

	void triggerFire(const glm::vec2& pos, const glm::vec2& direction, std::vector<Bullet>& bullets);


private:

	void fire(const glm::vec2& pos, const glm::vec2& direction, std::vector<Bullet>& bullets);

	// fire rate in terms of frames
	int _fireRate;
	// accuracy
	float _spread;
	// how many bullets are shot at a time
	int _bulletPerShot;

	float _bulletSpeed;
	float _bulletDamage;
	
	std::string _gunName;

	int _frameCounter;

};

