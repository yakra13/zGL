#include <iostream>
#include <memory>
// #include <Libraries/include/glad/glad.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "contentManager.h"
#include "graphicsDevice.h"
#include "timer.h"
//#include "primitiveBuilder.h"

#define DEFAULT_CONTENT_DIRECTORY ""

int main()
{
	PresentationParameters pParams;
	
	GraphicsDevice device(pParams);

	std::cout << "Initializing graphics device." << std::endl;

	if (!device.Initialize())
	{
		std::cerr << device.GetError() << std::endl;
		//TODO: probably exit...
	}

	for (size_t i = 0; i < device.GetMonitorCount(); i++)
	{
		device.PrintMonitorInfo(i);
	}

	Timer timer;
	ContentManager contentManager;
	contentManager.SetRootDirectory(DEFAULT_CONTENT_DIRECTORY);
	std::cout << "load test shader" << std::endl;
	std::shared_ptr<Shader> testShader = contentManager.LoadShader({ {"default.frag", ShaderType::Fragment},
																	 {"default.vert", ShaderType::Vertex} });

	if (!testShader)
	{
		std::cerr << "Failed to load the test shader. Do the files exist?" << std::endl;
		return 1;
	}

	std::cout << "load test texture" << std::endl;
	std::shared_ptr<Texture2D> testTexture = contentManager.LoadTexture2D("default.png");
	std::cout << "load test model" << std::endl;
	Model testModel = contentManager.LoadModel("fileName.obj");

	//PrimitiveBuilder builder;

	std::cout << "get the window" << std::endl;

	//Mesh cube = builder.BuildCube();
	//Mesh plane = builder.BuildPlane();
	GLFWwindow* window = device.GetWindow();

	double deltaTime = 0.0;

	while (!glfwWindowShouldClose(window))
	{
		timer.Update();
		deltaTime = timer.GetElapsedTime();

		// Get input
		glfwPollEvents();

		// Updates
		testModel.Update(deltaTime);

		// Clear the frame
		device.Clear();

		// Draws
		testModel.Draw();

		// Reset shader
		glUseProgram(0);
		// Show the new frame
		glfwSwapBuffers(window);
	}

	return 0;
}