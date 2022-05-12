/*
*
* < FontLoader.h >
* Github: https://github.com/orangelie
* // Copyright (C) 2022 by orangelie, Inc. All right reserved.
* // MIT License
*
*/





#pragma once

#include "../Utility/DxgiUtil.h"
#include "../Texture/TextureTools.h"
#include "../Mesh/MeshTools.h"

namespace orangelie {

	namespace Font {

		struct FontData {
			float left, right;
			UINT size;
		};

		class FontLoader {
		public:
			FontLoader();
			FontLoader(const FontLoader&) = delete;
			FontLoader& operator=(const FontLoader&) = delete;
			~FontLoader();

			void Initialize(ID3D12Device* Device,
				ID3D12GraphicsCommandList* CmdList,
				const char* fontFilename,
				const wchar_t* textureFilename);

			void BuildVertexIndexArray(
				std::vector<orangelie::Mesh::Vertex2>& vertices,
				std::vector<std::uint32_t>& indices,
				const char* sentence,
				float drawX,
				float drawY);

		private:


		private:
			orangelie::Texture::TextureLoader m_TextureLoader;
			std::vector<FontData> m_Font;

		};
	}
}