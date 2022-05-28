#pragma once

#include "HEADER/PUBLIC/PhysicsTool/PhysicsInitializer.h"
#include "HEADER/PUBLIC/PhysicsTool/LinearMotion.h"

#if not defined(PHYSICSINIT)
#define PHYSICSINIT orangelie::PhysicsTool::LinearMotion()
#endif