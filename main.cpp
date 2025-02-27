#include <iostream>

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

	if (!device.Initialize())
	{
		std::cerr << device.GetError() << std::endl;
	}

	for (size_t i = 0; i < device.GetMonitorCount(); i++)
	{
		device.PrintMonitorInfo(i);
	}

	Timer timer;
	ContentManager contentManager;
	contentManager.SetRootDirectory(DEFAULT_CONTENT_DIRECTORY);
	std::shared_ptr<Shader> testShader = contentManager.LoadShader({ {"default.frag", ShaderType::Fragment},
																	 {"default.vert", ShaderType::Vertex} });
	std::shared_ptr<Texture2D> testTexture = contentManager.LoadTexture2D("default.png");
	Model testModel = contentManager.LoadModel("fileName.obj");

	//PrimitiveBuilder builder;

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