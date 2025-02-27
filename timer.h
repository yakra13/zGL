#pragma once
class Timer
{
	private:
		double _lastFrameTime = 0.0;
		double _currentFrameTime = 0.0;
		double _deltaTime = 0.0;
		double _elapsedSeconds = 0.0;

		double _fpsCounter = 0.0;
		int _frameCount = 0;
		int _fps = 0;
		float _averageFPS = 0.0f;
	public:
		Timer();
		~Timer();

		double GetElapsedTime() const { return _deltaTime; }
		double GetTotalTime() const { return _fpsCounter; }
		int GetFPS() const { return _fps; }
		// TODO: may need to handle roll over; go with unsigned size_t? or do the average over the last X seconds
		float GetAverageFPS() const { return _averageFPS / (int)_elapsedSeconds; }

		void Update();

};

