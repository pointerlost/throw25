#include "graphics/Textures/Textures.h"

#include <filesystem>

#include "core/Logger.h"
#include "graphics/Textures/stb_image.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Graphics
{
	uint32_t TextureManager::load(const std::string& name, const std::string &filePath, int width, int height, int nrChannels)
	{
		if (hasTextureWithPath(filePath))
			return m_pathMap[filePath]->glID;

		uint32_t texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		// set the texture wrapping/filtering options (on the currently bound texture object)
		// default values but user can change with imgui
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		const auto texturePtr = std::make_shared<Texture>();

		texturePtr->name = name;
		texturePtr->path = filePath;
		texturePtr->width = width;
		texturePtr->height = height;
		texturePtr->nrChannels = nrChannels;
		texturePtr->glID = texture;

		glBindTexture(GL_TEXTURE_2D, texture);

		stbi_set_flip_vertically_on_load(true);

		if (unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0))
		{
			GLenum format = GL_RGB;
			GLint internalFormat = GL_RGB8;

			if (nrChannels == 1) {
				format = GL_RED;
				internalFormat = GL_R8;
			}
			else if (nrChannels == 3) {
				format = GL_RGB;
				internalFormat = GL_RGB8;
			}
			else if (nrChannels == 4) {
				format = GL_RGBA;
				internalFormat = GL_RGBA8;
			}

			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			stbi_image_free(data);

			m_nameMap[texturePtr->name] = texturePtr;
			m_pathMap[texturePtr->path] = texturePtr;
			m_allTextures.push_back(texturePtr);

			return texture;
		}

		Logger::warn("Failed to load texture \"" + filePath + "\".");
		return {};
	}

	void TextureManager::bind(uint32_t texID, uint32_t slot) {
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, texID);
	}

	std::shared_ptr<Texture> TextureManager::getTextureWithName(const std::string &name) {
		if (!m_nameMap.contains(name)) {
			Logger::warn("Texture \"" + name + "\" not found.");
			return nullptr;
		}
		return m_nameMap[name];
	}

	std::shared_ptr<Texture> TextureManager::getTextureWithPath(const std::string &path) {
		if (!m_pathMap.contains(path)) {
			Logger::warn("Texture \"" + path + "\" not found.");
			return nullptr;
		}
		return m_pathMap[path];
	}

	std::vector<std::shared_ptr<Texture>> TextureManager::getAllTextures() const {
		if (m_allTextures.empty()) {
			Logger::warn("No textures found.");
			return {};
		}
		return m_allTextures;
	}
}
