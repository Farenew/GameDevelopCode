#include "InputManager.h"

namespace Bengine {

	InputManager::InputManager() : _mouseCoords{ 0,0 }
	{

	}


	InputManager::~InputManager()
	{
	}

	void InputManager::pressKey(unsigned int keyID)
	{
		// you can just treat unordered map as array
		// if it's not in the map, then this is going to create it
		_keyMap[keyID] = true;
	}

	void InputManager::releaseKey(unsigned int keyID)
	{
		_keyMap[keyID] = false;
	}

	bool InputManager::isKeyPressed(unsigned int keyID)
	{
		if (_keyMap.count(keyID))
			return _keyMap[keyID];
		else
			return false;
	}

	void InputManager::setMouseCoords(float x, float y) {
		_mouseCoords.x = x;
		_mouseCoords.y = y;
	}
}