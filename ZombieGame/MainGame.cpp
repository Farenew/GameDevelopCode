#include "MainGame.h"
#include <SDL/SDL.h>

#include <iostream>
#include <string>
#include <random>
#include <ctime>

#include <Bengine/Bengine.h>
#include <Bengine/Timing.h>
#include <Bengine/errors.h>

MainGame::MainGame() :
	_screenHeight{ 768 },
	_screenWidth{ 1024 },
	_gameState{ GameState::PLAY },
	_fps{ 0.0 },
	_curLevel{ 0 },
	_player(nullptr),
	_numHumanKilledByPlayer{0},
	_numZombiesKilledByPlayer{0},
	_numHumanKilledByZombie{0}
{
 
}

MainGame::~MainGame() 
{
	for (int i = 0; i < _levels.size(); i++)
		delete _levels[i];

	for (int i = 0; i < _humans.size(); i++)
		delete _humans[i];

	for (int i = 0; i < _zombies.size(); i++) {
		delete _zombies[i];
	}
}

void MainGame::run() {
	initSystems();
	initLevel();
	gameLoop();
}

void MainGame::initSystems() {
	Bengine::init();

	_window.create("ZombieGame", _screenWidth, _screenHeight, 0);
	glClearColor(0.6f, 0.6f, 0.6f, 0.1f);

	initShaders();

	_camera.init(_screenWidth, _screenHeight);

	_agentSpriteBatch.init();
}

void MainGame::initLevel() {
	char level = _curLevel + 1 + '0';
	std::string filePath = "Levels/level" + std::string{ level } + ".txt";
	_levels.push_back(new Level(filePath));

	_player = new Player();
	Bengine::Color PlayerColor(0,0,180,255);

	_player->init(PLAYER_SPEED, _levels[_curLevel]->getPlayerPos(), PlayerColor, &_inputManager, &_camera, &_bullets);

	_humans.push_back(_player);

	// random generate to generate humans
	std::mt19937 rand;
	rand.seed(time(nullptr));
	// here distribution not starting from 0 or 1 since we want to avoid generate human at the border, which may cause detection out of vector subscript limit
	std::uniform_real_distribution<float> randX(3, _levels[_curLevel]->getWidth() - 3);
	std::uniform_real_distribution<float> randY(3, _levels[_curLevel]->getHeight() - 3);

	// add all random humans
	for (int i = 0; i < _levels[_curLevel]->getNumHumans(); i++) {
		_humans.push_back(new Human);
		glm::vec2 pos(randX(rand) * TILE_WIDTH, randY(rand) * TILE_WIDTH);
		_humans.back()->init(HUMAN_SPEED, pos);
	}

	// add all zombies
	const std::vector<glm::ivec2>& zombiePositions = _levels[_curLevel]->getZombiePos();
	for (int i = 0; i < zombiePositions.size(); i++) {
		_zombies.push_back(new Zombie);
		_zombies.back()->init(ZOMBIE_SPEED, zombiePositions[i]);
	}

	_player->addGun(new Gun("Magnum", 30, 1, 10.0f, 30, 20));
	_player->addGun(new Gun("ShotGun", 60, 10, 40.0f, 30, 4));
	_player->addGun(new Gun("MP5", 5, 1, 20.0f, 30, 40));
}


void MainGame::initShaders() {
    // Compile our color shader
	_textureProgram.compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
	_textureProgram.addAttribute("vertexPosition");
	_textureProgram.addAttribute("vertexColor");
	_textureProgram.addAttribute("vertexUV");
	_textureProgram.linkShaders();
}

void MainGame::gameLoop() {

	Bengine::FPSLimiter fpsLimiter;
	fpsLimiter.setMaxFPS(60.0f);

	const float CAMERA_SCALE = 0.5f;
	_camera.setScale(CAMERA_SCALE);

	while (_gameState == GameState::PLAY) {
		fpsLimiter.beginFrame();

		checkVictory();

		processInput();

		updateBullets();
		updateAgents();

		_camera.setPosition(_player->getPosition());
		_camera.update();

		drawGame();

		_fps = fpsLimiter.endFrame();
	}
}

void MainGame::updateAgents() {

	// update all humans 
	for (int i = 0; i < _humans.size(); i++) {
		_humans[i]->update(_levels[_curLevel]->getLevelData(), _humans, _zombies);
	}

	// update all zombies 
	for (int i = 0; i < _zombies.size(); i++) {
		_zombies[i]->update(_levels[_curLevel]->getLevelData(), _humans, _zombies);
	}

	// update zombies collisions
	for (int i = 0; i < _zombies.size(); i++) {
		// collide with zombies
		for (int j = i + 1; j < _zombies.size(); j++) {
			_zombies[i]->collideWithAgent(_zombies[j]);
		}
		// collide with humans, since player is the first human, we don't want to collide with player
		for (int j = 1; j < _humans.size(); j++) {
			if (_zombies[i]->collideWithAgent(_humans[j])) {
				// add a new zombie
				_zombies.push_back(new Zombie);
				_zombies.back()->init(ZOMBIE_SPEED, _humans[j]->getPosition());

				// delete a human
				delete _humans[j];
				_humans.erase(_humans.begin() + j, _humans.begin() + j + 1);

				_numHumanKilledByZombie++;
			}
		}

		if (_zombies[i]->collideWithAgent(_player)) {
			fatalError("YOU LOSE");
		}
	}

	// update human collisions
	for (int i = 0; i < _humans.size(); i++) {
		// collide with humans
		for (int j = i+1; j < _humans.size(); j++) {
			_humans[i]->collideWithAgent(_humans[j]);
		}
	}
}

void MainGame::updateBullets() {
	/// collide with the world
	for (int i = 0; i < _bullets.size();i++) {
		if (_bullets[i].update(_levels[_curLevel]->getLevelData())) {
			_bullets[i] = _bullets.back();
			_bullets.pop_back();
			i--;
		}
	}

	
	/// collide with agents
	for (int i = 0; i < _bullets.size(); i++) {
		bool isBulletRemoved = false;

		// check zombies collision
		for (int j = 0; j < _zombies.size(); j++) {
			if (_bullets[i].collideWithAgent(_zombies[j])) {
				// damage zombie
				if (_zombies[j]->applyDamage(_bullets[i].getDamage())) {
					delete _zombies[j];
					_zombies[j] = _zombies.back();
					_zombies.pop_back();

					_numZombiesKilledByPlayer++;
				}

				/// remove bullets
				_bullets[i] = _bullets.back();
				_bullets.pop_back();
				// make sure bullets won't be skipped
				i--;
				isBulletRemoved = true;
				break;
			}
		}

		if (isBulletRemoved)
			continue;

		// check human collision
		for (int j = 1; j < _humans.size(); j++) {
			if (_bullets[i].collideWithAgent(_humans[j])) {
				// damage human
				if (_humans[j]->applyDamage(_bullets[i].getDamage())) {
					delete _humans[j];
					_humans[j] = _humans.back();
					_humans.pop_back();

					_numHumanKilledByPlayer++;
				}

				/// remove bullets
				_bullets[i] = _bullets.back();
				_bullets.pop_back();
				// make sure bullets won't be skipped
				i--;
				break;
			}
		}

	}
}

void  MainGame::checkVictory() {
	// TODO: change level

	using namespace std;
	if (_zombies.empty()) {
		printf("*** YOU Win ! ***\n");
		printf("You killed %d Zombies, %d humans got killed by Zombies, and you killed %d humans\n", 
			_numZombiesKilledByPlayer, _numHumanKilledByZombie, _numHumanKilledByPlayer);

		exit(1);
	}
}

void MainGame::processInput() {
    SDL_Event evnt;

    //Will keep looping until there are no more events to process
    while (SDL_PollEvent(&evnt)) {
        switch (evnt.type) {
            case SDL_QUIT:
				_gameState = GameState::EXIT;
                break;
            case SDL_MOUSEMOTION:
                _inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
                break;
            case SDL_KEYDOWN:
				_inputManager.pressKey(evnt.key.keysym.sym);
                break;
            case SDL_KEYUP:
				_inputManager.releaseKey(evnt.key.keysym.sym);
                break;
            case SDL_MOUSEBUTTONDOWN:
				_inputManager.pressKey(evnt.button.button);
                break;
            case SDL_MOUSEBUTTONUP:
				_inputManager.releaseKey(evnt.button.button);
                break;
        }
	}
}

void MainGame::drawGame() {
    // Set the base depth to 1.0
    glClearDepth(1.0);
    // Clear the color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_textureProgram.use();

	//Draw code goes here
	// active Texture 0 memory
	glActiveTexture(GL_TEXTURE0);
	// set and upload texture data, only GLuint identifier is enough
	GLuint textureuniform = _textureProgram.getUniformLocation("mySampler");
	glUniform1i(textureuniform, 0);

	// set and upload camera data
	glm::mat4 projectrionMatrix = _camera.getCameraMatrix();
	GLint pUniform = _textureProgram.getUniformLocation("transformationMatrix");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectrionMatrix[0][0]);

	// level has its own spriteBatch, used to draw all the walls, etc. 
	_levels[_curLevel]->RenderLevelObject();

	// add agents to sprites
	_agentSpriteBatch.bufferClear();
	for (int i = 0; i < _humans.size(); i++) {
		_humans[i]->addToSpriteBatch(_agentSpriteBatch);
	}

	for (int i = 0; i < _zombies.size(); i++) {
		_zombies[i]->addToSpriteBatch(_agentSpriteBatch);
	}

	for (int i = 0; i < _bullets.size(); i++) {
		_bullets[i].addToSpriteBatch(_agentSpriteBatch);
	}

	_agentSpriteBatch.bufferUpdate();

	_agentSpriteBatch.renderBatch();

	_textureProgram.unuse();

	//Swap our buffer and draw everything to the screen!
	_window.swapBuffer();
}