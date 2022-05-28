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
		GeometryGenerator::MeshData GeometryGenerator::CreateBox(float width, float height, float depth, uint32 numSubdivisions)
		{
			MeshData meshData;

			//
			// Create the vertices.
			//

			Vertex v[24];

			float w2 = 0.5f * width;
			float h2 = 0.5f * height;
			float d2 = 0.5f * depth;

			// Fill in the front face vertex data.
			v[0] = Vertex(-w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
			v[1] = Vertex(-w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
			v[2] = Vertex(+w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
			v[3] = Vertex(+w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

			// Fill in the back face vertex data.
			v[4] = Vertex(-w2, -h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
			v[5] = Vertex(+w2, -h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
			v[6] = Vertex(+w2, +h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
			v[7] = Vertex(-w2, +h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

			// Fill in the top face vertex data.
			v[8] = Vertex(-w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
			v[9] = Vertex(-w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
			v[10] = Vertex(+w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
			v[11] = Vertex(+w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

			// Fill in the bottom face vertex data.
			v[12] = Vertex(-w2, -h2, -d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
			v[13] = Vertex(+w2, -h2, -d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
			v[14] = Vertex(+w2, -h2, +d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
			v[15] = Vertex(-w2, -h2, +d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

			// Fill in the left face vertex data.
			v[16] = Vertex(-w2, -h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
			v[17] = Vertex(-w2, +h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
			v[18] = Vertex(-w2, +h2, -d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
			v[19] = Vertex(-w2, -h2, -d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

			// Fill in the right face vertex data.
			v[20] = Vertex(+w2, -h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
			v[21] = Vertex(+w2, +h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
			v[22] = Vertex(+w2, +h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
			v[23] = Vertex(+w2, -h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);

			meshData.vertices.assign(&v[0], &v[24]);

			//
			// Create the indices.
			//

			uint32 i[36];

			// Fill in the front face index data
			i[0] = 0; i[1] = 1; i[2] = 2;
			i[3] = 0; i[4] = 2; i[5] = 3;

			// Fill in the back face index data
			i[6] = 4; i[7] = 5; i[8] = 6;
			i[9] = 4; i[10] = 6; i[11] = 7;

			// Fill in the top face index data
			i[12] = 8; i[13] = 9; i[14] = 10;
			i[15] = 8; i[16] = 10; i[17] = 11;

			// Fill in the bottom face index data
			i[18] = 12; i[19] = 13; i[20] = 14;
			i[21] = 12; i[22] = 14; i[23] = 15;

			// Fill in the left face index data
			i[24] = 16; i[25] = 17; i[26] = 18;
			i[27] = 16; i[28] = 18; i[29] = 19;

			// Fill in the right face index data
			i[30] = 20; i[31] = 21; i[32] = 22;
			i[33] = 20; i[34] = 22; i[35] = 23;

			meshData.indices.assign(&i[0], &i[36]);

			// Put a cap on the number of subdivisions.
			numSubdivisions = std::min<uint32>(numSubdivisions, 6u);

			for (uint32 i = 0; i < numSubdivisions; ++i)
				Subdivide(meshData);

			return meshData;
		}

		void GeometryGenerator::Subdivide(MeshData& meshData)
		{
			// Save a copy of the input geometry.
			MeshData inputCopy = meshData;


			meshData.vertices.resize(0);
			meshData.indices.resize(0);

			//       v1
			//       *
			//      / \
			//     /   \
			//  m0*-----*m1
			//   / \   / \
			//  /   \ /   \
			// *-----*-----*
			// v0    m2     v2

			uint32 numTris = (uint32)inputCopy.indices.size() / 3;
			for (uint32 i = 0; i < numTris; ++i)
			{
				Vertex v0 = inputCopy.vertices[inputCopy.indices[i * 3 + 0]];
				Vertex v1 = inputCopy.vertices[inputCopy.indices[i * 3 + 1]];
				Vertex v2 = inputCopy.vertices[inputCopy.indices[i * 3 + 2]];

				//
				// Generate the midpoints.
				//

				Vertex m0 = MidPoint(v0, v1);
				Vertex m1 = MidPoint(v1, v2);
				Vertex m2 = MidPoint(v0, v2);

				//
				// Add new geometry.
				//

				meshData.vertices.push_back(v0); // 0
				meshData.vertices.push_back(v1); // 1
				meshData.vertices.push_back(v2); // 2
				meshData.vertices.push_back(m0); // 3
				meshData.vertices.push_back(m1); // 4
				meshData.vertices.push_back(m2); // 5

				meshData.indices.push_back(i * 6 + 0);
				meshData.indices.push_back(i * 6 + 3);
				meshData.indices.push_back(i * 6 + 5);

				meshData.indices.push_back(i * 6 + 3);
				meshData.indices.push_back(i * 6 + 4);
				meshData.indices.push_back(i * 6 + 5);

				meshData.indices.push_back(i * 6 + 5);
				meshData.indices.push_back(i * 6 + 4);
				meshData.indices.push_back(i * 6 + 2);

				meshData.indices.push_back(i * 6 + 3);
				meshData.indices.push_back(i * 6 + 1);
				meshData.indices.push_back(i * 6 + 4);
			}
		}

		GeometryGenerator::Vertex GeometryGenerator::MidPoint(const Vertex& v0, const Vertex& v1)
		{
			XMVECTOR p0 = XMLoadFloat3(&v0.Position);
			XMVECTOR p1 = XMLoadFloat3(&v1.Position);

			XMVECTOR n0 = XMLoadFloat3(&v0.Normal);
			XMVECTOR n1 = XMLoadFloat3(&v1.Normal);

			XMVECTOR tan0 = XMLoadFloat3(&v0.Tangent);
			XMVECTOR tan1 = XMLoadFloat3(&v1.Tangent);

			XMVECTOR tex0 = XMLoadFloat2(&v0.TexC);
			XMVECTOR tex1 = XMLoadFloat2(&v1.TexC);

			// Compute the midpoints of all the attributes.  Vectors need to be normalized
			// since linear interpolating can make them not unit length.  
			XMVECTOR pos = 0.5f * (p0 + p1);
			XMVECTOR normal = XMVector3Normalize(0.5f * (n0 + n1));
			XMVECTOR tangent = XMVector3Normalize(0.5f * (tan0 + tan1));
			XMVECTOR tex = 0.5f * (tex0 + tex1);

			Vertex v;
			XMStoreFloat3(&v.Position, pos);
			XMStoreFloat3(&v.Normal, normal);
			XMStoreFloat3(&v.Tangent, tangent);
			XMStoreFloat2(&v.TexC, tex);

			return v;
		}

		/////////////////////////////////////////

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
			GeometryGenerator::MeshData meshData;

			//
			// Compute the vertices stating at the top pole and moving down the stacks.
			//

			// Poles: note that there will be texture coordinate distortion as there is
			// not a unique point on the texture map to assign to the pole when mapping
			// a rectangular texture onto a sphere.
			Vertex topVertex(0.0f, +radius, 0.0f, 0.0f, +1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
			Vertex bottomVertex(0.0f, -radius, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);

			meshData.vertices.push_back(topVertex);

			float phiStep = XM_PI / stackCount;
			float thetaStep = 2.0f * XM_PI / sliceCount;

			// Compute vertices for each stack ring (do not count the poles as rings).
			for (uint32 i = 1; i <= stackCount - 1; ++i)
			{
				float phi = i * phiStep;

				// Vertices of ring.
				for (uint32 j = 0; j <= sliceCount; ++j)
				{
					float theta = j * thetaStep;

					Vertex v;

					// spherical to cartesian
					v.Position.x = radius * sinf(phi) * cosf(theta);
					v.Position.y = radius * cosf(phi);
					v.Position.z = radius * sinf(phi) * sinf(theta);

					// Partial derivative of P with respect to theta
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