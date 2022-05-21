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

namespace orangelie {

	namespace Lighting {

		struct Light {
			XMFLOAT3 Strength;
			float FalloffStart;
			XMFLOAT3 Position;
			float FalloffEnd;
			XMFLOAT3 Direction;
			float SpotPower;
		};

		struct Material {
			XMFLOAT4X4 MatTransform;
			XMFLOAT4 DiffuseAlbedo;
			XMFLOAT3 R0;
			float Roughness;

			UINT MatIndex;
			UINT SrvHeapIndex;

			int NumFramesDirty = gNumFramesDirty;
		};
	}
}