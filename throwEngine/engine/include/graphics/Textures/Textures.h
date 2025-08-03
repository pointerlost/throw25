#pragma once
#include <vector>
#include <memory>
#include <string>
#include <unordered_map>

namespace TEXTURE
{
	struct Texture {
		std::string name;
		std::string path;
		uint32_t glID = 0;
		int width = 0;
		int height = 0;
		int nrChannels = 0;
	};

	class TextureManager {
	public:
		TextureManager() = default;
		~TextureManager() = default;

		uint32_t load(const std::string& name, const std::string& filePath, int width = 2, int height = 2, int nrChannels = 0);
		void bind(uint32_t texID, uint32_t slot);

		[[nodiscard]] bool hasTextureWithName(const std::string& name) const { return m_nameMap.contains(name); };
		[[nodiscard]] bool hasTextureWithPath(const std::string& path) const { return m_pathMap.contains(path); };

		[[nodiscard]] std::shared_ptr<Texture> getTextureWithName(const std::string& name);
		[[nodiscard]] std::shared_ptr<Texture> getTextureWithPath(const std::string& path);
		[[nodiscard]] std::vector<std::shared_ptr<Texture>> getAllTextures() const;

	private:
		std::unordered_map<std::string, std::shared_ptr<Texture>> m_nameMap; // name -> texture (for UI stuff)
		std::unordered_map<std::string, std::shared_ptr<Texture>> m_pathMap; // file path -> texture
		std::vector<std::shared_ptr<Texture>> m_allTextures; // for iteration
	};

}
