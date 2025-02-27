#pragma once

#include <string>
#include <sstream>
#include <map>
#include <type_traits>

// TODO: content loader
#include <iostream>
#include <fstream>

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


// TODO: this is not all the types
enum class ShaderType
{
	Program  = 0,
	Fragment = GL_FRAGMENT_SHADER,
	Vertex   = GL_VERTEX_SHADER,
	Geometry = GL_GEOMETRY_SHADER,
	Compute  = GL_COMPUTE_SHADER
};

struct UniformInfo
{
	GLsizei length;
	GLint size;
	GLenum type;
	GLint location;
};

class Shader
{
	private:
		GLuint _id = 0;

		std::map<ShaderType, std::string> _shaderPaths;
		std::map<ShaderType, GLuint> _shaderIds;
		std::map<const std::string, UniformInfo> _uniformInfos;

		bool _GetCompilerErrors(GLuint shader, ShaderType type);
		void _GetShaderUniforms();
	public:
		Shader();
		~Shader();

		bool Use();
		void Delete();
		void SetShaderSource(std::string source, ShaderType type);
		bool Build();
		std::string GetShaderInfo();
		
		void SetUniform(const std::string& name, const int value);
		void SetUniform(const std::string& name, const float value);
		void SetUniform(const std::string& name, const glm::vec2& value);
		void SetUniform(const std::string& name, const glm::vec3& value);
		void SetUniform(const std::string& name, const glm::vec4& value);
		void SetUniform(const std::string& name, const glm::mat3& value);
		void SetUniform(const std::string& name, const glm::mat4& value);

		template<typename T>
		void SetUniformArray(const std::string& name, const T& values, size_t count);
};
