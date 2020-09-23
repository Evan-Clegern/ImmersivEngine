/*
FILENAME: base_weapon.h
TITLE: Base NPC-side Weapons handler
PURPOSE: Provide the IECAI-side support for engine weapons.
REVISION: 1
*/
#include <string>
#include "base_stats.h"
#include "base_entity.h"
using namespace std;
using namespace statSys::damage;
namespace weaponry {
	enum wepType {LAUNCHER, MELEE, THROWABLE, GUN};
	struct weapon_class {
		string title, description, firingSound, reloadSound;
		wepType classif;
		dmg toInflict;
		point& offsetDistance;
		//projectile really means the bullets/arrows/etc to spawn, at a velocity, when fired.
		int clipSize, maxAmmo, projectilePerShot;
		float velocity, reloadTime, projectileDeviationMax, projectileDeviationMin, zKickOnShot;
		bool noFalloff, noGravity, silentToNPC;
	};
	class weapon {
	protected:
		//set weilder to sisterEnt if not being held
		entity& sisterEnt, weilder;
		weapon_class& parent;
		int currentClip, currentAmmoReserve;
		point angles; //We can use the 'point' class to provide pitch/yaw/roll as well as x/y/z
	public:
		weapon(entity& sister, weapon_class& _parent, point angle) {
			sisterEnt = sister;
			weilder = sister;
			parent = _parent;
			currentClip = _parent->clipSize;
			currentAmmoReserve = _parent->maxAmmo;
			angles = angle;
		}
		weapon(entity& sister, entity& user, weapon_class& _parent, point angle, int ammReserve, int clipSize) {
			sisterEnt = sister;
			weilder = user;
			parent = _parent;
			currentClip = clipSize;
			currentAmmoReserve = ammReserve;
			angles = angle;
		}
		void pickUp(entity& _weilder) {
			weilder = _weilder;
			sisterEnt->updatePosition(_weilder->position + parent->offsetDistance, _weilder->effRotation);
			angles = weilder->effRotation;
		}
		void setDown(point& pos, point& rot) {
			weilder = sisterEnt;
			angles = rot;
			sisterEnt->updatePosition(pos, rot);
		}
	};
}
