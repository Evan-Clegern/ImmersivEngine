/*
FILE: implements.h
TITLE: Base Engine-IECAI Implentations
PURPOSE: Provide 'hollow functions' to interface with target game engine.
REVISION: 3
*/
#define __IECAI_ENGINE_IMPLEMENT__
#include "base_structure.h"
using namespace base; //Reference the Base Structure Provider
void setAnimation(int& id, const char* what) {}
void translate(int& id, point to) {}
void rotate(int& id, float degrees) {}
point getPosition(int& id) {}
list<point> getNearbyTerrain(int& id, float range) {}
list<int&> getNearbyObjects(int& id, float range) {}
list<int&> getNearbyNPCs(int& id, float range) {}
