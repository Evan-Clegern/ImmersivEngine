/*
FILE: bsc_json.h
TITLE: Basic Entity JSON File Operations
PURPOSE: Provides format bases for entities
This File (a "fork" of entbase fileops) Has Been Debugged Fully (branch testing-nov2020)
VERSION: 2
*/
#define __IMVENG_FVERSION 1.25
#define __IMVENG_BASE_ENTITY_FILEOPS
#include <vector>
#include <string>
#include <fstream>
#include "dist/json/json.h"
#include "dist/json/json-forwards.h"
#include "dist/jsoncpp.cpp"
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
			for (int i = 0; i < obj.get(n, 1).asInt() - 1; i++) { //i love you jsoncpp (for this)
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
			for (int i = 0; i < total - 1; i++) {
				//jsoncpp example says it may treat "size" with respect to index... I doubt it.
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
			float d = simple::f_fetchnested(fileoper, "metadata", "imveng-vers");
			if (d == __IMVENG_FVERSION) {
				std::string b = simple::s_fetchnested(fileoper, "metadata","imveng-purpose");
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
					std::string b = simple::s_fetchnested(fileoper, "metadata","imveng-purpose");
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
		newstream["metadata"]["imveng-vers"] = __IECAI_FVERSION;
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
		//THE BATCH CONSTRUCTOR FOR LINKED POINTS!!!!
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
