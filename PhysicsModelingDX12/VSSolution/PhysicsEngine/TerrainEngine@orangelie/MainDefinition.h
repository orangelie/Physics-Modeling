#pragma once

#include "HEADER/PUBLIC/PhysicsTool/PhysicsInitializer.h"
#include "HEADER/PUBLIC/PhysicsTool/LinearMotion.h"
#include "HEADER/PUBLIC/PhysicsTool/Cannon1.h"
#include "HEADER/PUBLIC/PhysicsTool/Hanabi.h"
#include "HEADER/PUBLIC/PhysicsTool/Integrator.h"
#include "HEADER/PUBLIC/PhysicsTool/NewtonTest.h"
#include "HEADER/PUBLIC/PhysicsTool/RigidbodySim.h"


#if not defined(PHYSICSINIT)
#define PHYSICSINIT orangelie::PhysicsTool::RigidbodySim()
#endif