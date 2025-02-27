#include "texture2d.h"

#include "stb_image.h"

Texture2D::Texture2D()
{
}

Texture2D::~Texture2D()
{
}

bool Texture2D::LoadFromFile(std::string path)
{
	stbi_set_flip_vertically_on_load(true);

	unsigned char* bytes = stbi_load(path.c_str(), &_width, &_height, &_channels, 0);

	if (bytes == nullptr)
	{
		return false;
	}

	_format = (_channels == 4) ? TextureFormat::RGBA : TextureFormat::RGB;
	
	glGenTextures(1, &_id);
	glBindTexture(GL_TEXTURE_2D, _id);
	
	// Configures the type of algorithm that is used to make the image smaller or bigger
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Configures the way the texture repeats (if it does at all)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, (GLenum)_format, _width, _height, 0, (GLenum)_format, GL_UNSIGNED_BYTE, bytes);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(bytes);
	
	return true;
}

void Texture2D::GenerateMipMaps()
{
	if (_id == 0)
	{
		//TODO: No texture loaded yet
		return;
	}

	if (_mipmap == true)
		return;

	// Bind the texture to update settings
	glBindTexture(GL_TEXTURE_2D, _id);
	
	// Update min filtering to account for mipmaps
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	// Generate the mipmaps
	glGenerateMipmap(GL_TEXTURE_2D);

	// Unbind the texture
	glBindTexture(GL_TEXTURE_2D, 0);

	_mipmap = true;
}
