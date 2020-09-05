#define __IECAI_ENGINE_IMPLEMENT__
#include "base-struct.h"
using namespace base;
//Please complete these functions with your engine's back-end for them.
//All are for the NPCs that we're working with. The int& is addresses of objects representable in the program.
//I'm from the US, but, I prefer Metric. ALL UNITS ARE METIRC! (meters, liters, kilograms, etc)
void setAnimation(int& id, const char* what) {}
void translate(int& id, point to) {}
void rotate(int& id, float degrees) {}
point getPosition(int& id) {}
list<point> getNearbyTerrain(int& id, float range) {}
list<int&> getNearbyObjects(int& id, float range) {}
list<int&> getNearbyNPCs(int& id, float range) {}