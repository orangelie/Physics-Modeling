#include "InertiaTensor.h"

namespace orangelie {

	InertiaTensor::InertiaTensor() {
		memset(&gPointElements, 0x00, sizeof(_PointMass) * gPointMaxSize);
		SetWindowText(GetForegroundWindow(), "InertiaTensor");
	}

	InertiaTensor::~InertiaTensor() {
	}

	void InertiaTensor::InitializeInertiaTensor(_PointMass* pMass, size_t size) {
		pMass[0].correctedPosition = XMFLOAT3(1.0f, 1.0f, 1.0f);
		pMass[0].localInertia = XMFLOAT3(39.6f, 39.6, 12.5f);
		pMass[0].mass = 5.0f;

		pMass[1].correctedPosition = XMFLOAT3(0.0f, 0.0f, 0.0f);
		pMass[1].localInertia = XMFLOAT3(39.6f, 39.6, 12.5f);
		pMass[1].mass = 2.0f;

		pMass[2].correctedPosition = XMFLOAT3(5.0f, 5.0f, 5.0f);
		pMass[2].localInertia = XMFLOAT3(39.6f, 39.6, 12.5f);
		pMass[2].mass = 12.0f;

		pMass[3].correctedPosition = XMFLOAT3(12.0f, 12.0f, 12.0f);
		pMass[3].localInertia = XMFLOAT3(39.6f, 39.6, 12.5f);
		pMass[3].mass = 1.0f;
	}

	XMFLOAT3X3 InertiaTensor::CalcInertiaTensor(_PointMass* pMass, size_t size) {
		float Ixx = 0, Iyy = 0, Izz = 0, Ixy = 0, Iyz = 0, Izx = 0;
		XMFLOAT3X3 InertiaTensor;

		for (size_t i = 0; i < size; i++) {
			Ixx +=
				pMass[i].localInertia.x +
				pMass[i].mass *
				(pMass[i].correctedPosition.y * pMass[i].correctedPosition.y +
					pMass[i].correctedPosition.y * pMass[i].correctedPosition.z);

			Iyy +=
				pMass[i].localInertia.y +
				pMass[i].mass *
				(pMass[i].correctedPosition.x * pMass[i].correctedPosition.x +
					pMass[i].correctedPosition.z * pMass[i].correctedPosition.z);

			Izz +=
				pMass[i].localInertia.z +
				pMass[i].mass *
				(pMass[i].correctedPosition.x * pMass[i].correctedPosition.x +
					pMass[i].correctedPosition.y * pMass[i].correctedPosition.y);

			Ixy +=
				pMass[i].mass *
				(pMass[i].correctedPosition.x * pMass[i].correctedPosition.y);

			Iyz +=
				pMass[i].mass *
				(pMass[i].correctedPosition.y * pMass[i].correctedPosition.z);

			Izx +=
				pMass[i].mass *
				(pMass[i].correctedPosition.z * pMass[i].correctedPosition.x);
		}

		InertiaTensor._11 = +Ixx; InertiaTensor._12 = -Ixy; InertiaTensor._13 = -Izx;
		InertiaTensor._21 = -Ixy; InertiaTensor._22 = +Iyy; InertiaTensor._23 = -Iyz;
		InertiaTensor._31 = -Izx; InertiaTensor._32 = -Iyz; InertiaTensor._33 = +Izz;


		return InertiaTensor;
	}

	void InertiaTensor::PrintInertiaTensor(XMFLOAT3X3 InertiaTensor) {
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				std::cout << InertiaTensor.m[i][j] << "\t\t";
			}

			std::cout << std::endl;
		}
	}
}
