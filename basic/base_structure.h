/*
FILE: base_structure.h
TITLE: Base Structuring for the IECAI project.
PURPOSE: Provide the base classes or structures of which everything else is derived from.
REVISION: 4
*/
#define __IECAI_BASE_STRUCTURE__
#include <vector>
#include <string>
#include <locale>
//Windows Compile Options
#ifdef __WIN32 || __WIN64 || _WIN32 || _WIN64
	#include <windows.h>
#endif
namespace base {
	/*
	TITLE: Base Structure Provider
	PROVIDES: Structure components for the IECAI Project's framework.
	*/
	struct point {
		float x, y, z;
		unsigned int ID;
	};
	namespace object_components {
		/*
		TITLE: Object-Class Components Provider
		PROVIDES: Flexible components for setting the needed inputs and relations for an object.
		*/
		struct value_layout {
			std::string title, hint;
			bool expectNumber, expectTF, expectReference;
			value_layout(std::string _t, std::string _h, bool _num, bool _bool, bool _ref) : \
				expectNumber(_num), expectTF(_bool), expectReference(_ref), title(_t), hint(_h) {}
		};
		class value {
		protected:
			value_layout& base;
			std::string input;
			bool valid;
		public:
			value(value_layout& _base) : base(_base) {}
			bool updateInput(std::string _in) {
				if (self.base->expectNumber) {
					for (int i=0;i<_in.length();i++) {
						std::string temp;
						bool nodec = true;
						if (isdigit(_in.at(i))) {
							temp += _in.at(i);
						}
						if (nodec and (_in.at(i) == '.')) {
							temp += '.';
							nodec = false;
							//Prevents it from registering more than one decimal point
						}
					}
					if (temp.length() > 0) {
						self.input = temp;
						self.valid = true;
					} else {
						self.input = "Err";
						self.valid = false;
					}
				} else if (self.base->expectTF) {
					std::string d = toupper(_in);
					if ((d == "Y") or (d == "TRUE") or (d == "YES") or (d == "1")) {
						self.input = "True";
						self.valid = true;
					} else if ((d == "N") or (d == "FALSE") or (d == "NO") or (d == "0")) {
						self.input = "False";
						self.valid = true;
					} else {
						self.input = "Wrong type";
						self.valid = false;
					}
				} else {
					self.input = _in;
					self.valid = true;
				}
				return self.valid;
			}
		};
		struct global_object_options {
			int numid, values;
			bool for_npc, for_supply, for_vehicle, for_weapon, for_navigation;
		};
		global_object_options newOpt(int _id, int _vals, bool _n, bool _s, bool _v, bool _w, bool _nav) {
			global_object_options value;
			value.numid = _id;
			value.values = _vals;
			value.for_npc = _n;
			value.for_supply = _s;
			value.for_vehicle = _v;
			value.for_navigation = _nav;
			return value;
		}
	}
	class Object_Class {
		//backup-only values
		bool locked;
		std::vector<object_components::value_base&> organized_bases;
		std::vector<std::string> organized_defaults;
	protected:
		std::vector<object_components::value> organized_values;
		object_components::global_object_options config;
	public:
		Object_Class(std::vector<object_components::value> _vals, std::vector<std::string> _defVals, \
			     bool _npc, bool _supply, bool _vhcl, bool _wep, bool _nav, int _id) {
			organized_defaults = _defVals;
			organized_values = value;
			std::vector<value_base&> temp;
			int d = _defVals.size();
			for (int i=0;i<d - 1;i++) {
				temp.push_back(_defVals.at(i).base);
			}
			organized_bases = temp;
			locked = false;
			const object_components::global_object_options OPTS = object_components::newOpt(_id,d,_npc,_supply,_vhcl,_wep,_nav);
			config = OPTS;
		}
	};
}
