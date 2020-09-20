/*
FILE: base_entity.h
TITLE: Basic Entity Layout Provider
PURPOSE: Provides format bases for entities
VERSION: 9
*/
#define __IECAI_BASE_ENTITY__
#include <vector>
#include <string>
enum axi {x,y,z};
struct point {
	float posX,posY,posZ;
	point(float x, float y, float z) : posX(x), posY(y), posZ(z) {}
	inline point operator+(point a, point b) {
		float pX,pY,pZ = a.posX + b.posX,a.posY+b.posY,a.posZ+b.posZ;
		point temp(pX,pY,pZ);
		return temp;
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
	struct terrain_slice {
		//This needs to be a 5x5 square, where Z has the variance.
		linked_point* points[24];
		unsigned short int chunkChildID, obscuration;
		//Obscuration is 0-100; helps with visibilty calculations
		float avgHgt, maxHgt, minHgt, variance;
	};
	struct terrain_chunk {
		//15x15 of terrain_slices; therefore, 75x75 points
		terrain_slice* slices[224];
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
		bool solid, hint, ai, model, control, other;
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
		entBase& base;
		std::vector<linked_point> occupied_space;
		point position;
		std::string name;
		int baseID, engineID;
	public:
		std::vector<std::string> data_list; //The 'curValue' for each of the base's required items
		int registerSelf() {
			int& ref = (int&)this;
			this.base->addressChildren.push_back(ref);
			return entBase.addressChildren.length() - 1;
		}
		entity(entBase& parent, point location, std::string title, std::vector<std::string> list) : name(title) {
			std::vector<linked_point> d = parent->occupiedSpaceLocal;
			for (int i = 0; i < d.length() - 1; i++) {
				occupied_space.push_back(d.at(i) + location);
			}
			position = location;
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
			delete position;
		}
	};
}
