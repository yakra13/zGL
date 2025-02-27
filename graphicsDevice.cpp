#include "graphicsDevice.h"

GraphicsDevice::GraphicsDevice(PresentationParameters& presentationParameters)
	: _pParams(presentationParameters)
{
	UpdatePresentationParameters();
}

GraphicsDevice::~GraphicsDevice()
{
	glfwTerminate();
}

bool GraphicsDevice::Initialize()
{
	// Initialize glfw libraries
	if (glfwInit() == GLFW_FALSE)
	{
		_SetGLFWErrorMessage("Error: Unable to initialize GLFW.");

		return false;
	}

	// Hints to glfwCreateWindow on the version of openGL to use
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, _pParams.isWindowResizable);

	// Enable transparency in the frame buffer
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);

	// Collect monitor information
	_monitors = glfwGetMonitors(&_monitorCount);

	if (_monitorCount < 1)
	{
		_errorInfo = "You seem to not have a monitor?";
		glfwTerminate();
		return false;
	}

	_selectedMonitor = glfwGetPrimaryMonitor();

	for (int i = 0; i < _monitorCount; i++)
	{
		MonitorInfo mi = { 0 };
		glfwGetMonitorPhysicalSize(_monitors[i], &mi.physicalWidth, &mi.physicalHeight);
		mi.name = glfwGetMonitorName(_monitors[i]);
		mi.mode = *glfwGetVideoMode(_monitors[i]);
		mi.isPrimary = _selectedMonitor == _monitors[i] ? true : false;

		_monitorInfos.emplace_back(mi);
	}

	// Create the window
	_window = glfwCreateWindow(_pParams.preferredWindowWidth,
							   _pParams.preferredWindowHeight,
							   _pParams.windowTitle.c_str(),
							   _pParams.isFullScreen ? _selectedMonitor : NULL,
							   NULL);
	if (_window == NULL)
	{
		glfwTerminate();

		_SetGLFWErrorMessage("Error: Unable to create window.");

		return false;
	}

	// Window successfully created; set it to the current context
	glfwMakeContextCurrent(_window);

	// Initialize glad library
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	// Gather window and framebuffer information
	glfwGetWindowSize(_window, &_pParams.windowWidth, &_pParams.windowHeight);
	glfwGetFramebufferSize(_window, &_pParams.frameBufferWidth, &_pParams.frameBufferHeight);

	// Enable opengl blending for transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Create fixed frame buffer
	glGenFramebuffers(1, &_fixedFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, _fixedFrameBuffer);

	// Setup render texture for the fixed frame buffer
	glGenTextures(1, &_fixedFrameBufferTexture);
	glBindTexture(GL_TEXTURE_2D, _fixedFrameBufferTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _pParams.frameBufferWidth, _pParams.frameBufferHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	// Attach texure to the frame buffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _fixedFrameBufferTexture, 0);

	// Set default viewport to be the entire framebuffer size
	_viewports["default"] = ZViewport(0, 0, (GLsizei)_pParams.frameBufferWidth, (GLsizei)_pParams.frameBufferHeight);
	_viewports["default"].Use();

	// TODO: drawing code
	//// Bind the source framebuffer (FBO)
	//glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
	// Do drawing of stuff

	//// Bind the target framebuffer (default framebuffer)
	//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	//// Specify source and destination dimensions
	//glBlitFramebuffer(
	//	0, 0, 1920, 1080,          // Source FBO dimensions
	//	0, 0, windowWidth, windowHeight,  // Target window dimensions
	//	GL_COLOR_BUFFER_BIT,       // Copy the color buffer
	//	GL_LINEAR                  // Scaling filter
	//);
	

	// Set glad parameters
	if (_pParams.depthTest == DepthTest::Enabled)
	{
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(static_cast<int>(_pParams.depthFunction));
	}
	else
	{
		glDisable(GL_DEPTH_TEST);
	}

	if (_pParams.cullFace == CullFace::None)
	{
		glDisable(GL_CULL_FACE);
	}
	else
	{
		glEnable(GL_CULL_FACE);
		glCullFace(static_cast<int>(_pParams.cullFace));
		glFrontFace(static_cast<int>(_pParams.windingOrder));
	}


	glClearColor(_pParams.clearColor.r, _pParams.clearColor.g, _pParams.clearColor.b, _pParams.clearColor.a);
	// TODO: Add these settings to presentation parameters
	glClearStencil(0); // clear stencil buffer
	glClearDepth(1.0f); // 0 near, 1 far
	// glEnable(GL_BLEND);
	// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

	// Effectively enables vsync
	// TODO: some code to enable disable this tho milage may vary see glfw.org
	glfwSwapInterval(1);

	glfwSetWindowUserPointer(_window, this);
	// TODO: callbacks; likely handle elsewhere or provide wrappers for end user
	//glfwSetMonitorCallback(); // handles connecting and disconnecting monitors during execution
	//glfwSetWindowSizeCallback(window, window_size_callback); // 
	// void window_size_callback(GLFWwindow* window, int width, int height) // w/h of content area
	//glfwGetWindowFrameSize(window, &left, &top, &right, &bottom); // whole window with title etc
	glfwSetFramebufferSizeCallback(_window, _FrameBufferSizeCallback);
	//glfwSetKeyCallback(window, keyCallback);
	//glfwSetMouseButtonCallback(window, mouseButtonCallback);
	//glfwSetCursorPosCallback(window, cursorPosCallback);
	//glfwSetScrollCallback(window, mouseScrollCallback);
	//glfwSetErrorCallback(errorCallback);

	return true;
}

void GraphicsDevice::UpdatePresentationParameters()
{
	// TODO: update presentation parameters that have changed
	// window title, windowWidth/height, etc.
	// some changes will require recreating the window
}

std::string GraphicsDevice::GetError()
{
	return _errorInfo;
}


void GraphicsDevice::_SetGLFWErrorMessage(const char* message)
{
	// Clear any old error strings
	_errorInfo.clear();

	_errorInfo += message + '\n';

	const char* description;

	glfwGetError(&description);

	if (description)
	{
		_errorInfo += description;
	}
	else
	{
		_errorInfo += "Unable to retrieve GLFW error description.";
	}
}
void GraphicsDevice::_ScaleViewports(int width, int height)
{

}

void GraphicsDevice::_FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	GraphicsDevice* device = static_cast<GraphicsDevice*>(glfwGetWindowUserPointer(window));
	if (device)
	{
		// TODO: scale the viewports
		device->_ScaleViewports(width, height);
	}
}

void GraphicsDevice::PrintMonitorInfo(size_t index)
{
	std::cout << "Name: " << _monitorInfos[index].name << std::endl;
	std::cout << "Primary: " << _monitorInfos[index].isPrimary << std::endl;
	std::cout << "Physical Dimensions: " << _monitorInfos[index].physicalWidth << "mm x " << _monitorInfos[index].physicalHeight << "mm" << std::endl;
	std::cout << "Resolution: " << _monitorInfos[index].mode.width << "x" << _monitorInfos[index].mode.height << std::endl;
	std::cout << "Refresh Rate: " << _monitorInfos[index].mode.refreshRate << "Hz" << std::endl;
	std::cout << "Color Bits: " << _monitorInfos[index].mode.redBits << "/" << _monitorInfos[index].mode.greenBits << "/" << _monitorInfos[index].mode.blueBits << std::endl;
	std::cout << std::endl;
}

void GraphicsDevice::Clear()
{
	// TODO: include only the BITs that are actually set in this device
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void GraphicsDevice::Clear(glm::vec3 color)
{
	glClearColor(color.r, color.g, color.b, 1.0f);
	Clear();
	// Reset to params clear color
	glClearColor(_pParams.clearColor.r, _pParams.clearColor.g, _pParams.clearColor.b, _pParams.clearColor.a);
}

void GraphicsDevice::Clear(glm::vec4 color)
{
	glClearColor(color.r, color.g, color.b, color.a);
	Clear();
	// Reset to params clear color
	glClearColor(_pParams.clearColor.r, _pParams.clearColor.g, _pParams.clearColor.b, _pParams.clearColor.a);
}