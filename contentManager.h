#pragma once

#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "model.h"
#include "shader.h"
#include "texture2d.h"

class ContentManager
{
	private:
		static ContentManager* instance;

		std::string _rootDirectory = "";

		std::map<std::string, std::weak_ptr<Mesh>> _meshCache;
		std::map<std::string, std::weak_ptr<Shader>> _shaderCache;
		std::map<std::string, std::weak_ptr<Texture2D>> _texture2dCache;

	public:
		using ShaderFilePair = std::pair<std::string, ShaderType>;

		ContentManager()
		{
			if (instance == nullptr)
			{
				instance = this;
			}
		}

		~ContentManager() { std::cout << "Destruct ContentManager" << std::endl; }

		// Delete the copy constructor
		ContentManager(const ContentManager&) = delete;
		// Delete assignment operator
		ContentManager& operator=(const ContentManager&) = delete;

		void SetRootDirectory(const std::string&);

		//template<typename T, typename... Args>
		//std::shared_ptr<T> Load(Args&&... args);
		Model LoadModel(const std::string& file);
		std::shared_ptr<Shader> LoadShader(const std::vector<ShaderFilePair>& shaderFiles);
		std::shared_ptr<Texture2D> LoadTexture2D(const std::string& file);
};

