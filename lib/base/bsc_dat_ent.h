/*
FILE: bsc_dat_ent.h
TITLE: Basic Data and Entity Definitionsh
PURPOSE: Provides format bases for entities
This File Has Been Debugged Fully (branch testing-nov2020)
VERSION: 27
*/
#define __IMVENG_BASE_ENTITY_DEF
#include <vector>
#include <string>
#include <cmath>
class point {
public:
	float posX,posY,posZ;
	point(const float x , const float y , const float z) : posX(x), posY(y), posZ(z) {}
	inline point operator+=(point b) const {
		float x = this->posX + b.posX;
		float y = this->posY + b.posY;
		float z = this->posZ + b.posZ;
		point obje(x,y,z);
		return obje;
	}
};
point p(float x, float y, float z) {
	point bob(x, y, z);
	return bob;
}
//"throwback" function
point operator+(point a, point b) {
	float x = a.posX + b.posX;
	float y = a.posY + b.posY;
	float z = a.posZ + b.posZ;
	return p(x,y,z);
}
float operator>>(point a, point b) {
	//get distance between points
	float xDist = pow(b.posX - a.posX, 2);
	float yDist = pow(b.posY - a.posY, 2);
	float zDist = pow(b.posZ - a.posZ, 2);
	return sqrt(xDist + yDist + zDist);
}
class linked_point { 
public:
	//More or less enables us to have multi-dimensional and defined connections
	point base;
	std::vector<point> linkedTo;
	linked_point(point _b, std::vector<point> _to) : base(_b) {
		linkedTo = _to;
	}
};
inline linked_point addlinked(linked_point basic, point in) {
	point baes = basic.base + in;
	std::vector<point> points;
	for (int ind = 0; ind < basic.linkedTo.size() -1; ind++) {
		point yuh = basic.linkedTo.at(ind);
		//Round 7 Fix: fixed with rename/re-add
		yuh += in;
		points.push_back(yuh);
	}
	linked_point t(baes, points);
	return t;
}
namespace entbaseD {
	class entityValue {
	public:
		std::string title, defaultValue;
		bool numeric, boolean;
	};
	class entBase {
	public:
		//Treat these *like* they're read-only, please...
		bool solid, hint, npc, model, control, other;
		int npcclassID; //Set to -1 if N/A, set to &(NPC_class) if applicabale
		std::vector<linked_point> occupiedSpaceLocal;
		float volume;
		std::string base_name;
		std::vector<entityValue> values;
		//No more *readonly-ish*
		std::vector<int> realtime_children;
		entBase(bool _s, bool _h, bool _ai, bool _m, bool _c, bool _oth, std::string _entname, std::vector<linked_point> pnt, std::vector<entityValue> neededVals, float _vol) {
			solid = _s;
			hint = _h;
			volume = _vol;
			npc = _ai;
			model = _m;
			control = _c;
			other = _oth;
			base_name = _entname;
			for (int i = 0; i < pnt.size() - 1; i++) {
				occupiedSpaceLocal.push_back(pnt.at(i));
			}
			for (int i = 0; i < neededVals.size() - 1; i++) {
				values.push_back(neededVals.at(i));
			}
		}
	};
	class entity {
	public:
		//Pretend that this section is protected
		bool smellable, isCrouched;
		entBase& base;
		std::vector<linked_point> occupied_space;
		point true_pos, rotation;
		std::string name;
		float height;
		int baseID, engineID, linkedNPC, smellStrength, realtimeID, terrainSliceID;
		//Set linkedNPC to -1 if not applicable, set to the JSON ID if applicable!
		//No more *pretend protected*
		std::vector<std::string> data_list; //The 'curValue' for each of the base's required item
		int registerSelf(int file_id) {			
			this->realtimeID = file_id;
			this->base.realtime_children.push_back(realtimeID);
			return this->base.realtime_children.size() - 1;
		}
		entity(entBase& parent, const point rot_, const point location_, std::string title, std::vector<std::string> list, int realID, int trnID) : base(parent), relative_pos(ter_location), rotation(rot_), true_pos(location_)  {
			name = title;
			terrainSliceID = trnID;
			std::vector<linked_point> d = parent.occupiedSpaceLocal;
			//Round 2 Fix: forgot that vector doesn't have 'length' but 'size'
			for (int i = 0; i < d.size() - 1; i++) {
				occupied_space.push_back(addlinked(d.at(i), location_));
			}
			base = parent;
			registerSelf(realID);
			for (int i = 0; i < list.size() - 1; i++) {
				data_list.push_back(list.at(i));
			}
		}
		~entity() {
			this->occupied_space.erase(this->occupied_space.begin(), this->occupied_space.end());
			this->base.realtime_children.erase(this->base.realtime_children.begin() + this->baseID);
			delete &(this->occupied_space);
		}
	};
}
