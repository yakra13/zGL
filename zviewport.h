#pragma once

#include <glad/glad.h>

class ZViewport
{
	public:
		GLint X = 0;
		GLint Y = 0;
		GLsizei Width = 0;
		GLsizei Height = 0;

		ZViewport() : X(0), Y(0), Width(0), Height(0) {}
		
		ZViewport(GLint x, GLint y, GLsizei width, GLsizei height)
			: X(x), Y(y), Width(width), Height(height) {}

		void Use();
};