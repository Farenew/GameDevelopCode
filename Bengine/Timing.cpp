#include "Timing.h"
#include <SDL\SDL.h>

namespace Bengine {


	FPSLimiter::FPSLimiter()
	{

	}
	void FPSLimiter::init(float maxFPS)
	{
		setMaxFPS(maxFPS);
	}

	void FPSLimiter::setMaxFPS(float maxFPS)
	{
		_maxFPS = maxFPS;
	}

	void FPSLimiter::beginFrame()
	{
		_startTicks = SDL_GetTicks();
	}

	// end will return the current FPS
	float FPSLimiter::endFrame()
	{
		calculateFPS();
		float frameTicks = SDL_GetTicks() - _startTicks;

		// limit fps to max fps
		if (1000.0f / _maxFPS > frameTicks) {
			SDL_Delay(1000.0f / _maxFPS - frameTicks);
		}
		return _fps;
	}
	void FPSLimiter::calculateFPS()
	{
		// number of frames to average
		static const int NUM_SAMPLES = 10;
		// store all the frameTimes for each frame that we will average
		static float frameTimes[NUM_SAMPLES];
		// the current frame we are on
		static int currentFrame = 0;

		// the ticks of the previous frame
		static float preTicks = SDL_GetTicks();

		static float currentTicks;

		currentTicks = SDL_GetTicks();

		// calculate the number of ticks for this frame
		_frameTime = currentTicks - preTicks;
		frameTimes[currentFrame % NUM_SAMPLES] = _frameTime;


		preTicks = currentTicks;

		int count;

		currentFrame++;

		if (currentFrame < NUM_SAMPLES) {
			count = currentFrame;
		}
		else {
			count = NUM_SAMPLES;
		}

		float frameTimeAverage = 0;
		for (int i = 0; i < count; i++) {
			frameTimeAverage += frameTimes[i];
		}
		frameTimeAverage /= count;
		if (frameTimeAverage > 0) {
			_fps = 1000.f / frameTimeAverage;
		}
		else {
			_fps = 60.0f;
		}
	}


}