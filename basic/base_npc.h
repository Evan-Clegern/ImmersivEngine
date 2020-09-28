/*
FILENAME: base_npc.h
TITLE: Base NPC class provider
PURPOSE: To give the fully-fledged and ready-to-behave class for an NPC.
REVISION: 13
NOTE: I apologize for the complexity of the data for NPCs, but, it's for the immersive-ness (is that a word?)
*/
#include "base_entity.h"
#include "base_stats.h"
#include "implements.h"
#include "base_weapon.h"
using namespace std;
using namespace entbase; //from 'base_entity.h'
using namespace statSys; //from 'base_stats.h'
using namespace weaponry; //from 'base_weapon.h'
using namespace npc_actions;
using namespace eng_actionsl //both from 'implements.h'
namespace NPC_BASE {
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
	struct NPC_vocal { //Face-only animation for an Idle. Much simpler!
		string voicelinePath, voicelineAnim;
		emoteTypeps eType;
		float lengthSec;
	};
	//NPCs are disallowed to weild multiple weapons; limit how many weapons they can store!
	struct NPC_action { //Queueable actions for an NPC - reaction to getting hit, shooting, being scared, etc.
		NPC_animation& anim;
		NPC_vocal& voiceline;
		entity& newTarget; //The NPC will play their animation (like 'attack') and fire @/hit this entity
		weapon& newWeapon;
		squadMode newMode;
		emoteTypes newEmote;
		//swapWeapon, weaponIsStored = get 'newWeapon' from NPC's inventory. WILL CAUSE AN ERROR IF IT DOESNT EXIST.
		bool noAnim, reloadWeapon, swapWeapon, dropWeapon, noAttack, noChangeRole, noChangeEmote;
		bool targisMedical, targisAmmo, weaponIsStored, targIsGoal, noVoiceline, voiceBefore;
		int msToWaitBf, msToWaitAft;
	};
	NPC_action weaponSwap(bool isInventory, bool dropCurrent, bool reloadFirst, NPC_animation& switchAnim, weapon& switchTo, NPC_vocal& voice, bool noVoice) {
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
		d.noVoiceline = noVoice;
		d.targisMedical = false;
		d.targisAmmo = false;
		d.voiceline = voice;
		d.voiceBefore = true;
		d.noChangeRole = true;
		d.noChangeEmote = true;
		d.weaponIsStored = isInventory;
		d.targIsGoal = false;
		return d;
	}
	struct NPC_config {
		//Full config for NPCs; with stats, absolutely critical for behavior algorithms. Most of these are constant (ones that arent have a NC)
		bool factionless, betrayable, NCagile, cautiousRelations, blankEmoteOnly, noMove, heavyDuty, NCtactical, ambusher, NCignoreCurios, NCoblivious;
		//tactical NPCs aim 15% worse, but, they help coordinate squadmates and/or make plans for themselves for ambush or escape
		//oblivious NPCs pay no attention to hazards in the world around them (fire, enemies, etc.)
		bool NChasWeapon, distractable, observant, skiddish, inaccurateFirstAtks, forceSquads, NCpeaceful, NCalone, constSquadMode, giveChase, aggressiveChasing;
		int NCfactionID, angleAimMax, NCdefensiveness, msSpotTime, storableWeapons, maxQueueSize;
		emoteTypes emotion;
		weapon& equippedWeapon; //Put to some random thing if (hasWeapon == false)
	};
	//Has the 'unarmed' and "blank" animations separate, and then a vector of special ones
	struct NPC_anim_array {
		NPC_animation idleUn, walkUn, runUn, hideUn, useUn, talkUn, observeUn, deathUn, turnWUn, turnSUn, getHitUn, fleeUn;
		vector<NPC_animation> other_anim;
		vector<NPC_animation&> indexFormsOf(emoteType emotion, animTypes type, squadMode squad, weapon_class& wep, bool noWep, bool noSquad, bool wepPointDown) {
			//Finds how many animations meet that specification (not including the unarmed, blank ones)
			vector<NPC_animation&> temp;
			for (int i = 0; i < other_anwim.size() - 1; i++) {
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
				if (applicable) {temp.push_back(ju);}
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
	struct NPC_miniroutine { 
		/*NPC Mini-routine
		FORMAT:
		 Start with an action
		 Observe using one or more methods
		 Play a final action + return observed data		
		*/
		NPC_action start, final;
		bool see, smell; //hearingRange is more of a triggered thing.
		vector<entity&> runObserve(NPC_config& data, point pnt, int observeMs) { //This is a tedious function, but one which is CRITICAL!
			//It gets the obscuration per slice of terrain. If the entity is in an obscured area, they won't be seen.
			vector<entity&> _data;
			vector<terrain_chunk> terrainNearby = getSurroundingTerrain(pnt, data->sightRange);
			if (see) {
				vector<entity&> soup;
				soup = findSurroundingEntities(pnt, data->sightRange);
				float d = data->sightRange;
				for (int i = 0; i < soup.size() -1; i++) {
					waitMs(observeMs);
					//TODO: Implement viewing
					//This includes utilizing entities' height and the obscuration of the area.
					//Will need the 'getSurroundingTerrainSquare(point pnt, int range)
					//As well as obscuration calculations
				}
			}
			waitMs(observeMs * 2);
			if (smell) {
				vector<entity&> smells;
				smells = findSurroundingEntities(pnt, data->smellRange);
				for (int i = 0; i < smells.size() - 1; i++) {
					entity& b = smells.at(i);
					waitMs(observeMs);
					if (b->smellable) {
						int str = b->smellStrength;
						//Use linear distance of our smell range
						int range = data->smellRange;
						float distance = b->position >> pnt;
						if (not (distance > range)) {
							float d = 100 - (range / distance);
							if (str >= d) {
								//NPC can smell the object
								_data.push_back(b);
							}
						}
					}
				}
			}
		}
	};
	struct NPC_routine {
		/*NPC Routine.
		FORMAT:
		 Start with an action and wait
		 Observe using one or more methods
		 Play a sequence of in-between actions
		 Change NPC configurations and play a voiceline (if wanted)
		 Replace a NPC relationship
		 Play a final action + return observed data
		*/
		NPC_action start
		int waitMs;
		vector<NPC_action> middleActions;
		//Toggles or newEmotions happen after the middleActions
		bool rtDistracts, rtAngers, rtScares, rtMoves, rtChases, rtBetrays, rtAttacks, rtHelps, rtUses, rtAmbushes, rtNoVoiceline;
		//^^ are for helping say "what does this routine do?"
		NPC_vocal laterVoiceline; //To ay as options change
		int newSquad, newDefensiveness, newFactionID;
		emoteType emotionNew;
		squadMode modeNew;
		NPC_relationship& old, replaceWith;
		bool togAgile, togTactical, togCurioIgnore, togOblivious, togPeaceful;
		NPC_action final; //What happens after the options change.
	};
	struct NPC_behavior_minitree {

	};
}
