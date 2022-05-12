/*
*
* < LightingUtils.h >
* Github: https://github.com/orangelie
* // Copyright (C) 2022 by orangelie, Inc. All right reserved.
* // MIT License
*
*/





#include "HEADER/PUBLIC/Font/FontEngine.h"

namespace orangelie {

	namespace Font {

		FontEngine::FontEngine() {
		}

		FontEngine::~FontEngine() {
		}

		void FontEngine::Initialize(
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
			int positionY) {

			// value Initializing
			XMStoreFloat4x4(&m_ViewMatrix, viewMatrix);
			m_ScreenWidth = screenWidth;
			m_ScreenHeight = screenHeight;

			// pointer Initializing
			m_Sentence_1 = std::make_unique<SentenceType>();

			m_Sentence_1->maxLength = maxLength;

			m_Sentence_1->red = red;
			m_Sentence_1->green = green;
			m_Sentence_1->blue = blue;

			m_Sentence_1->vertexCount = maxLength * 4;
			m_Sentence_1->indexCount = maxLength * 6;

			m_Sentence_1->vertexID = heapID;

			// build heap memory
			std::vector<orangelie::Mesh::Vertex2> vertices;
			vertices.resize(m_Sentence_1->vertexCount);

			std::vector<std::uint32_t> indices;
			indices.resize(m_Sentence_1->indexCount);

			m_MeshGeometry = std::make_unique<orangelie::Mesh::MeshGeometry>();
			m_MeshGeometry->Name = "textGeo";

			const UINT vBytes = sizeof(orangelie::Mesh::Vertex2) * (UINT)vertices.size();
			const UINT iBytes = sizeof(std::uint32_t) * (UINT)indices.size();

			m_MeshGeometry->GpuVertexBuffer = orangelie::Utility::Tools::CreateDefaultBuffer(Device, CmdList,
				vertices.data(), vBytes, m_MeshGeometry->GpuVertexUploader);
			m_MeshGeometry->GpuIndexBuffer = orangelie::Utility::Tools::CreateDefaultBuffer(Device, CmdList,
				indices.data(), iBytes, m_MeshGeometry->GpuIndexUploader);
			HR(D3DCreateBlob(vBytes, m_MeshGeometry->CpuVertexBuffer.GetAddressOf()));
			CopyMemory(m_MeshGeometry->CpuVertexBuffer->GetBufferPointer(), vertices.data(), vBytes);
			HR(D3DCreateBlob(iBytes, m_MeshGeometry->CpuIndexBuffer.GetAddressOf()));
			CopyMemory(m_MeshGeometry->CpuIndexBuffer->GetBufferPointer(), indices.data(), iBytes);

			m_MeshGeometry->IndexBufferByteSize = iBytes;
			m_MeshGeometry->IndexFormat = DXGI_FORMAT_R32_UINT;
			m_MeshGeometry->VertexBufferByteSize = vBytes;
			m_MeshGeometry->VertexByteStride = sizeof(orangelie::Mesh::Vertex2);

			orangelie::Mesh::SubmeshGeometry submeshGeo;
			submeshGeo.IndexCount = (UINT)indices.size();
			submeshGeo.BaseVertexLocation = 0;
			submeshGeo.StartIndexLocation = 0;

			m_MeshGeometry->Submeshes["text"] = submeshGeo;

			// update sentence
			m_Floader.Initialize(Device, CmdList, "./Font/fontdata.txt", L"./Textures/font.dds");
			UpdateSentence(Device, CmdList, red, green, blue, text, positionX, positionY, m_Sentence_1.get());
		}

		void FontEngine::UpdateSentence(
			ID3D12Device* Device,
			ID3D12GraphicsCommandList* CmdList,
			float red, float green, float blue,
			const char* text,
			int positionX,
			int positionY,
			SentenceType* sentence) {

			UINT numLetters = (UINT)strlen(text);
			if (sentence->maxLength < numLetters)
				HR(-1);

			std::vector<orangelie::Mesh::Vertex2> vertices;
			vertices.resize((size_t)(numLetters * 4));
			memset(vertices.data(), 0x00, vertices.size() * sizeof(orangelie::Mesh::Vertex2));

			std::vector<std::uint32_t> indices;
			indices.resize((size_t)(numLetters * 6));
			memset(indices.data(), 0x00, indices.size() * sizeof(std::uint32_t));

			float drawX = (float)(((m_ScreenWidth / 2) * -1) + positionX);
			float drawY = (float)((m_ScreenHeight / 2) - positionY);

			m_Floader.BuildVertexIndexArray(vertices, indices, text, drawX, drawY);

			sentence->vertices = vertices;
			sentence->indexCount = (UINT)indices.size();
		}

		void FontEngine::RenderSentence(
			ID3D12Device* Device,
			ID3D12GraphicsCommandList* CmdList,
			std::vector<orangelie::Rendering::RenderItem*> Ritems,
			orangelie::FrameResource* CurrframeResource,
			SentenceType* sentence) {

			auto fontvb = CurrframeResource->m_FontVB.get();
			fontvb->CopyData(sentence->vertexID, sentence->vertices);

			for (auto& r : Ritems) {
				r->MeshGeo->GpuVertexBuffer = fontvb->Resource();

				CmdList->IASetVertexBuffers(0, 1, &orangelie::CppStdUtil::unmove(r->MeshGeo->VertexBufferView()));
				CmdList->IASetIndexBuffer(&orangelie::CppStdUtil::unmove(r->MeshGeo->IndexBufferView()));
				CmdList->IASetPrimitiveTopology(r->PrimitiveTopology);

				CmdList->DrawIndexedInstanced(r->IndexCount, 1,
					r->StartIndexLocation, r->BaseVertexLocation, 0);
			}
		}

		SentenceType* FontEngine::GetSentence() const {
			return m_Sentence_1.get();
		}
	}
}