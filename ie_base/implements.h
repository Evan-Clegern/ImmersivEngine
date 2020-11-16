/*
FILE: implements.h
TITLE: Base Engine-IECAI Implentations
PURPOSE: Provide 'hollow functions' to interface with target game engine 
REVISION: 6
*/
#define __IECAI_ENGINE_IMPLEMENT__
#include "base_entity.h"
using namespace entbaseD;
using namespace std;
//I would recommend using custom properties with each item to provide for the 'entity&' data point.
namespace npc_actions { //WILL BE "OBSOLETED" IN FAVOR OF FILESIDE OPERATIONS.
    void updateMidpoint(entity& obj) {}
    void rotate(entity& obj, point rot) {}
    point getRotation(entity& obj) {}
    void setAnimation(entity& obj, string what) {}
    //NOTE: For below - Start at the terrain at (at), and then work it out to the maximum 'X' chunk before "curling" around
    //Like this (A to I)
    // E D C 
    // F A B    >> vector<terrain_chunk&> {A,B,C,D,E,F,G,H,I};
    // G H I
    vector<terrain_chunk&> getSurroundingTerrainSquare(point at, int rangeInChunks) {}
    vector<entity&> findSurroundingEntities(point at, int range) {}
    void storeWep(entity& weilder, entity& what) {}
    void takeOutWep(entity& weilder, entity& what) {}
    void updateStat(entity& npc, string what, float newlvl) {}
    float getStat(entity& npc, string what) {}
    void kill(entity& npc, entity& inflictor) {}
}
namespace eng_actions { //Will continue to exist here.
    bool waitMs(int milliseconds) {}
    //Engines have timing; we need this to return 'true' once the time has elapsed. Simple.
    void setEngineOValue(entity& obj, string where, string value) {}
    string getEngineOValue(entity& obj, string where) {}
    void triggerEvent(string what, string data) {}
    void log(string text) {}
    void logAIonly(string text) {} //*may* be moved
    void destroy(entity& obj) {}
    void hide(entity& obj) {}
    void show(entity& obj) {}
}
