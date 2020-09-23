/*
FILENAME: base_npc.h
TITLE: Base NPC class provider
PURPOSE: To give the fully-fledged and ready-to-behave class for an NPC.
REVISION: 1
*/
#include "base_entity.h"
#include "base_stats.h"
#include "implements.h"
using namespace std;
using namespace entbase; //from 'base_entity.h'
using namespace npc_actions; //from 'implements.h'
using namespace statSys; //from 'base_stats.h'
struct NPC_hitchunk {
	linked_point center;
	vector<linked_point> frame;
};
enum animTypes {idle, walk, run, attack, hide, use, talk, observe, death, oother};
struct NPC_animation { //Backend for 'setAnimation(entity& obj, string what)'
	string anmName;
	animTypes what;
	int animTypeID;
};
struct NPC_config {
	//Full config for NPCs; with stats, absolutely critical for behavior algorithms
	bool factionless, betrayable, agile, cautiousRelations;
	int factionID, angleRandMax, defensiveness;
};
class NPC_class { 
protected:
	entBase& sister; //The entBase class has a field to link to the address of this NPC_class
	string classname, description, modelPath, namePrefix; //namePrefix is what comes before an NPC's NPC_class child ID
	vector<NPC_hitchunk> hitAreas;
	advancedstats default_stats; //Typically, only the basicstats are needed, but use it anyway (useAdvanced below switches what to use)
	bool useAdvanced;
	NPC_config defaultData;
	vector<NPC_animation> availableAnims;
};
