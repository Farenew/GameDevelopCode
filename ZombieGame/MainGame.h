#pragma once

#include <Bengine\Window.h>
#include <Bengine\GLSLProgram.h>
#include <Bengine\Camera2D.h>
#include <Bengine\InputManager.h>
#include <Bengine\SpriteBatch.h>

#include "Level.h"
#include "Player.h"
#include "Zombie.h"
#include "Bullet.h"

enum class GameState {
	PLAY,
	EXIT
};


class MainGame
{
public:
    MainGame();
    ~MainGame();

    /// Runs the game
    void run();

private:
    /// Initializes the core systems
    void initSystems();
    /// Initializes the shaders
    void initShaders();

	void initLevel();

    /// Main game loop for the program
    void gameLoop();
    /// Handles input processing
    void processInput();
    /// Renders the game
    void drawGame();

	/// update all agents
	void updateAgents();
	/// update bullets
	void updateBullets();

	/// check if we should exit game
	void checkVictory();


private:
    Bengine::Window _window; ///< The game window
	Bengine::GLSLprogram _textureProgram; ///< The shader program
	Bengine::InputManager _inputManager; ///< Handles input
    Bengine::Camera2D _camera; ///< Main Camera

	Bengine::SpriteBatch _agentSpriteBatch;

	GameState _gameState;

	std::vector<Level*> _levels;
	int _curLevel;

	std::vector<Human*> _humans;
	std::vector<Zombie*> _zombies;
	Player* _player;

	int _screenWidth, _screenHeight;
	float _fps;

	std::vector<Bullet> _bullets;

	int _numHumanKilledByPlayer;
	int _numZombiesKilledByPlayer;

	int _numHumanKilledByZombie;
};

