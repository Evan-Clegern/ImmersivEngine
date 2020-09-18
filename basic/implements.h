/*
FILE: implements.h
TITLE: Base Engine-IECAI Implentations
PURPOSE: Provide 'hollow functions' to interface with target game engine.
REVISION: 3
*/
#define __IECAI_ENGINE_IMPLEMENT__
#include "base_entity.h"
using namespace base; //Reference the Base Structure Provider
using namespace std;
//I would recommend using custom properties with each item to provide for the 'entity&' data point.
void updateMidpoint(entity& obj) {}
void setAnimation(entity& obj, string what) {}
terrain_chunk getSurroundingTerrain(point at) {}
vector<entity&> findSurroundingEntities(point at, int range) {}
void setEngineOValue(entity& obj, string where, string value) {}
string getEngineOValue(entity& obj, string where) {}
void triggerEvent(string what, string data) {}
