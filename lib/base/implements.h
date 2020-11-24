/*
FILE: implements.h
TITLE: Base Engine-IECAI Implentations
PURPOSE: Simplified 
REVISION: 6.5
*/
#define __IECAI_ENGINE_IMPLEMENT__
#include "bsc_dat_ent.h"
#include "bsc_json.h"
using namespace entbaseD;
using namespace std;
//I would recommend using custom properties with each item to provide for the 'entity&' data point.
namespace npc_actions { //These operations will not be ready for some time.
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
    bool waitMs(int milliseconds) {} //We need to find out *how* to do this.
    void triggerEvent(string what, string data) {}
    void log(string text) {}
    void logAIonly(string text) {} //*may* be moved
    void destroy(entity& obj) {}
    //void hide(entity& obj) {} //need to be done after graphics engine operational
    //void show(entity& obj) {}
}
