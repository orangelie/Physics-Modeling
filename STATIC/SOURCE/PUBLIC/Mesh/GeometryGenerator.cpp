/*
*
* < GeometryGenerator.cpp >
* Github: https://github.com/orangelie
* // Copyright (C) 2022 by orangelie, Inc. All right reserved.
* // MIT License
*
*/





#pragma once

#include "HEADER/PUBLIC/Mesh/GeometryGenerator.h"

namespace orangelie {

	namespace Mesh {
		GeometryGenerator::GeometryGenerator() {
		}

		GeometryGenerator::~GeometryGenerator() {
		}

		GeometryGenerator::MeshData GeometryGenerator::CreateGrid(float width, float depth, uint32 m, uint32 n) {
			const uint32 faceCount = 2 * (m - 1) * (n - 1);
			const uint32 vertexCount = m * n;

			const float halfWidth = width * 0.5f;
			const float halfDepth = depth * 0.5f;

			const float dx = width / (n - 1);
			const float dz = depth / (m - 1);

			const float du = 1.0f / (n - 1);
			const float dv = 1.0f / (m - 1);

			MeshData mesh;
			mesh.vertices.resize(vertexCount);

			for (uint32 i = 0; i < m; ++i) {
				float z = halfDepth - i * dz;

				for (uint32 j = 0; j < n; ++j) {
					float x = -halfWidth + j * dx;

					mesh.vertices[i * n + j].Position = XMFLOAT3(x, 0.0f, z);
					mesh.vertices[i * n + j].Tangent = XMFLOAT3(1.0f, 0.0f, 0.0f);
					mesh.vertices[i * n + j].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);

					mesh.vertices[i * n + j].TexC.x = j * du;
					mesh.vertices[i * n + j].TexC.y = i * dv;
				}
			}

			mesh.indices.resize(faceCount * 3);
			uint32 k = 0;

			for (uint32 i = 0; i < (m - 1); ++i) {
				for (uint32 j = 0; j < (n - 1); ++j) {
					mesh.indices[k + 0] = (i + 0) * n + (j + 0);
					mesh.indices[k + 1] = (i + 0) * n + (j + 1);
					mesh.indices[k + 2] = (i + 1) * n + (j + 0);

					mesh.indices[k + 3] = (i + 1) * n + (j + 0);
					mesh.indices[k + 4] = (i + 0) * n + (j + 1);
					mesh.indices[k + 5] = (i + 1) * n + (j + 1);

					k += 6;
				}
			}

			return mesh;
		}

		GeometryGenerator::MeshData GeometryGenerator::CreateSphere(float radius, uint32 sliceCount, uint32 stackCount)
		{
			MeshData meshData;

			Vertex topVertex(0.0f, +radius, 0.0f, 0.0f, +1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
			Vertex bottomVertex(0.0f, -radius, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);

			meshData.vertices.push_back(topVertex);

			float phiStep = XM_PI / stackCount;
			float thetaStep = 2.0f * XM_PI / sliceCount;

			for (uint32 i = 1; i <= stackCount - 1; ++i)
			{
				float phi = i * phiStep;

				for (uint32 j = 0; j <= sliceCount; ++j)
				{
					float theta = j * thetaStep;

					Vertex v;

					v.Position.x = radius * sinf(phi) * cosf(theta);
					v.Position.y = radius * cosf(phi);
					v.Position.z = radius * sinf(phi) * sinf(theta);

					v.Tangent.x = -radius * sinf(phi) * sinf(theta);
					v.Tangent.y = 0.0f;
					v.Tangent.z = +radius * sinf(phi) * cosf(theta);

					XMVECTOR T = XMLoadFloat3(&v.Tangent);
					XMStoreFloat3(&v.Tangent, XMVector3Normalize(T));

					XMVECTOR p = XMLoadFloat3(&v.Position);
					XMStoreFloat3(&v.Normal, XMVector3Normalize(p));

					v.TexC.x = theta / XM_2PI;
					v.TexC.y = phi / XM_PI;

					meshData.vertices.push_back(v);
				}
			}

			meshData.vertices.push_back(bottomVertex);


			for (uint32 i = 1; i <= sliceCount; ++i)
			{
				meshData.indices.push_back(0);
				meshData.indices.push_back(i + 1);
				meshData.indices.push_back(i);
			}


			uint32 baseIndex = 1;
			uint32 ringVertexCount = sliceCount + 1;
			for (uint32 i = 0; i < stackCount - 2; ++i)
			{
				for (uint32 j = 0; j < sliceCount; ++j)
				{
					meshData.indices.push_back(baseIndex + i * ringVertexCount + j);
					meshData.indices.push_back(baseIndex + i * ringVertexCount + j + 1);
					meshData.indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);

					meshData.indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);
					meshData.indices.push_back(baseIndex + i * ringVertexCount + j + 1);
					meshData.indices.push_back(baseIndex + (i + 1) * ringVertexCount + j + 1);
				}
			}


			uint32 southPoleIndex = (uint32)meshData.vertices.size() - 1;

			baseIndex = southPoleIndex - ringVertexCount;

			for (uint32 i = 0; i < sliceCount; ++i)
			{
				meshData.indices.push_back(southPoleIndex);
				meshData.indices.push_back(baseIndex + i);
				meshData.indices.push_back(baseIndex + i + 1);
			}

			return meshData;
		}
	}
}