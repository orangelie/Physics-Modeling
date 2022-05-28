/*
*
* < GeometryGenerator.h >
* Github: https://github.com/orangelie
* // Copyright (C) 2022 by orangelie, Inc. All right reserved.
* // MIT License
*
*/





#pragma once

#include "../Utility/DxgiUtil.h"
#include "MeshTools.h"

using uint16 = std::uint16_t;
using uint32 = std::uint32_t;

namespace orangelie {

	namespace Mesh {

		class GeometryGenerator {
		public:
			struct Vertex {
				XMFLOAT3 Position;
				XMFLOAT3 Normal;
				XMFLOAT3 Tangent;
				XMFLOAT2 TexC;

				Vertex() {}

				Vertex(XMFLOAT3 position, XMFLOAT3 norm, XMFLOAT3 tang, XMFLOAT2 tex) :
					Position(position), Normal(norm), Tangent(tang), TexC(tex) { }

				Vertex(
					float position_x, float position_y, float position_z,
					float norm_x, float norm_y, float norm_z,
					float tang_x, float tang_y, float tang_z,
					float texc_x, float texc_y) :
					Position(position_x, position_y, position_z),
					Normal(norm_x, norm_y, norm_z),
					Tangent(tang_x, tang_y, tang_z),
					TexC(texc_x, texc_y) { }
			};

			struct MeshData {
				std::vector<GeometryGenerator::Vertex> vertices;
				std::vector<uint32> indices;

				std::vector<uint16> GetIndices16() {
					std::vector<uint16> indices16(indices.size());

					for (size_t i = 0; i < indices.size(); i++) {
						indices16[i] = (uint16)indices[i];
					}

					return indices16;
				}
			};

			GeometryGenerator();
			GeometryGenerator(const GeometryGenerator&) = delete;
			GeometryGenerator& operator=(const GeometryGenerator&) = delete;
			~GeometryGenerator();

			GeometryGenerator::Vertex MidPoint(const Vertex& v0, const Vertex& v1);
			void Subdivide(MeshData& meshData);
			GeometryGenerator::MeshData CreateBox(float width, float height, float depth, uint32 numSubdivisions);
			static MeshData CreateGrid(float width, float depth, uint32 m, uint32 n);
			static MeshData CreateSphere(float radius, uint32 sliceCount, uint32 stackCount);

		private:


		};
	}
}