/*
FILENAME: base_npc.h
TITLE: Base NPC class provider
PURPOSE: To give the fully-fledged and ready-to-behave class for an NPC.
REVISION: 1
*/
#include "base_entity.h"
#include "base_stats.h"
#include "base_weapon.h"
#include "implements.h"
using namespace std;
using namespace entbase; //from 'base_entity.h'
using namespace npc_actions; //from 'implements.h'
using eng_actions::waitMs; //from 'implements.h'
using namespace statSys; //from 'base_stats.h'
using namespace weaponry; //from 'base_weapon.h'
struct NPC_hitchunk {
	linked_point center;
	vector<linked_point> frame;
};
//turn Stnry = stationary
enum animTypes {idle, walk, run, attack, hide, use, talk, observe, death, turnWalk, turnStnry, getHit, flee, reload, other};
enum emoteTypes {calm, cautious, annoyed, angry, sad, tired, happy, terrified, scared, hurt, serious, blank};
enum squadMode {attacker, scout, medic, pickOff, defender}; //tactical can make these 'different.'
struct NPC_animation { //Backend for 'setAnimation(entity& obj, string what)'
	string anmEngName, anmTitle, dlgName; //What the NPC says when anim is played. Leave blank for none. (even for wolves making barks)
	animTypes aType;
	emoteTypes eType;
	float lengthSec;
	int animTypeID, keyframes;
	bool forWeapon, weaponPntDown;
	//The animation (especially attack/reload) is affected by a weilded weapon.
	//Set to some random one and 'forWeapon' to false if it's an unarmed animation
	weapon_class& whileWeilding;
};
struct NPC_config {
	//Full config for NPCs; with stats, absolutely critical for behavior algorithms. Most of these are constant (ones that arent have a NC)
	bool factionless, betrayable, NCagile, cautiousRelations, blankEmoteOnly, noMove, heavyDuty, NCtactical, ambusher;
	//tactical NPCs aim 15% worse, but, they help coordinate squadmates and/or make plans for themselves for ambush or escape
	bool NChasWeapon, distractable, observant, skiddish, inaccurateFirstAtks, forceSquads, NCpeaceful, NCalone, constSquadMode;
	int NCfactionID, angleAimMax, NCdefensiveness, msSpotTime;
	emoteTypes emotion;
	weapon& equippedWeapon; //Put to some random thing if (hasWeapon == false)
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
