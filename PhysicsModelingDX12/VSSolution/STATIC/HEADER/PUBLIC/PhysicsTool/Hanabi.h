/*
*
* < Hanabi.h >
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

		class Hanabi : public PhysicsEngine {
		private:
			 const float g = 9.80665f;
			 const float pi = 3.14159f;

		private:
			auto GetRandom(int u1, int u2) -> int {
				std::random_device device;
				std::mt19937_64 engine(device());
				std::uniform_int_distribution uniformer(u1, u2);

				return std::bind(uniformer, engine)();
			}

			auto GetRandom(FLOAT f1, FLOAT f2) -> FLOAT {
				std::random_device device;
				std::mt19937_64 engine(device());
				std::uniform_real_distribution uniformer(f1, f2);

				return std::bind(uniformer, engine)();
			}

		protected:
			virtual void startup() {
				SetTitle("Hanabi");
				SetCameraSpeed(70.0f);


				m_Colors[0] = "red"; m_Colors[1] = "green"; m_Colors[2] = "blue"; m_Colors[3] = "cyan";
				m_Colors[4] = "magenta"; m_Colors[5] = "yellow"; m_Colors[6] = "white"; m_Colors[7] = "black";

				ParticlePositions.resize(ElementAmount);
				for (size_t i = 0; i < ParticlePositions.size(); ++i) {
					ParticlePositions[i] = { 0.0f, 4.0f, 0.0f };
				}

				Spheres.resize(ElementAmount);
				for (size_t i = 0; i < Spheres.size(); ++i) {
					SphereOpt sphOpt(ParticlePositions[i].x, ParticlePositions[i].y, ParticlePositions[i].z, 1.0f, 1.0f, 1.0f, m_Colors[GetRandom(0, 7)]);
					Spheres[i] = sphere(sphOpt);
					Spheres[i]->velocity = { vi.x, vi.y, vi.z };
				}

				BoxOpt boxOpt2(0.0f, -5.0f, 0.0f, 1500.0f, 0.01f, 1500.0f, "white");
				Plane = box(boxOpt2);
			}

			virtual void render(float dt) {
				// Accumulation Total Times...
				static float t = 0.0f;
				t += dt;


				if (t > 1.0f && !explosion) {
					explosion = true;

					for (size_t i = 0; i < Spheres.size(); ++i) {
						Spheres[i]->velocity += { GetRandom(0.0f, 5.0f) - 2.5f, GetRandom(0.0f, 5.0f) - 2.5f , GetRandom(0.0f, 5.0f) - 2.5f };
					}
				}

				for (size_t i = 0; i < Spheres.size(); ++i) {
					Spheres[i]->velocity += a * dt;
					Spheres[i]->position += Spheres[i]->velocity * dt;

					if (Spheres[i]->position.y < Plane->position.y + Spheres[i]->GetScale().y) {
						Spheres[i]->position.y = Plane->position.y + Spheres[i]->GetScale().y;
						Spheres[i]->velocity.y = -0.8f * Spheres[i]->velocity.y;
					}
				}
			}

		private:
			// Objects & List
			std::unordered_map<UINT, std::string> m_Colors;
			std::vector<std::unique_ptr<Sphere>> Spheres;
			std::vector<DirectX::XMFLOAT3> ParticlePositions;

			std::unique_ptr<Box> Plane = nullptr;

			// Element
			const UINT ElementAmount = 200;
			DirectX::XMFLOAT3 vi = { 0.0f, 15.0f, 0.0f };
			Vector3f a = { 0.0f, -9.81f, 0.0f };
			bool explosion = false;

		};
	}
}