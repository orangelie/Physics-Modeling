/*
*
* < FrameResources.h >
* Github: https://github.com/orangelie
* // Copyright (C) 2022 by orangelie, Inc. All right reserved.
* // MIT License
*
*/





#pragma once

#include "../Utility/DxgiUtil.h"
#include "../Mesh/MeshTools.h"
#include "HEADER/PUBLIC/Lighting/LightingUtils.h"

namespace orangelie {

	namespace Rendering {

		enum class RenderLayer : int {
			Opaque = 0,
			Count,
		};

		struct RenderItem {
			RenderItem() = default;

			XMFLOAT4X4 World = orangelie::Utility::Tools::Identity();
			XMFLOAT4X4 TexTransform = orangelie::Utility::Tools::Identity();

			int NumFramesDirty = gNumFramesDirty;

			orangelie::Mesh::MeshGeometry* MeshGeo = nullptr;

			D3D12_PRIMITIVE_TOPOLOGY PrimitiveTopology;

			UINT ObjCBIndex = 0;

			UINT IndexCount;
			UINT StartIndexLocation;
			UINT BaseVertexLocation;

			orangelie::Lighting::Material* Material;
		};
	}
}