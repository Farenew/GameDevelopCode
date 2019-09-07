#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Bengine {

	class Camera2D
	{
	public:
		Camera2D();
		~Camera2D();

		void init(int screenWidth, int screenHeight);

		void update();

		glm::vec2 getWorldCoordsFromScreenCoords(glm::vec2 screenCoords);


		void setPosition(const glm::vec2& newPosition) { _position = newPosition; _needsMatrixUpdate = true; }
		glm::vec2 getPosition() const { return _position; }

		void setScale(float newScale) { _scale = newScale; _needsMatrixUpdate = true; }
		float getScale() const { return _scale; }

		glm::mat4 getCameraMatrix() const { return _cameraMatrix; }

	private:
		int _screenWidth, _screenHeight;
		bool _needsMatrixUpdate;
		float _scale;

		// float x, float y
		glm::vec2 _position;
		
		glm::mat4 _cameraMatrix;
		glm::mat4 _orthoMatrix;
	};

}