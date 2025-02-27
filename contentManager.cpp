#include "contentManager.h"

void ContentManager::SetRootDirectory(const std::string& directory)
{
	// TODO: validation all over the place
	// append an appropriate '/' or '\' if missing
	_rootDirectory = directory;
}


Model ContentManager::LoadModel(const std::string& file)
{
	std::string fullPath = _rootDirectory + file;
	if (_meshCache.find(fullPath) != _meshCache.end())
	{
		// Check if the shader resource has existing references
		// lock() will return nullptr if there are no reference to the resource
		std::shared_ptr<Mesh> m = _meshCache[fullPath].lock();
		if (m != nullptr)
		{
			return Model(m);
		}
		// If not then erase it from the map
		_meshCache.erase(fullPath);
	}

	auto m = std::make_shared<Mesh>();
	if (m->LoadFromFile(fullPath))
	{
		_meshCache[fullPath] = m;
		return Model(m);
	}

	// TODO: return a model with defaults like a cube + checker material
	return Model();
}

std::shared_ptr<Shader> ContentManager::LoadShader(const std::vector<ShaderFilePair>& files)
{
	// Sort the shader files by name
	std::vector<ShaderFilePair> sortedFiles = files;

	std::sort(sortedFiles.begin(), sortedFiles.end(), [](const ShaderFilePair& a, const ShaderFilePair& b)
	{
		return a.first < b.first;  // Sort based on the file path (first element)
	});

	// Create the map key for this shader
	// NOTE: It is technically possible to skip loading a new shader if the files have the same name
	// as an already loaded shader but come from a different directory
	// ie:
	// SetContentDirectory("dir1");
	// LoadShader("shader.glsl"...);
	// SetContentDirectory("dir2");
	// LoadShader("shader.glsl"...); // Would return the shader loaded prior even tho they are different files.
	// TODO: set cacheKey to start as _currentDirectory? should prevent the above
	std::string cacheKey = "";
	for (const auto& file : sortedFiles)
	{
		cacheKey += file.first;
	}

	// Check if the shader already exists
	if (_shaderCache.find(cacheKey) != _shaderCache.end())
	{
		// Check if the shader resource has existing references
		// lock() will return nullptr if there are no reference to the resource
		std::shared_ptr<Shader> s = _shaderCache[cacheKey].lock();
		if (s != nullptr)
		{
			return s;
		}
		// If not then erase it from the map
		_shaderCache.erase(cacheKey);
	}

	// Build a new shader and add it to the cache
	auto s = std::make_shared<Shader>();
	// Add each source file to the shader
	for (const auto& file : files)
	{
		s->SetShaderSource(_rootDirectory + file.first, file.second);
	}

	// Build the shader
	if (s->Build())
	{
		_shaderCache[cacheKey] = s;
		return s;
	}

	// TODO: i believe Build() does something to let the user know it failed to build...

	return nullptr;
}

std::shared_ptr<Texture2D> ContentManager::LoadTexture2D(const std::string& file)
{
	std::string fullPath = _rootDirectory + file;
	// Check if the shader already exists
	if (_texture2dCache.find(fullPath) != _texture2dCache.end())
	{
		// Check if the texture resource has existing references
		std::shared_ptr<Texture2D> t = _texture2dCache[fullPath].lock();
		if (t != nullptr)
		{
			return t;
		}
		// If not then erase it from the map
		_texture2dCache.erase(fullPath);
	}

	auto t = std::make_shared<Texture2D>();
	if (t->LoadFromFile(fullPath))
	{
		_texture2dCache[fullPath] = t;
		return t;
	}

	// TODO: LoadFromFile failure

	return nullptr;
}
