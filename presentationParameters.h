#pragma once

#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>

enum class DepthTest
{
	Disabled,
	Enabled
};

enum class DepthFunction
{
	Never    = GL_NEVER,
	Less     = GL_LESS,
	Equal    = GL_EQUAL,
	LEqual   = GL_LEQUAL,
	Greater  = GL_GREATER,
	NotEqual = GL_NOTEQUAL,
	GEqual   = GL_GEQUAL,
	Always   = GL_ALWAYS
};

enum class CullFace
{
	None  = GL_NONE,
	Front = GL_FRONT,
	Back  = GL_BACK
};

enum class WindingOrder
{
	CCW = GL_CCW,
	CW  = GL_CW
};

class PresentationParameters
{
	private:
	public:
		// TODO
		int preferredWindowWidth  = 1920;
		int preferredWindowHeight = 1080;
		// Store actual window dimensions
		int windowWidth			  = preferredWindowWidth;
		int windowHeight		  = preferredWindowHeight;
		// The dimension to draw at regardless of window size
		int frameBufferWidth	  = preferredWindowWidth;
		int frameBufferHeight	  = preferredWindowHeight;

		bool isFullScreen = false;
		bool isWindowResizable = false;
		std::string windowTitle = "zGL";

		DepthTest depthTest	        = DepthTest::Enabled;
		DepthFunction depthFunction = DepthFunction::LEqual;
		CullFace cullFace			= CullFace::Back;
		WindingOrder windingOrder	= WindingOrder::CCW;

		glm::vec4 clearColor = glm::vec4(0.0f);

		PresentationParameters();
		~PresentationParameters();
};