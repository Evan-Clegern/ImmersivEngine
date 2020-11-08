/*
TESTING FILE
FILE: base_entity.h
TITLE: Basic Entity Layout Provider
PURPOSE: Provides format bases for entities
VERSION: 23
*/
#define __IECAI_FVERSION 1.25
#define __IECAI_BASE_ENTITY__
#include <vector>
#include <string>
#include <cmath>
#include <fstream>
#include "dist/json/json.h"
#include "dist/json/json-forwards.h"
#include "dist/jsoncpp.cpp"
enum axi {x,y,z};
class point {
public:
	float posX,posY,posZ;
	point(float x, float y, float z) : posX(x), posY(y), posZ(z) {}
	inline void operator+=(point b) const {
		//Round 2 Fix: += does not allow for 2 args
		//So treated 'this' argument like an address-of (as specified in error code)
		this->posX +=b.posX;
		this->posY +=b.posY;
		this->posZ +=b.posZ;
	}
};
point p(float x, float y, float z) { //Round 2 Fix: Moved up
	point bob(x, y, z);
	return bob;
}
//"throwback" functions
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
namespace entbaseD {
	struct linked_point { 
		//More or less enables us to have multi-dimensional and defined connections
		point base;
		std::vector<point> linkedTo;
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
			//Round 2 Fix: Variable Name Problem (ai  instead of  npc)
			npc = _ai;
			model = _m;
			control = _c;
			other = _oth;
			base_name = _entname;
			for (int i = 0; i < pnt.size() - 1; i++) {
				//Round 2 Fix: forgot that vector doesn't have 'length' but 'size'
				occupiedSpaceLocal.push_back(pnt.at(i));
			}
			for (int i = 0; i < neededVals.length() - 1; i++) {
				//Round 2 Fix: forgot that vector doesn't have 'length' but 'size'
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
		terrain_slice& relative_pos;
		std::string name;
		float height;
		int baseID, engineID, linkedNPC, smellStrength, realtimeID;
		//Set linkedNPC to -1 if not applicable, set to the JSON ID if applicable!
		//No more *pretend protected*
		std::vector<std::string> data_list; //The 'curValue' for each of the base's required items
		int registerSelf(int file_id) {
			this->realtimeID = file_id;
			//Round 2 Fix(es): (const entity*)this  fix (-> versus .)
			this->base->addressChildren.push_back(realtimeID);
			//Round 2 Fix: Naming issue, and -> versus .
			return this->base->addressChildren.length() - 1;
		}
		entity(entBase& parent, point location, std::string title, std::vector<std::string> list) : name(title) {
			//Round 2 Fix: Replaced 'point& location' to 'point location'
			//Issue: uninitialized reference member to 'class entbaseD::entBase&'
			//Issue: no matching function for call to 'point::point()'
			//Issue: uninitialized reference member to 'struct entbaseD::terrain_slice&'
			std::vector<linked_point> d = parent->occupiedSpaceLocal;
			//Round 2 Fix: forgot that vector doesn't have 'length' but 'size'
			for (int i = 0; i < d.size() - 1; i++) {
				occupied_space.push_back(d.at(i) + location);
			}
			//Round 2 Fix: fixed rename mismatch (position  renamed to  true_pos)
			true_pos = location;
			//Round 2 Fix: fixed rename mismatch (effRotation  renamed to  rotation)
			rotation = p(0.0,0.0,0.0);
			base = parent;
			registerSelf();
			//Round 2 Fix: forgot that vector doesn't have 'length' but 'size'
			for (int i = 0; i < list.size() - 1; i++) {
				data_list.push_back(list.at(i));
			}
		}
		~entity() {
			//Round 2 Fixes: replaced the bad 'this.' with 'this->'
			this->occupied_space.erase(this->occupied_space.begin(), this->occupied_space.end());
			this->base->addressChildren.erase(this->base->addressChildren.begin() + this->baseID);
			delete &(this->occupied_space); //Round 2 Fixes: implemented pointers
			base=NULL;
			delete &(this->baseID);
		}
		void updatePosition(point pos, point rot) {
			//Round 2 Fixes: replaced the bad 'this.' with 'this->'
			this->position = pos;
			this->effRotation = rot;
			//TODO: Terrain Slice (relative positioning)
		}
	};
}
namespace entbaseFIN {
	//Fileside for entbase will be using JavaScript Object Notation due to ease-of-access
	//See the 'iecENTS-generic.json' for the layout this reads through!
	//According to the documentation from open-source-parsers/jsoncpp, this is all 100% correct.
	//WILL NEED TESTING!
	namespace simple {
		//jsoncpp uses a 'Json::Value root' and then input is given by an ifstream 'config_doc("whatevah")'
		//then config_doc >> root to load it, so we can then use 'root.get(value)' to read that itemvalue!
		std::string s_fetchsingle(Json::Value input, std::string name) {
			return input.get(name,"no").asString();
		}
		std::string s_fetchnested(Json::Value input, std::string parent, std::string name) {
			return input.get(input[parent].get(name, "no")).asString();
		}
		int i_fetchsingle(Json::Value input, std::string name) {
			return input.get(name, 0).asInt();
		}
		int i_fetchnested(Json::Value input, std::string parent, std::string name) {
			return (input[parent].get(name, 0)).asInt();
		}
		float f_fetchsingle(Json::Value input, std::string name) {
			return input.get(name, 0.00).asFloat();
		}
		float f_fetchnested(Json::Value input, std::string parent, std::string name) {
			return (input[parent].get(name, 0.00)).asFloat();
		}
		Json::Value loadstream(std::string file) {
			std::ifstream rate(file, std::ifstream::binary);
			Json::Value robert;
			rate >> robert;
			return robert;
		}
		std::vector<std::string> s_fetchlist(Json::Value input, std::string cname) {
			const Json::Value moine = input[cname];
			std::vector<std::string> data;
			for (int index = 0; index < moine.size(); index++) {
				data.push_back(moine[index].asString());
			}
			return data;
		}
		std::vector<int> i_fetchlist(Json::Value input, std::string cname) {
			const Json::Value moine = input[cname];
			std::vector<int> data;
			for (int i = 0; i < moine.size(); i++) {
				data.push_back(moine[i].asInt());
			}
			return data;
		}
		std::string s_doublenested(Json::Value input, std::string parA, std::string parB, std::string vName) {
			return (input[parA][parB].get(name, "no")).asString();
		}
		int i_doublenested(Json::Value input, std::string parA, std::string parB, std::string vName) {
			return (input[parA][parB].get(name, 0)).asInt();
		}
		float f_doublenested(Json::Value input, std::string parA, std::string parB, std::string vName) {
			return (input[parA][parB].get(name, 0)).asFloat();
		}
		std::string s_listobj(Json::Value input, std::string lname, unsigned short int index) {
			return (input[lname][index]).asString();
		}
		int i_listobj(Json::Value input, std::string lname, unsigned short int index) {
			return (input[lname][index]).asInt();
		}
		float f_listobj(Json::Value input, std::string lname, unsigned short int index) {
			return (input[lname][index]).asFloat();
		}
		entbaseD::linked_point fetch_linkedPoint(Json::Value oper_const) {
			//Point layout in JSON - {"main":[0,0,0],"size":3,"links":[ [0,0,1],[0,1,0],[1,0,0] ]}
			float x_main = oper_const["main"][0].asFloat();
			float y_main = oper_const["main"][1].asFloat();
			float z_main = oper_const["main"][2].asFloat();
			const point bruh = p(x_main, y_main, z_main);
			int size = oper_const.get(size, 3).asInt() - 1;
			std::vector<point> pointes;
			for (int i=0;i<size;i++) {
				float x= oper_const["links"][i][0];
				float y= oper_const["links"][i][1];
				float z= oper_const["links"][i][2];
				point t = p(x, y, z);
				pointes.push_back(t);
			}
			entbaseD::linked_point the_pnt;
			the_pnt.base = bruh;
			the_pnt.linkedTo = pointes;
			return the_pnt;
		}
		std::vector<entbaseD::linked_point> fetch_alllinks(Json::Value obj) {
			std::vector<entbaseD::linked_point> bruh;
			std::string n = "spacelocal_s";
			if (obj.get(n,-5).asInt() == 0) {
				throw "No Linked points exist in class!";
			}
			for (int i = 0; i < obj.get(n, 1).asInt() - 1; i++) { //i love you jsoncpp
				const Json::Value list = obj["spacelocal"][i];
				const entbaseD::linked_point yuh = fetch_linkedPoint(list);
				bruh.push_back(yuh);
			}
			return bruh;
		}
		entbaseD::entityValue get_entvalue(Json::Value obj, std::string name) {
			//NOTE: For 'entvalues', 0 = string, 1 = boolean and 2 = number (for the lists!)
			const Json::Value nuw = obj[name];
			entbaseD::entityValue bro;
			if (nuw[0].asInt() == 0) {
				bro.numeric = false;
				bro.boolean = false;
			} else if (nuw[0].asInt() == 1) {
				bro.numeric = false;
				bro.boolean = true;
			} else {
				bro.numeric = true;
				bro.boolean = false;
			}
			bro.defaultValue = obj[1];
			bro.title = name;
			return bro;
		}
		std::vector<entbaseD::entityValue> load_values(Json::Value entit) {
			const Json::Value listA = entit["valuenames"];
			int total = listA.size();
			const Json::Value listB = entit["valuedata"];
			std::vector<entbaseD::entityValue> burh;
			for (int i = 0; i < total - 1; i++) { //jsoncpp example says it may treat "size" with respect to index... I doubt it.
				burh.push_back( get_entvalue(listB, listA[i]) );
			}
			return burh;
		}
	}
	enum entval_t {text, toggle, number};
	const float good_throwbacks[1.15];
	const int throwback_cnt 1;
	namespace f_tests {
		bool validmeta(Json::Value fileoper, std::string purpose) {
			float d = simple::f_fetchnested(input, "metadata", "iecai-vers");
			if (d == __IECAI_VERSION) {
				std::string b = simple::s_fetchnested(fileoper, "metadata","iecai-purpose");
				if (b == purpose) {
					return true;
				} else {
					return false;
				}
			} else {
				bool t = false;
				if (d == good_throwbacks[0]) {
					t = true;
				}
				for (int i=1;i<throwback_cnt - 1;i++) {
					if (d == good_throwbacks[i]) {
						t = true;
					}
				}
				if (t) {
					std::string b = simple::s_fetchnested(fileoper, "metadata","iecai-purpose");
					if (b == purpose) {
						return true;
					} else {
						return false;
					}
				} else {
					return false;
				}
				
			}
		}
		bool ispointent(Json::Value fileoper, std::string name) {
			float t = simple::f_fetchnested(fileoper, name, "volume");
			if (t == 0.00) {return true;} else {return false};
		}
	}
	entbaseD::entBase generateClass(std::string file, std::string objName) {
		Json::Value d = simple::loadstream(file);
		if (not f_tests::validmeta(d, "entity")) {
			throw "Invalid file version or type.";
		}
		const Json::Value operations = d[objName];
		bool _s, _h, _ai, _m, _c, _ot;
		_s = simple::i_listobj(operations, "options", 0);
		_h = simple::i_listobj(operations, "options", 1);
		_ai = simple::i_listobj(operations, "options", 2);
		_m = simple::i_listobj(operations, "options", 3);
		_c = simple::i_listobj(operations, "options", 4);
		_ot = simple::i_listobj(operations, "options", 5);
		int classe = simple::i_fetchsingle(operations, "npc_class");
		std::vector<entbaseD::linked_point> space = simple::fetch_alllinks(operations);
		std::vector<int> childfids= simple::i_fetchlist(operations, "children");
		float vol = f_fetchsingle(operations, "volume");
		std::vector<entbaseD::entityValue> vals = simple::load_values(operations);
		entbaseD::entBase djungelskog(_s, _h, _ai, _m, _c, _ot, objName, space, vals, vol);
		return djungelskog; //https://www.ikea.com/gb/en/cat/djungelskog-collection-40892/
	}
}
namespace entbaseFOUT {
	bool update_existingF(std::string file, entbaseD::entBase& base) {
		//Can you do root["metadata"]["ent-count"] = 2
		// and then do [{fstream}] << root  ?
		//JSONCPP example says you can... okay then
		Json::Value oldstream = entbaseFIN::loadstream(file);
		if (not entbaseFIN::f_tests::validmeta(oldstream, "entity")) {return false;}
		Json::Value newstream = oldstream;
		//We'll overwrite the entire file with this
		//Why? We mirrored its contents in a Json::Value
		std::fstream streame(file, std::fstream::in | std::fstream::out | std::fstream::binary | std::fstream::truncate);
		newstream["metadata"]["iecai-vers"] = __IECAI_FVERSION;
		newstream["metadata"]["file-vers"] = (oldstream["metadata"].get("file-vers",0).asInt() + 1);
		//Once we determine if it's replacing or not, change this accordingly.
		unsigned short int t = oldstream["metadata"].get("ent-count",0).asInt();
		if (oldstream.get(base->base_name, "") == NULL) {
			newstream["metadata"]["ent-count"] = t + 1;
		} else {
			newstream["metadata"]["ent-count"] = t; //Object exists in file
		}
		std::string name = base->base_name;
		//Reverse Construct the entBase class.
		newstream[name]["volume"] = base->volume;
		newstream[name]["npc_class"] = base->npcclassID;
		//THE BATCH CONSTRUCTOR FOR LINKED POINTS!!!!!!!!!!!!!!!!!!!!!!
		for (int A = 0; A < base->occupiedSpaceLocal.size() - 1; A++) {
			linked_point pnt = base->occupiedSpaceLocal.at(A);
			newstream[name]["spacelocal"][A]["main"][0] = pnt.base.x;
			newstream[name]["spacelocal"][A]["main"][1] = pnt.base.y;
			newstream[name]["spacelocal"][A]["main"][2] = pnt.base.z;
			int index = pnt.linkedTo.size;
			newstream[name]["spacelocal"][A]["size"] = index;
			for (int i = 0; i < index - 1; i++) {
				const point D = pnt.linkedTo.at(i);
				//oh what fun. 5-level parenting...
				newstream[name]["spacelocal"][A]["links"][i][0] = D.x;
				newstream[name]["spacelocal"][A]["links"][i][1] = D.y;
				newstream[name]["spacelocal"][A]["links"][i][2] = D.z;
			}
			newstream[name]["spacelocal_s"] = index;
			delete &pnt;
		}	
		for (int i = 0; i < base->values.size() - 1; i++) {
			entbaseD::entityValue d = base->values.at(i);
			newstream[name]["valuenames"][i] = d.title;
			int deed;
			if (d.numeric) {
				deed = 2;
			} else if (d.boolean) {
				deed = 1;
			} else {
				deed = 0;
			}
			newstream[name]["valuedata"][d.title][0] = deed; //0 = string, 1 = bool (Yes/No), 2 = number (Float)
			newstream[name]["valuedata"][d.title][1] = d.defaultValue;
		}
		for (int i = 0; base->addressChildren.size() - 1; i++) {
			newstream[name]["children"][i] = base->addressChildren.at(i);
		}
		//Last thing: options
		int o_a, o_b, o_c, o_d, o_e, o_f;
		if (base->solid) {o_a = 1;} else {o_a = 0;}
		if (base->hint) {o_b = 1;} else {o_b = 0;}
		if (base->npc) {o_c = 1;} else {o_c = 0;}
		if (base->model) {o_d = 1;} else {o_d = 0;}
		if (base->control) {o_e = 1;} else {o_e = 0;}
		if (base->other) {o_f = 1;} else {o_f = 0;}
		newstream[name]["options"][0] = o_a;
		newstream[name]["options"][1] = o_b;
		newstream[name]["options"][2] = o_c;
		newstream[name]["options"][3] = o_d;
		newstream[name]["options"][4] = o_e;
		newstream[name]["options"][5] = o_f;
		delete &oldstream;
		try {
			streame << newstream;	
		} catch {
			return false;	
		}
		return true;
	}
	
}
