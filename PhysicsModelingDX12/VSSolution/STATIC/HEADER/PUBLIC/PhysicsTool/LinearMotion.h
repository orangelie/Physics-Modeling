/*
*
* < LinearMotion.h >
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

		class LinearMotion : public PhysicsEngine {
		protected:
			virtual void startup() {
				SetCameraSpeed(30.0f);

				SphereOpt sphOpt1(0.0f, 1.0f, 0.0f, 2.0f, 2.0f, 2.0f, "yellow");
				Sph1 = sphere(sphOpt1);
				Sph1->velocity = { 1.0f, 0.0f, 0.0f };

				SphereOpt sphOpt2(0.0f, 3.0f, 0.0f, 2.0f, 2.0f, 2.0f, "cyan");
				Sph2 = sphere(sphOpt2);
				Sph2->acceleration = { 1.0f, 0.0f, 0.0f };

				BoxOpt boxOpt(0.0f, 0.0f, 0.0f, 100.0f, 0.01f, 100.0f, "magenta");
				Plane = box(boxOpt);
			}

			virtual void render(float dt) {
				static float t = 0.0f;
				t += dt;

				if ((GetAsyncKeyState('C') & 0x8000) != 0) {
					GeoMovementAccel = false;
				}

				if ((GetAsyncKeyState('V') & 0x8000) != 0) {
					GeoMovementAccel = true;
				}

				if (GeoMovementAccel) {
					Sph1->position = Sph1->position + Vector3f{ Sph1->velocity.x * dt, Sph1->velocity.y * dt, Sph1->velocity.z * dt };

					Sph2->velocity = Sph2->velocity + (Sph2->acceleration * dt);
					Sph2->position = Sph2->position + Vector3f{ Sph2->velocity.x * dt, Sph2->velocity.y * dt, Sph2->velocity.z * dt };
				}
			}

		private:
			std::unique_ptr<Sphere> Sph1 = nullptr;
			std::unique_ptr<Sphere> Sph2 = nullptr;

			std::unique_ptr<Box> Plane = nullptr;

			bool GeoMovementAccel = false;

		};
	}
}