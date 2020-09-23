/*
FILE: base_stats.h
TITLE: Base NPC Stat System Provider
PURPOSE: Provide bases for health and other statistics involved in NPC actions
REVISION: 4
*/
//Note: all distance-like stats are in POINTS.
namespace statSys {
	struct basicstats {
		float health, stamina, speed, jumpHeight, cleverness, riskiness, sightRange, moodLevel, sleepiness, accuracy;
		//Cleverness, SightRange and Riskiness are CRITICAL for NPC behavior algorithms.
		int typeResist[11]; //Resistances to specific damage types, simplified to an integer.
	};
	struct advancedStats {
		//For more open-world environments; optional.
		basicstats base;
		float hunger, thirst, weight, hgrRegenDrain, thrRegenDrain, regenPerSec, hearingRange, smellingRange;
	};
	namespace damage {
		enum dmgType {BLUNT, SHARP, GUNSHOT, EXPLOSIVE, HEAT, FIRE, COLD, ACID, TOXIN, BIOHAZARD, RADIATION, SHOCK};
		struct dmg { //Non-targeted damage; more of a placeholder/template, especially for weapons.
			dmgType type;
			float amount;
			bool critical;
		};
		struct damage : dmg { //Targeted damage
			basicstats& inflictor, victim;
		};
		struct debuff {
			dmg& onTickA, onTickB, onTickC;
			float tickPerSecond, stamDrain, resistDrain, speedDrain, sleepAdd, moodDrain;
			int secondsLength;
			bool curable, lethal, instaRestore;
			//iF not lethal, victim HP won't drop to 0. instaRestore puts *drained* stats back to full once done
		};
		void inflictDamage(damage data) { //Runs calculations to inflict the damage to the victim.
			float dResist = 1 - (data.victim->typeResist[data.type] / 100); //Percentile steps of the damage type to ignore.
			float dmg = data.amount;
			if (data.critical) {
				dResist=1 - (data.victim->typeResist[data.type] / 225); //Resistance is effectively MUCH lower
				dmg*=1.1; //10% stronger atop reduced resistance
			}
			data.victim->health-=(dmg * dResist);
		}
		void inflictDmg(dmg damage, basicstats& victim, basicstats& inflictor) {
			damage d;
			d.victim = victim;
			d.inflictor = inflictor;
			inflictDamage(d);
		}
	}
}
