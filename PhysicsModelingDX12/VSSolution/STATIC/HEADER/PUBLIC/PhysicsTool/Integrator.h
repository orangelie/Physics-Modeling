/*
*
* < Integrator.h >
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





namespace orangelie
{

	namespace PhysicsTool
	{

		class Integrator : public PhysicsEngine
		{
		private:
			const float g = 9.80665f;
			const float pi = 3.14159f;

		private:
			void Euler(float dt, Sphere* Object)
			{
				Object->velocity += Object->acceleration * dt;
				Object->position += Object->velocity * dt;
			}

			void Rugge_Kutta(float t, float dt, Sphere* Object)
			{
				auto Integrate = [](float dt, Vector3f position, Vector3f velocity, Vector3f force, float m) -> std::tuple<Vector3f, Vector3f>
				{
					return { velocity, force / m };
				};

				auto [rk1_v, rk1_a] = Integrate(t, Object->position, Object->velocity, Object->force, Object->m);
				auto [rk2_v, rk2_a] = Integrate(t + dt / 2.0f, Object->position + (rk1_v / 2.0f) * dt, Object->velocity + (rk1_a / 2.0f) * dt, Object->force, Object->m);
				auto [rk3_v, rk3_a] = Integrate(t + dt / 2.0f, Object->position + (rk2_v / 2.0f) * dt, Object->velocity + (rk2_a / 2.0f) * dt, Object->force, Object->m);
				auto [rk4_v, rk4_a] = Integrate(t + dt, Object->position + rk3_v * dt, Object->velocity + rk3_a * dt, Object->force, Object->m);

				Object->position += dt * ((1.0f / 6.0f) * rk1_v + (1.0f / 3.0f) * rk2_v + (1.0f / 3.0f) * rk3_v + (1.0f / 6.0f) * rk4_v);
				Object->velocity += dt * ((1.0f / 6.0f) * rk1_a + (1.0f / 3.0f) * rk2_a + (1.0f / 3.0f) * rk3_a + (1.0f / 6.0f) * rk4_a);
			}

		protected:
			virtual void startup()
			{
				SetTitle("Integrator { Red: Euler, Green: Rugge-Kutta }");
				SetCameraSpeed(70.0f);

				SphereOpt sphOpt1(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, "red");
				Spheres_euler = sphere(sphOpt1);
				Spheres_euler->acceleration = { 1.5f, 0.0f, 0.0f };

				SphereOpt sphOpt2(0.0f, 10.0f, 0.0f, 1.0f, 1.0f, 1.0f, "green");
				Spheres_rk = sphere(sphOpt2);
				Spheres_rk->force = { 1.5f, 0.0f, 0.0f };
				Spheres_rk->m = 1.0f;
			}

			virtual void render(float dt)
			{
				// Accumulation Total Times...
				static float t = 0.0f;
				t += dt;

				Euler(dt, Spheres_euler.get());
				Rugge_Kutta(t, dt, Spheres_rk.get());
			}

		private:
			std::unique_ptr<Sphere> Spheres_euler, Spheres_rk;
			std::unique_ptr<Box> Plane = nullptr;

		};
	}
}