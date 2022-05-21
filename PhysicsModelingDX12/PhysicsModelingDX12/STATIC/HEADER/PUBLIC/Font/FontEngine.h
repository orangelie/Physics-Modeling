/*
*
* < LightingUtils.h >
* Github: https://github.com/orangelie
* // Copyright (C) 2022 by orangelie, Inc. All right reserved.
* // MIT License
*
*/





#pragma once

#include "../Utility/DxgiUtil.h"
#include "../Texture/TextureTools.h"
#include "../Mesh/MeshTools.h"
#include "../Rendering/RenderSystem.h"
#include "../FrameResources.h"
#include "FontLoader.h"

namespace orangelie {

	namespace Font {

		struct SentenceType {
			std::vector<orangelie::Mesh::Vertex2> vertices;
			UINT indexSize;

			UINT maxLength, vertexID, vertexCount, indexCount;
			float red, blue, green;
		};

		class FontEngine {
		public:
			FontEngine();
			FontEngine(const FontEngine&) = delete;
			FontEngine& operator=(const FontEngine&) = delete;
			~FontEngine();

			void Initialize(
				ID3D12Device* Device,
				ID3D12GraphicsCommandList* CmdList,
				UINT maxLength,
				UINT screenWidth,
				UINT screenHeight,
				XMMATRIX viewMatrix,
				float red, float green, float blue,
				UINT heapID,
				const char* text,
				int positionX,
				int positionY);

			void UpdateSentence(
				ID3D12Device* Device,
				ID3D12GraphicsCommandList* CmdList,
				float red, float green, float blue,
				const char* text,
				int positionX,
				int positionY,
				SentenceType* sentence);

			void RenderSentence(
				ID3D12Device* Device,
				ID3D12GraphicsCommandList* CmdList,
				std::vector<orangelie::Rendering::RenderItem*> Ritems,
				orangelie::FrameResource* CurrframeResource,
				SentenceType* sentence);

			SentenceType* GetSentence() const;

		private:


		private:
			std::unique_ptr<SentenceType> m_Sentence_1;
			XMFLOAT4X4 m_ViewMatrix;
			UINT m_ScreenWidth, m_ScreenHeight;
			std::unique_ptr<orangelie::Mesh::MeshGeometry> m_MeshGeometry;
			orangelie::Font::FontLoader m_Floader;

		};
	}
}