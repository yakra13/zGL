#pragma once

#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Texture2D
{
	enum class TextureType
	{
		Diffuse,
		Specular,
		Normal
	};

	enum class TextureFormat
	{
		RGB = GL_RGB,
		RGBA = GL_RGBA
	};

	private:
		GLuint _id = 0;
		GLuint _slot = 0;

		int _width = 0;
		int _height = 0;
		int _channels = 0;

		bool _mipmap = false;

		TextureFormat _format = TextureFormat::RGB;

	public:
		Texture2D();
		~Texture2D();

		bool LoadFromFile(std::string path);
		void GenerateMipMaps();
};