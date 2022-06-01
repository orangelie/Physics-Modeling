/*
*
* < Cannon1.h >
* Github: https://github.com/orangelie
* // Copyright (C) 2022 by orangelie, Inc. All right reserved.
* // MIT License
*
*/




#pragma once
#include "SAMPLES/PhysicsEngine.h"

/*
* [ Object Type List ]
* 1. Box
* 2. Sphere
*
* [ Color List ]
* 1. red
* 2. green
* 3. blue
* 4. cyan
* 5. magenta
* 6. yellow
* 7. white
* 8. black
*
* [ Object Option List ]
* 1. SphereOpt
* 2. BoxOpt
*
* how to generate an object?
* > std::unique_ptr<[ObjectType]> [Name] = nullptr;
*
* how to create an sphere object option?
* > SphereOpt sphOpt(0.0f, 10.0f, 0.0f, 2.0f, 2.0f, 2.0f, "blue"); Sph1 = sphere(sphOpt);
*
*
*/





namespace orangelie {

	namespace PhysicsTool {
		
		inline float DegToRad(float degrees) {
			return (degrees * (3.14159f / 180.0f));
		}

		class Cannon1 : public PhysicsEngine {
		private:
			const float g = 9.80665f;
			const float pi = 3.14159f;

		private:
			float sx, sy, sz;				// �߻�ü�� ��ġ
			float Vm;						// �߻�ü�� �ӷ�
			float Yb;						// ������ ó�� y�߻���ġ
			float L;						// ������ ����
			float Alpha;					// xz���� ������ ���ͻ����� ��
			float Gamma;					// ���⺤�͸� xz��鿡 ������ ���ͻ����� ��

			float Length, Height, Width;	// ��ǥ���� ����, ũ��, ����
			float X, Y, Z;					// ��ǥ���� x, y, z��ǥ

			float Lx, Ly, Lz, cosX, cosY, cosZ, b;
			float xe, ze, tx1, ty1, tz1, tx2, ty2, tz2;

			void InitializeSimulation() {
				Yb = 2.0f;
				sx = 0.0f;
				sy = Yb;
				sz = 0.0f;
				Vm = 75.0f;
				L = 6.5f;
				Alpha = 25.0f;
				Gamma = 8.0f;

				Length = 6.0;
				Width = 5.0;
				Height = 8.0;
				X = 400.0;
				Y = 75.0;
				Z = 55.0;


				b = L * cos(DegToRad(90.0f - Alpha));
				Lx = b * (cos(DegToRad(Gamma)));
				Ly = L * (cos(DegToRad(Alpha)));
				Lz = b * (sin(DegToRad(Gamma)));

				cosX = Lx / L;
				cosY = Ly / L;
				cosZ = Lz / L;
			}

		protected:
			virtual void startup() {
				SetCameraSpeed(70.0f);

				InitializeSimulation();

				SphereOpt sphOpt(sx, sy, sz, 1.0f, 1.0f, 1.0f, "green");
				Sph = sphere(sphOpt);

				BoxOpt boxOpt1(X, Y, Z, Length, Height, Width, "magenta");
				Box1 = box(boxOpt1);

				BoxOpt boxOpt2(0.0f, 0.0f, 0.0f, 1500.0f, 0.01f, 1500.0f, "white");
				Plane = box(boxOpt2);
			}

			virtual void render(float dt) {
				static float t = 0.0f;

				if (Collided) return;

				if ((GetAsyncKeyState('C') & 0x8000) != 0) {
					GeoMovementAccel = false;
				}

				if ((GetAsyncKeyState('V') & 0x8000) != 0) {
					GeoMovementAccel = true;
				}

				if (GeoMovementAccel) {
					t += dt;

					xe = L * (cos(DegToRad(90.0f - Alpha)) * cos(DegToRad(Gamma)));
					ze = L * (cos(DegToRad(90.0f - Alpha)) * sin(DegToRad(Gamma)));

					sx = Vm * cosX * t + xe;
					sy = (Yb + L * cos(DegToRad(Alpha))) + (Vm * cosY * t) - (0.5f * g * t * t);
					sz = Vm * cosZ * t + ze;

					Sph->position = { sx, sy, sz };

					tx1 = X - (0.5f * Length);
					tx2 = X + (0.5f * Length);
					ty1 = Y - (0.5f * Height);
					ty2 = Y + (0.5f * Height);
					tz1 = Z - (0.5f * Width);
					tz2 = Z + (0.5f * Width);

					if ((sx >= tx1 && sx <= tx2) && (sy >= ty1 && sy <= ty2) && (sz >= tz1 && sz <= tz2)) {
						MessageBoxA(0, "��ü�� �ε�ħ", "", MB_OK);
						Collided = true;
					}

					if (sy <= 0.0f) {
						MessageBoxA(0, "�ٴڿ� �ε�ħ", "", MB_OK);
						Collided = true;
					}
				}
			}

		private:
			std::unique_ptr<Sphere> Sph = nullptr;
			std::unique_ptr<Box> Box1 = nullptr;
			std::unique_ptr<Box> Plane = nullptr;

			bool GeoMovementAccel = false;
			bool Collided = false;

		};
	}
}