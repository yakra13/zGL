#include "shader.h"

std::string getFileContents(std::string fileName)
{
	std::ifstream in(fileName, std::ios::binary);
	if (!in)
	{
		throw std::ios_base::failure("Error: Unable to open file " + fileName);
	}
	
	std::string contents;
	
	in.seekg(0, std::ios::end);
	contents.resize(in.tellg());
	in.seekg(0, std::ios::beg);
	in.read(&contents[0], contents.size());
	in.close();

	return (contents);
}

Shader::Shader()
{
}

Shader::~Shader()
{
	glDeleteProgram(_id);
}

bool Shader::Use()
{
	// TODO: 
	if (_id == 0)
	{
		return false;
	}
	
	glUseProgram(_id);

	return true;
}

void Shader::Delete()
{
}

void Shader::SetShaderSource(std::string source, ShaderType type)
{
	_shaderPaths[type] = source;
}

bool Shader::Build()
{
	if (_id != 0)
	{
		return false;
	}
	
	GLuint tempId = 0;

	for (const auto& [type, path] : _shaderPaths)
	{
		tempId = glCreateShader((GLenum)type);
		std::cout << "trying" << std::endl;
		std::string ssource = std::string();

		try
		{
			ssource = getFileContents(path);
			
		}
		catch(const std::ios_base::failure& e)
		{
			std::cout << "Caught exception: " << e.what() << std::endl;
			return false;
		}
		
		const char* source = ssource.c_str();

		glShaderSource(tempId, 1, &source, NULL);
		glCompileShader(tempId);

		if (_GetCompilerErrors(tempId, type) == false)
		{
			// TODO: got compilation error
			// TODO: reset id map probably
			return false;
		}

		_shaderIds[type] = tempId;
	}

	_id = glCreateProgram();
	
	for (const auto& [type, id] : _shaderIds)
	{
		glAttachShader(_id, id);
	}

	glLinkProgram(_id);

	if (_GetCompilerErrors(_id, ShaderType::Program) == false)
	{
		// TODO: error in linking process need to clean up
		// TODO: reset _id probably?
		//glDeleteShader(vertexShader);
		//glDeleteShader(fragmentShader);
		return false;
	}

	for (const auto& [type, id] : _shaderIds)
	{
		glDeleteShader(id);
	}

	// TODO: clear maps out for potential shader reuse?
	//_shaderPaths.clear(); // maybe this one should stay populated
	_shaderIds.clear();

	_GetShaderUniforms();

	return true;
}

std::string Shader::GetShaderInfo()
{
	std::stringstream ss;
	
	ss << "Shader id: " << _id << "\n";
	ss << "Sources:\n";
	
	for (const auto& [type, path] : _shaderPaths)
	{
		switch (type)
		{
			case ShaderType::Fragment:
				ss << "\tFragment: ";
				break;
			case ShaderType::Vertex:
				ss << "\tFragment: ";
				break;
			case ShaderType::Geometry:
				ss << "\tGeometry: ";
				break;
			case ShaderType::Compute:
				ss << "\tCompute: ";
				break;
			default:
				ss << "\tUnknown: ";
		}
		ss << path << "\n";
	}

	ss << "Uniforms:\n";
	for (const auto& [name, info] : _uniformInfos)
	{
		ss << "\t" << name;
		ss << "; length: " << info.length;
		ss << "; size: " << info.size;
		ss << "; type: " << std::hex << info.type << std::dec;
		ss << "; location: " << info.location << "\n";
	}

	return ss.str();
}


void Shader::SetUniform(const std::string& name, const int value)
{
	GLint location = _uniformInfos[name].location;
	glUniform1i(location, value);

	// if (std::is_same<T, glm::vec3>::value)
	// {
		
	// }
	// else if (std::is_same<T, glm::vec4>::value)
	// {
	// 	glUniform4fv(location, 1, glm::value_ptr(value));
	// }
	// else if (std::is_same<T, glm::mat3>::value)
	// {
	// 	glUniformMatrix3fv(location, 1, false, (const float*)glm::value_ptr(value));
	// }
	// else if (std::is_same<T, glm::mat4>::value)
	// {
	// 	glUniformMatrix4fv(location, 1, false, (const float*)glm::value_ptr(value));
	// }
	// else
	// {
	// 	// TODO: value type not supported
	// }
}

void Shader::SetUniform(const std::string& name, const float value)
{
	GLint location = _uniformInfos[name].location;
	glUniform1f(location, value);
}

void Shader::SetUniform(const std::string& name, const glm::vec2& value)
{
	GLint location = _uniformInfos[name].location;
	glUniform2fv(location, 1, glm::value_ptr(value));
}

void Shader::SetUniform(const std::string& name, const glm::vec3& value)
{
	GLint location = _uniformInfos[name].location;
	glUniform3fv(location, 1, glm::value_ptr(value));
}

template<typename T>
void Shader::SetUniformArray(const std::string& name, const T& values, size_t count)
{
	GLint location = _uniformInfos[name];

	if (std::is_same<T, int*>::values)
	{
		glUniform1iv(location, static_cast<GLsizei>(count), values);
	}
	else if (std::is_same<T, float*>::values)
	{
		glUniform1fv(location, static_cast<GLsizei>(count), values);
	}
	else
	{
		// TODO: value type not supported
	}
}


bool Shader::_GetCompilerErrors(GLuint shader, ShaderType type)
{
	GLint didCompile = GL_FALSE;

	char infoLog[1024];

	if (type != ShaderType::Program)
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &didCompile);

		if (didCompile == GL_FALSE)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);

			// TODO: store the error info, compilation error
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &didCompile);
			
		if (didCompile == GL_FALSE)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			
			// TODO: store the error info, linking error
		}
	}

	return didCompile;
}

void Shader::_GetShaderUniforms()
{
	GLint uniformCount = 0;
	GLint maxLength = 0;

	glGetProgramiv(_id, GL_ACTIVE_UNIFORMS, &uniformCount);
	glGetProgramiv(_id, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLength);

	GLchar* uName = new GLchar[maxLength];
	GLsizei length;
	GLint size;
	GLenum type;
	GLint location;

	for (GLint i = 0; i < uniformCount; i++)
	{

		glGetActiveUniform(_id, i, sizeof(uName), &length, &size, &type, uName);
		location = glGetUniformLocation(_id, uName);
		std::string uniformName = uName;
		_uniformInfos[uniformName] = { length, size, type, location };
	}

	delete[] uName;

}