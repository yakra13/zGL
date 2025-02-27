#include "timer.h"
#include <GLFW/glfw3.h>

Timer::Timer()
{
}

Timer::~Timer()
{
}

void Timer::Update()
{
	_currentFrameTime = glfwGetTime();
	_deltaTime = _currentFrameTime - _lastFrameTime;
	
	_lastFrameTime = _currentFrameTime;

	_fpsCounter += _deltaTime;
	_elapsedSeconds += _deltaTime;

	_frameCount++;

	if (_fpsCounter >= 1.0)
	{
		_fps = _frameCount;
		_averageFPS += _fps;
		_frameCount = 0;
		_fpsCounter = 0.0;
	}
}
