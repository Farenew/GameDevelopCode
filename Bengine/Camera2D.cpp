#include "Camera2D.h"

//#include <glm/gtx/string_cast.hpp>
//#include <iostream>
namespace Bengine {

	Camera2D::Camera2D() : 
		_position(0.0f, 0.0f),
		_cameraMatrix(1.0f),
		_scale(1.0f),
		_needsMatrixUpdate(true),
		_orthoMatrix(1.0f),
		_screenWidth(500),
		_screenHeight(500)
	{
	}


	Camera2D::~Camera2D()
	{
	}


	void Camera2D::init(int screenWidth, int screenHeight)
	{
		_screenWidth = screenWidth;
		_screenHeight = screenHeight;
		// glm::ortho would build up an ortho matrix
		_orthoMatrix = glm::ortho(0.0f, (float)_screenWidth, 0.0f, (float)_screenHeight);

		// here ortho matrix is build upon world cordinate, not unit matrix, which means we can use world cordinate to tranlate and get normalized value.
		// std::cout << glm::to_string(_orthoMatrix) << std::endl;
	}

	void Camera2D::update()
	{
		if (_needsMatrixUpdate) {

			// camera Translation, move viewpoint, vec3 records (x,y,z)
			glm::vec3 translate = glm::vec3(-_position.x + _screenWidth / 2, -_position.y + _screenHeight / 2, 0.0f);
			_cameraMatrix = glm::translate(_orthoMatrix, translate);

			// translate according to center, but seems useless?
			//translate = glm::vec3(-_position.x + _screenWidth / 2, _position.y - _screenHeight / 2, 0.0f);
			//_cameraMatrix = glm::translate(_orthoMatrix, translate);

			// camera scale, scale is (x,y,z)
			glm::vec3 scale(_scale, _scale, 0.0f);
			_cameraMatrix = glm::scale(glm::mat4(1.0f), scale) * _cameraMatrix;

			_needsMatrixUpdate = false;
		}
	}

	glm::vec2 Camera2D::getWorldCoordsFromScreenCoords(glm::vec2 screenCoords)
	{
		// Invert Y direction
		screenCoords.y = _screenHeight - screenCoords.y;
		// let left most side be negative, make center 0,0
		screenCoords -= glm::vec2(_screenWidth / 2, _screenHeight / 2);
		// scale the coords
		screenCoords /= _scale;
		// translate with the camera positoin
		screenCoords += _position;
		return screenCoords;
	}

}