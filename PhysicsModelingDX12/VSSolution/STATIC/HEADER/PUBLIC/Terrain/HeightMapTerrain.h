#pragma once

#include "HEADER/PUBLIC/Mesh/GeometryGenerator.h"

namespace orangelie {

	namespace Terrain {

		using MeshData = orangelie::Mesh::GeometryGenerator::MeshData;

		class TerrainHeightMap {
		public:
			TerrainHeightMap();
			TerrainHeightMap(const TerrainHeightMap&) = delete;
			TerrainHeightMap& operator=(const TerrainHeightMap&) = delete;
			~TerrainHeightMap();

			MeshData CreateHeightMap(const char* fileName);

		private:
			void LoadHeightMapFile(const char* fileName);
			void NormalizeHeightMap(MeshData meshData);
			void CreateHeightMapVB(MeshData meshData);

		private:


		};
	}
}