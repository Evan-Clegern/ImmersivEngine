/*
FILENAME: base_npc.h
TITLE: Base NPC class provider
PURPOSE: To give the fully-fledged and ready-to-behave class for an NPC.
REVISION: 2
NOTE: I apologize for the complexity of the data for NPCs, but, it's for the immersive-ness (is that a word?)
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
enum squadMode {attacker, scout, medic, pickOff, defender, none}; //tactical can make these 'different.'
struct NPC_animation { //Backend for 'setAnimation(entity& obj, string what)'
	string anmEngName, anmTitle, dlgName; //What the NPC says when anim is played. Leave blank for none. (even for wolves making barks)
	animTypes aType;
	emoteTypes eType;
	squadMode sType; //IF APPLICABLE
	float lengthSec;
	vector<vector<NPC_hitchunk>> frameByFrameHitbox;
	int animTypeID, keyframes;
	bool forWeapon, weaponPntDown, squadApplicabale;
	//The animation (especially attack/reload) is affected by a weilded weapon.
	//Set to some random one and 'forWeapon' to false if it's an unarmed animation
	weapon_class& whileWeilding;
};
//NPCs are disallowed to weild multiple weapons; limit how many weapons they can store!
struct NPC_action { //Queueable actions for an NPC - reaction to getting hit, shooting, being scared, etc.
	NPC_animation& anim;
	entity& newTarget; //The NPC will play their animation (like 'attack') and fire @/hit this entity
	weapon& newWeapon;
	squadMode newMode;
	emoteTypes newEmote;
	//swapWeapon, weaponIsStored = get 'newWeapon' from NPC's inventory. WILL CAUSE AN ERROR IF IT DOESNT EXIST.
	bool noAnim, reloadWeapon, swapWeapon, dropWeapon, noAttack, noChangeRole, noChangeEmote, weaponIsStored;
	int msToWaitBf, msToWaitAft;
};
NPC_action weaponSwap(bool isInventory, bool dropCurrent, bool reloadFirst, NPC_animation& switchAnim, weapon& switchTo) {
	NPC_action d;
	d.anim = switchAnim;
	d.newTarget = switchTo->sisterEnt;
	d.newWeapon = switchTo;
	d.newMode = squadMode.none;
	d.newEmote = emoteTypes.blank;
	d.noAnim = false;
	d.reloadWeapon = reloadFirst;
	d.swapWeapon = true;
	d.dropWeapon = dropCurrent;
	d.noAttack = true;
	d.noChangeRole = true;
	d.noChangeEmote = true;
	d.weaponIsStored = isInventory;
	return d;
}
struct NPC_config {
	//Full config for NPCs; with stats, absolutely critical for behavior algorithms. Most of these are constant (ones that arent have a NC)
	bool factionless, betrayable, NCagile, cautiousRelations, blankEmoteOnly, noMove, heavyDuty, NCtactical, ambusher, NCignoreCurios, NCoblivious;
	//tactical NPCs aim 15% worse, but, they help coordinate squadmates and/or make plans for themselves for ambush or escape
	//oblivious NPCs pay no attention to hazards in the world around them (fire, enemies, etc.)
	bool NChasWeapon, distractable, observant, skiddish, inaccurateFirstAtks, forceSquads, NCpeaceful, NCalone, constSquadMode, noDescalateCombat;
	int NCfactionID, angleAimMax, NCdefensiveness, msSpotTime, storableWeapons, maxQueueSize;
	emoteTypes emotion;
	weapon& equippedWeapon; //Put to some random thing if (hasWeapon == false)
};
//Has the 'unarmed' and "blank" animations separate, and then a vector of special ones
struct NPC_anim_array {
	NPC_animation idleUn, walkUn, runUn, hideUn, useUn, talkUn, observeUn, deathUn, turnWUn, turnSUn, getHitUn, fleeUn;
	vector<NPC_animation> other_anim;
	int indexFormsOf(emoteType emotion, animTypes type, squadMode squad, weapon_class& wep, bool noWep, bool noSquad, bool wepPointDown) {
		//Finds how many animations meet that specification (not including the unarmed, blank ones)
		int temp = 0;
		for (int i = 0; i < other_anim.size() - 1; i++) {
			NPC_animation& ju = &(other_anim.at(i));
			bool weap, squad, applicable = ju->forWeapon, ju->squadApplicable, false;
			if ((ju->whileWeilding == wep) and (noWep != weap) and (wepPointDown == ju->weaponPntDown)) {
				applicable = true;
			} else {
				applicable = false;
			}
			if ((ju->sType == type) and (noSquad != squad)) {
				applicable = true;
			} else {
				applicable = false;
			}
			if (ju->aType == type) {
				applicable = true;
			} else {
				applicable = false;
			}
			if (ju->eType == emotion) {
				applicable = true;
			} else {
				applicable = false;
			}
			if (applicable) {temp++;}
		}
		return temp;
	}
};
class NPC_class { 
protected:
	entBase& sister; //The entBase class has a field to link to the address of this NPC_class
	string classname, description, namePrefix; //namePrefix is what comes before an NPC's NPC_class child ID
	vector<NPC_hitchunk> hitAreas;
	advancedstats default_stats; //Typically, only the basicstats are needed, but use it anyway (useAdvanced below switches what to use)
	bool useAdvanced;
	NPC_config defaultData;
	NPC_anim_array availableAnims;
public:
	NPC_class(entBase& _s, string name, string desc, string pref, vector<NPC_hitchunk> hitarea, vector<NPC_animation> anims, NPC_config config, advancedstats stats) {
		for (int i = 0; i < hitarea.size() - 1; i++) {
			hitAreas.push_back(hitarea.at(i));
		}
		for (int i = 0; i < anims.size() - 1; i++) {
			availableAnims.push_back(anims.at(i));
		}
		useAdvanced = false;
		default_stats = stats;
		sister = _s;
		classname = name;
		description = desc;
		namePrefix = pref;
		defaultData = config;
	}
	void toggleAdvancedStats() {
		if (useAdvanced) {
			useAdvanced = false;
		} else {
			useAdvanced = true;
		}
	}
};
enum NPC_relations {hate, dislike, neutral, like, love};
struct NPC_relationship {
	NPC_relations level;
	NPC_class& from, to;
	bool reciprocal, fearrelation, actLikeSquadmates;
};
class NPC {
	vector<NPC_action> actionQueue;
	int panicLevel, timesHit, timesAttacked, timesFled, timesDefended, timesHelped;
	vector<entity&> squad, nearbyGood, nearbyEnemy, nearbyOther;
	vector<weapon&> weaponInventory;
protected:
	entity& sister;
	NPC_class& parent;
	advancedstats stats;
	entBase& stepparent;
	NPC_config config;
	terrain_slice terrainPos;
	point truePos;
	vector<NPC_relationship> relations;
public:
	bool queueAction(NPC_action new) {
		if (this.actionQueue.size() > this.config.maxQueueSize) {
			return false;
		}
		try {
			this.actionQueue.push_back(new);
		} catch {
			return false;
		}
		return true;
	}
	bool moveTo(point to_true, terrain_slice to_terrain, bool run) {
		
	}
};
