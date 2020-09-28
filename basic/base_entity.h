/*
FILE: base_entity.h
TITLE: Basic Entity Layout Provider
PURPOSE: Provides format bases for entities
VERSION: 14
*/
#define __IECAI_BASE_ENTITY__
#include <vector>
#include <string>
#include <cmath>
enum axi {x,y,z};
struct point {
	float posX,posY,posZ;
	point(float x, float y, float z) : posX(x), posY(y), posZ(z) {}
	inline point operator+(point a, point b) {
		float pX,pY,pZ = a.posX + b.posX,a.posY+b.posY,a.posZ+b.posZ;
		point temp(pX,pY,pZ);
		return temp;
	}
	inline float operator>>(point a, point b) {
		//get distance
		float xDist = pow(b.x - a.x, 2);
		float yDist = pow(b.y - a.y, 2);
		float zDist = pow(b.z - a.z, 2);
		return sqrt(xDist + yDist + zDist);
	}
};
point p(float x, float y, float z) {
	point bob(x, y, x);
	return bob;
}
namespace entbase {
	struct linked_point { 
		//More or less enables us to have multi-dimensional and defined connections
		point& base;
		std::vector<point&> linkedTo;
	};
	//Very simple groups; put here so they aren't cluttering other header files
	struct terrain_slice {
		//This needs to be a 5x5 square, where Z has the variance.
		linked_point points[24];
		unsigned short int chunkChildID, obscuration;
		//Obscuration is 0-100; helps with visibilty calculations
		float avgHgt, maxHgt, minHgt, variance;
	};
	struct terrain_chunk {
		//15x15 of terrain_slices; therefore, 75x75 points
		terrain_slice slices[224];
		unsigned int chunkNum, biomeTypeNum;
		float maxHgt, minHgt, avgHgt, varianceHgt, avgObscuration, normalTemperature;
	};
	class entityValue {
	protected:
		std::string title, defaultValue;
		bool numeric, boolean;
	};
	class entBase {
	protected:
		bool solid, hint, npc, model, control, other;
		int npcclassID; //Set to -1 if N/A, set to &(NPC_class) if applicabale
		std::vector<linked_point> occupiedSpaceLocal;
		float volume;
		std::string base_name;
		std::vector<entityValue> values;
		//These are the +/- from the midpoint of the functional entity.
	public:
		std::vector<int&> addressChildren;
		entBase(bool _s, bool _h, bool _ai, bool _m, bool _c, bool _oth, std::string _entname, std::vector<linked_point> pnt, std::vector<entityValue> neededVals) {
			solid = _s;
			hint = _h;
			ai = _ai;
			model = _m;
			control = _c;
			other = _oth;
			base_name = _entname;
			for (int i = 0; i < pnt.length() - 1; i++) {
				occupiedSpaceLocal.push_back(pnt.at(i));
			}
			for (int i = 0; i < neededVals.length() - 1; i++) {
				values.push_back(neededVals.at(i));
			}
		}
	};
	class entity {
	protected:
		bool smellable;
		entBase& base;
		std::vector<linked_point> occupied_space;
		point& position, effRotation;
		terrain_slice& relative_pos;
		std::string name;
		float height;
		int baseID, engineID, linkedNPC, smellStrength; //Set linkedNPC to -1 if not applicable, set to &(npc) if applicable!
	public:
		std::vector<std::string> data_list; //The 'curValue' for each of the base's required items
		int registerSelf() {
			int& ref = (int&)this;
			this.base->addressChildren.push_back(ref);
			return entBase.addressChildren.length() - 1;
		}
		entity(entBase& parent, point& location, std::string title, std::vector<std::string> list) : name(title) {
			std::vector<linked_point> d = parent->occupiedSpaceLocal;
			for (int i = 0; i < d.length() - 1; i++) {
				occupied_space.push_back(d.at(i) + location);
			}
			position = location;
			effRotation = &(p(0.0,0.0,0.0));
			base = parent;
			registerSelf();
			for (int i = 0; i < list.length() - 1; i++) {
				data_list.push_back(list.at(i));
			}
		}
		~entity() {
			this.occupied_space.erase(this.occupied_space.begin(), this.occupied_space.end());
			this.base->addressChildren.erase(this.base->addressChildren.begin() + this.baseID);
			delete occupied_space;
			base=NULL;
			delete baseID;
		}
		void updatePosition(point& pos, point& rot) {
			this.position = pos;
			this.effRotation = rot;
		}
	};
}
