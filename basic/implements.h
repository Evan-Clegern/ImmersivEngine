/*
FILE: implements.h
TITLE: Base Engine-IECAI Implentations
PURPOSE: Provide 'hollow functions' to interface with target game engine.
REVISION: 4
*/
#define __IECAI_ENGINE_IMPLEMENT__
#include "base_entity.h"
using namespace entbase;
using namespace std;
//I would recommend using custom properties with each item to provide for the 'entity&' data point.
namespace npc_actions {
    void updateMidpoint(entity& obj) {}
    void rotate(entity& obj, point rot) {}
    point getRotation(entity& obj) {}
    void setAnimation(entity& obj, string what) {}
    terrain_chunk getSurroundingTerrain(point at) {}
    vector<entity&> findSurroundingEntities(point at, int range) {}
    void storeWep(entity& weilder, entity& what) {}
    void takeOutWep(entity& weilder, entity& what) {}
}
namespace eng_actions {
    bool waitMs(int milliseconds) {}
    //Engines have timing; we need this to return 'true' once the time has elapsed. Simple.
    void setEngineOValue(entity& obj, string where, string value) {}
    string getEngineOValue(entity& obj, string where) {}
    void triggerEvent(string what, string data) {}
    void log(string text) {}
    void hide(entity& obj) {}
    void show(entity& obj) {}
}
