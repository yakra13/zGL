#pragma once

#include <map>

#include "glm/glm.hpp"

#include "shader.h"
#include "texture2d.h"

enum class RenderMode
{
	Opaque = 0,
	Cutout = 1,
	Transparent = 2,
};

struct BaseMaterial
{
	GLuint shaderID = 0;
	glm::vec3 diffuse = glm::vec3(1.0f);
	glm::vec3 emission = glm::vec3(0.0f);
	glm::vec3 specular = glm::vec3(0.0f);

	protected:
		RenderMode mode = RenderMode::Opaque;
};

struct OpaqueMaterial : BaseMaterial
{
	std::shared_ptr<Texture2D> diffuseMap;
	std::shared_ptr<Texture2D> emissionMap;
	std::shared_ptr<Texture2D> normalMap;
	std::shared_ptr<Texture2D> specularMap;
};

struct TransparentMaterial : OpaqueMaterial
{
	float transparency = 1.0f;

	TransparentMaterial() { mode = RenderMode::Transparent; }
};

class Material
{
	private:
		std::shared_ptr<Shader> _shader;
		std::map<std::string, std::shared_ptr<Texture2D>> _textures;

	public:
		Material(std::shared_ptr<Shader> shader);
		~Material();
		template <typename T>
		void SetUniform(const std::string& name, const T& value);
};