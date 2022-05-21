/*
*
* < TextureTools.h >
* Github: https://github.com/orangelie
* // Copyright (C) 2022 by orangelie, Inc. All right reserved.
* // MIT License
*
*/





#pragma once

#include "../Utility/DxgiUtil.h"
#include "../Texture/DDSTextureLoader.h"

namespace orangelie {

	namespace Texture {

		struct TextureData {
			std::string Name;

			ComPtr<ID3D12Resource> GPUHeap;
			ComPtr<ID3D12Resource> GPUUploader;
		};

		class TextureLoader {
		public:
			TextureLoader();
			TextureLoader& operator=(const TextureLoader&) = delete;
			TextureLoader(const TextureLoader&) = delete;
			~TextureLoader();

			void AddTexture(
				const char* Name,
				const wchar_t* TexturePath,
				ID3D12Device* Device,
				ID3D12GraphicsCommandList* CmdList);

			TextureData* GetTexture(std::string Name);

		private:
			std::unordered_map<std::string, std::unique_ptr<TextureData>> m_Textures;

		};
	}
}