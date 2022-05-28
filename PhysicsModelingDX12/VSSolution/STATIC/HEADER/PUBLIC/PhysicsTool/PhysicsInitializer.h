/*
*
* < PhysicsIntializer.h >
* Github: https://github.com/orangelie
* // Copyright (C) 2022 by orangelie, Inc. All right reserved.
* // MIT License
*
*/





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

		class PhysicsIntializer : public PhysicsEngine {
		protected:
			virtual void startup() {
				SetCameraSpeed(10.0f);

				SphereOpt sphOpt(0.0f, 10.0f, 0.0f, 2.0f, 2.0f, 2.0f, "yellow");
				Sph1 = sphere(sphOpt);

				BoxOpt boxOpt(0.0f, 5.0f, 0.0f, 2.0f, 2.0f, 2.0f, "magenta");
				Box1 = box(boxOpt);
			}

			virtual void render(float dt) {

			}

		private:
			std::unique_ptr<Sphere> Sph1 = nullptr;
			std::unique_ptr<Box> Box1 = nullptr;

		};
	}
}