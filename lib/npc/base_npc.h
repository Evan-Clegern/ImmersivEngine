/*
FILENAME: base_npc.h
TITLE: Base NPC class provider
PURPOSE: Providing memory- and filesystem-side NPC class definitions, as well as related file operations.
REVISION: 17
NOTE: I apologize for the complexity of the data for NPCs, but, it's for the immersive-ness (is that a word?)
*/
#include "base_entity.h"
#include "base_stats.h"
#include "implements.h"
#include "base_weapon.h"
#include <cmath>
#include <fstream>
using namespace std;
using namespace entbase; //from 'base_entity.h'
using namespace statSys; //from 'base_stats.h'
using namespace weaponry; //from 'base_weapon.h'
using namespace npc_actions;
using namespace eng_actions; //both from 'implements.h'
/*
Temporary files will be generated for each NPC during runtime as well (reduce memory consumption)

*.iecai-acts is for multiple NPC_vocal s/NPC_action s  - ACTION FILE
*.iecai-anm is for NPC_anim_array (which has direct storages for NPC_animation s)  - ANIMATION FILE
*.iecai-cls is for NPC_Class; NPC_Config; NPC_Relationship  - CLASS FILE
*.iecai-bhv is for NPC_miniroutine; NPC_routine; NPC_behavior_minitree  - BEHAVIOR FILE

*.iecai-npc is for the actual NPC (defined beyond /basic/)
*/
namespace NPC_BASE_T1D {
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
		bool NChasWeapon, distractable, observant, skiddish, inaccurateFirstAtks, forceSquads, NCpeaceful, NCalone, constSquadMode, giveChase;
		bool aggressiveChasing, crouchLowVis; 
		//crouchLowVis is for anything with the 'crouching' flag in its sight range. Reduces visibility of obj by 50%. Lets the player cheat subtly if true.
		int NCfactionID, angleAimMax, NCdefensiveness, msSpotTime, storableWeapons, maxQueueSize, height;
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
}
namespace NPC_BASE_T1F { 
	//File functions for NPC_BASE_T1D(ata)
}
namespace NPC_BASE_T2D {
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
	vector<entity&> runObserve(NPC_config& data, point pnt, float rotZ, int observeMs, float sightRange, float smellRange) {
		//This is a tedious function, but one which is CRITICAL!
		//It gets the obscuration per slice of terrain. If the entity is in an obscured area, they won't be seen.
		vector<entity&> _data;
		vector<terrain_chunk> terrainNearby = getSurroundingTerrainSquare(pnt, data->sightRange);
		if (see) {
			//Sight range is 96 degrees (human peripheral vision)
			int ofst = 48;
			float d = sightRange
			if (data->observant) {
				ofst+=7;
				d+=20;
			} else if (data->NCoblivious) {
				ofst-=10;
				d-=5;
			} ellse if (data->tactical) {
				ofst+=5;
			}
			float min = (ofst - rotZ);
			float max = (ofst + rotZ);
			bool xXneg, xYneg, nXneg, nYneg;
			//These are all done within a circle.
			//Sets which things to put as negative after sine/cosine calculations
			if (min > 0) {
				nXneg = false; nYneg = false;
			} else if (min > -90) {
				nXneg = false; nYneg = true; min+=90;
			} else if (min > -180) {
				nXneg = true; nYneg = true; min+=180;
			} else if (min > -270) {
				nXneg = true; nYneg = false; min+=270;
			} else if (min > -360) {
				nXneg = false; nYneg = false; min+=360;
			}
			if (max > 360) {
				xXneg = false; xYneg = false; max-=360;
			} else if (max > 270) {
				xXneg = true; xYneg = false; max-=270;
			} else if (max > 180) {
				xXneg = true; xYneg = true; max-=180;
			} else if (max > 90) {
				xXneg = false; xYneg = true; max-=90;
			} else {
				xXneg = false; xYneg = true;
			}
			min = (min * 3.14159) / 180; //functions are in radians
			max = (max * 3.14159) / 180;
			//150* arc of vision, with a distance at any point being NPC's sightRange
			float MINY = sin(min) * d; //Multiply (o/h) by h, get o - our Y
			float MINX = cos(min) * d;
			float MAXY = sin(max) * d;
			float MAXX = cos(max) * d;
			if (xXneg) {MAXX * -1;}
			if (xYneg) {MAXY * -1;}
			if (nXneg) {MINX * -1;}
			if (nYneg) {MINY * -1;}
			vector<entity&> soup;
			waitMs(observeMs);
			soup = findSurroundingEntities(pnt, data->sightRange);
			point myEyes = pnt ++ data->height;
			for (int i = 0; i < soup.size() -1; i++) {
				waitMs(ceil(observeMs/2));
				//Viewing is dependent on the angle of the NPC; smelling is not
				point objectEyes = soup.at(i)->position ++ soup.at(i)->height;
				if ((objectEyes.x > MINX) and (objectEyes.x < MAXX) and (objectEyes.y > MINY) and (objectEyes.y < MAXY)) {
					logAIonly("Object " + to_string((int)soup.at(i)) + " is in sightRange, also in view arc");
					float dist = myEyes >> objectEyes;
					//Their X,Y of their 'height' is in our view range.
					//TODO: GET OBSCURATION EN-ROUTE AND THE HEIGHT OF TERRAIN!
					//FAIL IF:
					//if average obscuration along the way > maxObscuration
					//if obscuration of target slice > maxObscuration
					//if height of object < height of any terrain along the way
					float obscur = 50.00; //Our max obscuration
					if (data->observant) {
						obscur+=20.50;
					} else if (data->NCoblivious) {
						obscur-=25.75;
					} else if (data->tactical) {
						obscur+=8.35;
					}
					float slopeTL = (objectEyes.y - myEyes.y) / (objectEyes.x - myEyes.x);
					//slopeTL is for a line on the Terrain to determine which terrainChunks to use.
					bool lowCrouch = data->crouchLowVis;
					bool isCrouched = soup.at(i)->isCrouched;
					//surroundingTerrain (in vector terrainNearby) is in a square
					bool allowed = true;
					for (int _id = 0; _id < terrainNearby.size() - 1; _id++) {
						//chunks are 75x75 of points; 15x15 of slices
						//allowed = false; break; for this, if obscuration level too high
					}
					if (not allowed) {
						continue;
					}
				} else {
					logAIonly("Object " + to_string((int)soup.at(i)) + " is in sightRange, not in view arc");	
				}
			}
		}
		waitMs(observeMs);
		if (smell) {
			vector<entity&> smells;
			smells = findSurroundingEntities(pnt, smellRange);
			for (int i = 0; i < smells.size() - 1; i++) {
				entity& b = smells.at(i);
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
	struct NPC_miniroutine { 
		/*NPC Mini-routine
		FORMAT:
		 Start with an action
		 Observe using one or more methods
		 Play a final action + return observed data		
		*/
		NPC_action start, final;
		bool see, smell; //hearingRange is more of a triggered thing.
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
