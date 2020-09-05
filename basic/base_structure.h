/*
FILE: base_structure.h
TITLE: Base Structuring for the IECAI project.
PURPOSE: Provide the base classes or structures of which everything else is derived from.
REVISION: 4
*/
#define __IECAI_BASE_STRUCTURE__
#include <vector>
#include <string>
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
			bool updateInput(std::string _in) {
				if (self.base->expectNumber) {
					//TODO: Test if string is a number
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
				} else if (base->expectReference) {
					//TODO: Test if string is a reference
				} else {
					self.input = _in;
					self.valid = true;
				}
				return self.valid;
			}
		};
	}
	
}
