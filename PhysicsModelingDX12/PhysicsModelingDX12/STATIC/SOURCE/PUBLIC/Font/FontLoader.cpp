/*
*
* < FontLoader.cpp >
* Github: https://github.com/orangelie
* // Copyright (C) 2022 by orangelie, Inc. All right reserved.
* // MIT License
*
*/





#include "HEADER/PUBLIC/Font/FontLoader.h"

namespace orangelie {

	namespace Font {

		FontLoader::FontLoader() {
		}

		FontLoader::~FontLoader() {
		}

		void FontLoader::Initialize(ID3D12Device* Device,
			ID3D12GraphicsCommandList* CmdList,
			const char* fontFilename,
			const wchar_t* textureFilename) {

			// create a texture.
			m_TextureLoader.AddTexture("bmfont", textureFilename, Device, CmdList);

			// load font data file.
			m_Font.resize(95);

			std::ifstream ifs(fontFilename);
			char temp;

			if (ifs.fail())
				HR(-1);

			for (UINT i = 0; i < 95; ++i) {
				ifs.get(temp); while (temp != ' ') { ifs.get(temp); }
				ifs.get(temp); while (temp != ' ') { ifs.get(temp); }

				ifs >> m_Font[i].left;
				ifs >> m_Font[i].right;
				ifs >> m_Font[i].size;
			}

			ifs.close();
		}

		void FontLoader::BuildVertexIndexArray(
			std::vector<orangelie::Mesh::Vertex2>& vertices,
			std::vector<std::uint32_t>& indices,
			const char* sentence,
			float drawX,
			float drawY) {

			/*
			*
			* sentenceLength: 표시할 글자의 수
			* letter: 표시될 글자
			* vindex: 정점버퍼 인덱싱 누적
			* iindex: 인덱스버퍼 인덱싱 누적
			* sentenceHeight: 한글자의 세로길이
			*
			*/

			const int sentenceHeight = 16;
			int sentenceLength = (int)strlen(sentence);
			int vindex = 0, iindex = 0;

			for (int i = 0; i < sentenceLength; ++i) {
				int letter = ((int)sentence[i]) - 32; // <== 32는 글꼴텍스트파일에서 이미 정해진숫자입니다. 따라서 반드시 32를 빼야합니다.

				// 만약 letter가 ' '라면 다음으로 그려질 글자의 x좌표 3.0만큼 증가
				if (letter == 0) {
					drawX += 3.0f;
				}

				else {
					XMFLOAT3 v1 = XMFLOAT3(drawX, drawY, 0.0f);	// top left (0)
					XMFLOAT3 v2 = XMFLOAT3(drawX + m_Font[letter].size, drawY, 0.0f);	// top right (1)
					XMFLOAT3 v3 = XMFLOAT3(drawX, drawY - sentenceHeight, 0.0f); // bottom left (2)
					XMFLOAT3 v4 = XMFLOAT3(drawX + m_Font[letter].size, drawY - sentenceHeight, 0.0f); //bottom right (3)

					XMFLOAT2 TexC_TopLeft = XMFLOAT2(m_Font[letter].left, 0.0f);
					XMFLOAT2 TexC_TopRight = XMFLOAT2(m_Font[letter].right, 0.0f);
					XMFLOAT2 TexC_BottomLeft = XMFLOAT2(m_Font[letter].left, 1.0f);
					XMFLOAT2 TexC_BottomRight = XMFLOAT2(m_Font[letter].right, 1.0f);

					std::uint32_t i1 = 0;
					std::uint32_t i2 = 1;
					std::uint32_t i3 = 2;
					std::uint32_t i4 = 1;
					std::uint32_t i5 = 2;
					std::uint32_t i6 = 3;


					vertices[vindex + 0].Position = v1;
					vertices[vindex + 1].Position = v2;
					vertices[vindex + 2].Position = v3;
					vertices[vindex + 3].Position = v4;
					vindex += 4;

					indices[iindex + 0] = i1;
					indices[iindex + 1] = i2;
					indices[iindex + 2] = i3;
					indices[iindex + 3] = i4;
					indices[iindex + 2] = i5;
					indices[iindex + 3] = i6;
					iindex += 6;

					drawX = drawX + m_Font[letter].size + 1.0f;
				}
			}
		}
	}
}