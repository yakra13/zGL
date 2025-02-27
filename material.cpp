#include "material.h"

Material::Material(std::shared_ptr<Shader> shader)
	: _shader(shader)
{
}

Material::~Material()
{
}

template<typename T>
void Material::SetUniform(const std::string& name, const T& value)
{
	_shader.SetUniform(name, value);

}