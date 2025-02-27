#pragma once

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "presentationParameters.h"
#include "zviewport.h"

class GraphicsDevice
{
	public:
		typedef struct
		{
			int physicalWidth; // milimeters
			int physicalHeight; // milimeters
			std::string name;
			GLFWvidmode mode;
			bool isPrimary;
		} MonitorInfo;

		// Alpha | Blue | Green | Red
		enum class Color : uint32_t
		{
			Black = 0xff000000,
			Blue  = 0xffff0000,
			Lime  = 0xff00ff00,
			Red   = 0xff0000ff,
			
		};

	private:
		GLFWwindow* _window = nullptr;
		PresentationParameters& _pParams;

		int _monitorCount = -1;
		GLFWmonitor** _monitors = nullptr;
		std::vector<MonitorInfo> _monitorInfos;
		GLFWmonitor* _selectedMonitor = nullptr;

		GLuint _fixedFrameBuffer = 0;
		GLuint _fixedFrameBufferTexture = 0;
		std::map<const std::string, ZViewport> _viewports;

		std::string _errorInfo;
		//int _preferredWindowWidth = 800;
		//int _preferredWindowHeight = 600;
		//bool _isFullScreen = false;

		//std::string _windowTitle = "zGL";

		void _SetGLFWErrorMessage(const char* message);
		static void _FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
		void _ScaleViewports(int width, int height);

	public:
		GraphicsDevice(PresentationParameters& presentationParameters);
		~GraphicsDevice();

		std::string GetError();

		bool Initialize();
		void UpdatePresentationParameters();
		size_t GetMonitorCount() { return _monitorInfos.size(); }
		void PrintMonitorInfo(size_t index);

		GLFWwindow* GetWindow() { return _window; }

		void Clear();
		void Clear(glm::vec3 color);
		void Clear(glm::vec4 color);
};