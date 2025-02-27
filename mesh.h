#pragma once

#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "material.h"
#include "vertex.h"


class Mesh
{
	private:
		std::vector<Vertex> _vertices;
		std::vector<GLuint> _indices;
		//std::vector<Texture> _textures;

		glm::vec3 _position = glm::vec3(0.0f);
		glm::quat _rotation = glm::quat(glm::vec3(0.0f));
		glm::vec3 _scale = glm::vec3(1.0f);
		glm::mat4 _transform = glm::mat4(1.0f);

		GLuint _vao = 0;
		GLuint _vbo = 0;

		// TODO: put in content manager?
		glm::mat4 _preTransform = glm::mat4(1.0f);

	public:
		Mesh();
		Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices);
		~Mesh();
		bool LoadFromFile(std::string path);
		void Update(float deltaTime);
		void Draw();
};

// Model collection of meshes and materials