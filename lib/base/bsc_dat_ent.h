/*
FILE: bsc_dat_ent.h
TITLE: Basic Data and Entity Definitions
PURPOSE: Provides format bases for entities
This File Has Been Debugged Fully (branch testing-nov2020)
VERSION: 26
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
			return input[parent].get(name, "no").asString();
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
			return (input[parA][parB].get(vName, "no")).asString();
		}
		int i_doublenested(Json::Value input, std::string parA, std::string parB, std::string vName) {
			return (input[parA][parB].get(vName, 0)).asInt();
		}
		float f_doublenested(Json::Value input, std::string parA, std::string parB, std::string vName) {
			return (input[parA][parB].get(vName, 0)).asFloat();
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
		linked_point fetch_linkedPoint(Json::Value oper_const) {
			//Point layout in JSON - {"main":[0,0,0],"size":3,"links":[ [0,0,1],[0,1,0],[1,0,0] ]}
			float x_main = oper_const["main"][0].asFloat();
			float y_main = oper_const["main"][1].asFloat();
			float z_main = oper_const["main"][2].asFloat();
			const point bruh = p(x_main, y_main, z_main);
			int size = oper_const.get(size, 3).asInt() - 1;
			std::vector<point> pointes;
			for (int i=0;i<size;i++) {
				//Round 4 Fix: Forgot '.asFloat()' item
				float x= oper_const["links"][i][0].asFloat();
				float y= oper_const["links"][i][1].asFloat();
				float z= oper_const["links"][i][2].asFloat();
				point t = p(x, y, z);
				pointes.push_back(t);
			}
			linked_point the_pnt(bruh, pointes);
			return the_pnt;
		}
		std::vector<lnked_point> fetch_alllinks(Json::Value obj) {
			std::vector<entbaseD::linked_point> bruh;
			std::string n = "spacelocal_s";
			if (obj.get(n,-5).asInt() == 0) {
				throw "No Linked points exist in class!";
			}
			for (int i = 0; i < obj.get(n, 1).asInt() - 1; i++) { //i love you jsoncpp
				const Json::Value list = obj["spacelocal"][i];				
				const linked_point yuh = fetch_linkedPoint(list);
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
			bro.defaultValue = obj[1].asString();
			bro.title = name;
			return bro;
		}
		std::vector<entbaseD::entityValue> load_values(Json::Value entit) {
			const Json::Value listA = entit["valuenames"];
			int total = listA.size();
			const Json::Value listB = entit["valuedata"];
			std::vector<entbaseD::entityValue> burh;
			for (int i = 0; i < total - 1; i++) { //jsoncpp example says it may treat "size" with respect to index... I doubt it.
				burh.push_back( get_entvalue(listB, listA[i].asString()) );
			}
			return burh;
		}
	}
	enum entval_t {text, toggle, number};
	const float good_throwbacks[] = {1.15};
	const int throwback_cnt = 1;
	namespace f_tests {
		bool validmeta(Json::Value fileoper, std::string purpose) {
			float d = simple::f_fetchnested(fileoper, "metadata", "iecai-vers");
			if (d == __IECAI_FVERSION) {
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
			if (t == 0.00) {return true;} else {return false;}
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
		std::vector<linked_point> space = simple::fetch_alllinks(operations);
		std::vector<int> childfids= simple::i_fetchlist(operations, "children");
		float vol = simple::f_fetchsingle(operations, "volume");
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
		Json::Value oldstream = entbaseFIN::simple::loadstream(file);
		if (not entbaseFIN::f_tests::validmeta(oldstream, "entity")) {return false;}
		Json::Value newstream = oldstream;
		//We'll overwrite the entire file with this
		//Why? We mirrored its contents in a Json::Value
		std::fstream streame(file, std::fstream::in | std::fstream::out | std::fstream::binary | std::fstream::trunc);
		newstream["metadata"]["iecai-vers"] = __IECAI_FVERSION;
		newstream["metadata"]["file-vers"] = (oldstream["metadata"].get("file-vers",0).asInt() + 1);
		//Once we determine if it's replacing or not, change this accordingly.
		unsigned short int t = oldstream["metadata"].get("ent-count",0).asInt();
		if (oldstream.get(base.base_name, "") == NULL) {
			newstream["metadata"]["ent-count"] = t + 1;
		} else {
			newstream["metadata"]["ent-count"] = t; //Object exists in file
		}
		std::string name = base.base_name;
		//Reverse Construct the entBase class.
		newstream[name]["volume"] = base.volume;
		newstream[name]["npc_class"] = base.npcclassID;
		//THE BATCH CONSTRUCTOR FOR LINKED POINTS!!!!!!!!!!!!!!!!!!!!!!
		for (int A = 0; A < base.occupiedSpaceLocal.size() - 1; A++) {
			linked_point pnt = base.occupiedSpaceLocal.at(A);
			newstream[name]["spacelocal"][A]["main"][0] = pnt.base.posX;
			newstream[name]["spacelocal"][A]["main"][1] = pnt.base.posY;
			newstream[name]["spacelocal"][A]["main"][2] = pnt.base.posZ;
			int index = pnt.linkedTo.size();
			newstream[name]["spacelocal"][A]["size"] = index;
			for (int i = 0; i < index - 1; i++) {
				const point D = pnt.linkedTo.at(i);
				//oh what fun. 5-level parenting...
				newstream[name]["spacelocal"][A]["links"][i][0] = D.posX;
				newstream[name]["spacelocal"][A]["links"][i][1] = D.posY;
				newstream[name]["spacelocal"][A]["links"][i][2] = D.posZ;
			}
			newstream[name]["spacelocal_s"] = index;
			delete &pnt;
		}	
		for (int i = 0; i < base.values.size() - 1; i++) {
			entbaseD::entityValue d = base.values.at(i);
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
		for (int i = 0; base.realtime_children.size() - 1; i++) {
			//Round 6 Fix: naming error (realtime_children, not addressChildren)
			newstream[name]["children"][i] = base.realtime_children.at(i);
		}
		//Last thing: options
		int o_a, o_b, o_c, o_d, o_e, o_f;
		if (base.solid) {o_a = 1;} else {o_a = 0;}
		if (base.hint) {o_b = 1;} else {o_b = 0;}
		if (base.npc) {o_c = 1;} else {o_c = 0;}
		if (base.model) {o_d = 1;} else {o_d = 0;}
		if (base.control) {o_e = 1;} else {o_e = 0;}
		if (base.other) {o_f = 1;} else {o_f = 0;}
		newstream[name]["options"][0] = o_a;
		newstream[name]["options"][1] = o_b;
		newstream[name]["options"][2] = o_c;
		newstream[name]["options"][3] = o_d;
		newstream[name]["options"][4] = o_e;
		newstream[name]["options"][5] = o_f;
		delete &oldstream;
		try {
			streame << newstream;	
		} catch(int e) {
			return false;	
		}
		return true;
	}
}
