#include "HEADER/PUBLIC/Terrain/HeightMapTerrain.h"

namespace orangelie {

	namespace Terrain {

		TerrainHeightMap::TerrainHeightMap() {
		}

		TerrainHeightMap::~TerrainHeightMap() {
		}

		MeshData TerrainHeightMap::CreateHeightMap(const char* fileName) {
			MeshData meshData;

			LoadHeightMapFile(fileName);
			NormalizeHeightMap(meshData);
			CreateHeightMapVB(meshData);

			return meshData;
		}

		void TerrainHeightMap::LoadHeightMapFile(const char* fileName) {

		}

		void TerrainHeightMap::NormalizeHeightMap(MeshData meshData) {

		}

		void TerrainHeightMap::CreateHeightMapVB(MeshData meshData) {

		}
	}
}