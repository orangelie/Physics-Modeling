/*
*
* < TextureTools.cpp >
* Github: https://github.com/orangelie
* // Copyright (C) 2022 by orangelie, Inc. All right reserved.
* // MIT License
*
*/





#include "HEADER/PUBLIC/Texture/TextureTools.h"

namespace orangelie {

	namespace Texture {

		TextureLoader::TextureLoader() {
		}

		TextureLoader::~TextureLoader() {
		}

		void TextureLoader::AddTexture(
			const char* Name,
			const wchar_t* TexturePath,
			ID3D12Device* Device,
			ID3D12GraphicsCommandList* CmdList) {

			auto texData = std::make_unique<TextureData>();
			texData->Name = Name;
			HR(CreateDDSTextureFromFile12(Device, CmdList, TexturePath, texData->GPUHeap, texData->GPUUploader));

			m_Textures[Name] = std::move(texData);
		}

		TextureData* TextureLoader::GetTexture(std::string Name) {
			return m_Textures[Name].get();
		}
	}
}