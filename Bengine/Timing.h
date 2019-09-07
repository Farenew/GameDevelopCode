#pragma once

namespace Bengine {

	// using this we want to do 3 things, getTicks, calculateFPS, and limit FPS
	class FPSLimiter {
	public:
		FPSLimiter();
		void init(float maxFPS);

		void setMaxFPS(float maxFPS);
		void beginFrame();

		// end will return the current FPS
		float endFrame();

	private:
		void calculateFPS();

		float _fps;
		float _maxFPS;
		float _frameTime;
		unsigned int _startTicks;

	};

}