#pragma once
#pragma comment(linker, "/SUBSYSTEM:CONSOLE")
#pragma comment(lib, "dxgi")


/*        Direct3D         */
#include <DirectXCollision.h>
#include <DirectXColors.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>

using namespace DirectX;
using namespace DirectX::PackedVector;

/*        Windows         */
#include <Windows.h>
#include <windowsx.h>

/*        C++         */
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

/*        C           */
#include <conio.h>


#define BLOCK { \
	int _ = _getch(); \
} \

constexpr static const size_t gPointMaxSize = 50;

namespace orangelie {

	struct _PointMass {
		float mass;
		XMFLOAT3 correctedPosition;
		XMFLOAT3 designPosition;
		XMFLOAT3 localInertia;
	};

	static _PointMass gPointElements[gPointMaxSize];

	class InertiaTensor {
	public:
		InertiaTensor();
		InertiaTensor& operator=(const InertiaTensor&) = delete;
		InertiaTensor(const InertiaTensor&) = delete;
		virtual ~InertiaTensor();

		static void InitializeInertiaTensor(_PointMass* pMass, size_t size);
		static XMFLOAT3X3 CalcInertiaTensor(_PointMass* pMass, size_t size);
		static void PrintInertiaTensor(XMFLOAT3X3 InertiaTensor);

	private:


	};
}