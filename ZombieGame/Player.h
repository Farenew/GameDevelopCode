#pragma once

#include "Human.h"
#include <Bengine/InputManager.h>
#include <Bengine/Camera2D.h>
#include "Gun.h"

class Gun;

class Player: public Human
{
public:
	Player();
	~Player();

	void init(float speed, glm::vec2 pos, Bengine::Color color, Bengine::InputManager* inputManager, Bengine::Camera2D* camera, std::vector<Bullet>* bullets);

	void update(const std::vector<std::string>& levelData,
		std::vector<Human*>& humans,
		std::vector<Zombie*>& zombies) override;

	void addGun(Gun* gun);

private:
	Bengine::InputManager* _inputManager;

	std::vector<Gun*> _guns;
	int _currentGun;

	Bengine::Camera2D* _camera;

	std::vector<Bullet>* _bullets;
};

