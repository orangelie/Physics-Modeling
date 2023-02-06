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

		class NewtonTest : public PhysicsEngine {
		private:
			const float g = 9.80665f;
			const float pi = 3.14159f;

		protected:
			virtual void startup() {
				SetTitle("NewtonTest");
				SetCameraSpeed(70.0f);


				BoxOpt boxOpt2(0.0f, -5.0f, 0.0f, 1500.0f, 0.01f, 1500.0f, "white");
				Plane = box(boxOpt2);
			}

			virtual void render(float dt) {
				// Accumulation Total Times...
				static float t = 0.0f;
				t += dt;

				
			}

		private:
			// Objects & List
			std::unique_ptr<Box> Plane = nullptr;

		};
	}
}