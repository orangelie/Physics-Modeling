#pragma once

#include "HEADER/PUBLIC/PhysicsTool/PhysicsInitializer.h"
#include "HEADER/PUBLIC/PhysicsTool/LinearMotion.h"
#include "HEADER/PUBLIC/PhysicsTool/Cannon1.h"
#include "HEADER/PUBLIC/PhysicsTool/Hanabi.h"
#include "HEADER/PUBLIC/PhysicsTool/Integrator.h"


#if not defined(PHYSICSINIT)
#define PHYSICSINIT orangelie::PhysicsTool::Integrator()
#endif