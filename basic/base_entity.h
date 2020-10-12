/*
FILE: base_entity.h
TITLE: Basic Entity Layout Provider
PURPOSE: Provides format bases for entities
VERSION: 18
*/
#define __IECAI_BASE_ENTITY__
#include <vector>
#include <string>
#include <cmath>
#include <fstream>
enum axi {x,y,z};
struct point {
	float posX,posY,posZ;
	point(float x, float y, float z) : posX(x), posY(y), posZ(z) {}
	inline point operator+(point a, point b) {
		float pX,pY,pZ = a.posX + b.posX,a.posY+b.posY,a.posZ+b.posZ;
		point temp(pX,pY,pZ);
		return temp;
	}
	inline point operator++(point a, float hgt) {
		float pX,pY,pZ = a.posX, a.posY, a.posZ + hgt;
		point temp(pX, pY, pZ);
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
namespace entbaseD {
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
		bool smellable, isCrouched;
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
namespace entbaseF {
	//These assume 'path' is valid!
	enum chartype {header, info, option, comment, list, ref, name, point, linkpnt, entopt, locopt};
	struct confLine {
		chartype type;
		string line;
		int lineNo;
		bool constant;
	};
	entBase fetchFromFile(std::string path, int classID) {
		std::fstream stree;
		stree.open(path, std::fstream::in);
		if (stree) { //Check to ensure it's valid
			stree.seekg(stree.beg + 5); //get 1 to 9 digit size of line_counter
			int numsize = stree.get() - 48;
			//48 == 0; 57 == 9  (In ASCII Ints)
			stree.seekg(stree.beg + 7);
			char[8] d;
			stree.get(d, numsize);
			int lines = 0;
			for (int i=0;i<numsize;i++){lines+=(((int)d[i]) - 48);}
			//lines is now set to the length of the file.
			int curPos = 7 + numsize + 1;
			for (int i = 1; i < lines; i++) {
				stree.sync();
				std::string temp;
				//I would fear whatever abomination would take more than 128 characters in a line; especially in my format.
				std::getline(stree, temp, '\n'); //overloaded string-type getline
				confLine line;
				line.line = temp;
				line.lineNo = i;
				curPos+=temp.length(); //I don't know if this is necessary; nothing in documentation saying it isn't. No -1 because of 'start on new line'
/*
DIRECTLY FROM THE .iecai-ent GENERIC FILE:
; IECAI Entities File Template
; Follow the following notation rules:
; # DEFINES FILE INFORMATION (TITLE, DESCRIPTION, VERSION, VERSION INGAME, AUTHOR, TOTAL OBJECTS, TOTAL CLASSES)
; $ DEFINES HEADER SPACES.
;  $ is a top-level header; requires: 't'itle, 'c'lasstypes, 'p'id  'r'ange, 'l'ength
;  $$ is an object-level header; requires: 'n'ame, 'i'd, 'ch'ildren, 'l'ength
;  top-level: no indent; obj-level: 2 indent, object: 3 indent (spaces)
; % DEFINES AN OPTION (I.E. 'SOLID' OR 'VOLUME'). %(type) (name) (value)
; [types: 'b'ool, 'i'nt, 'f'loat, 's'tring, 'p'oint, 'r'eference, 'l'inked 'p'oint, 'en'tity option]
; ; DEFINES A COMMENT.
; \ DEFINES A CONSTANT. PLACE BEFORE OTHER SYMBOL.
; #[ ] DEFINES LIST STRUCTURE (1sp indent when expanded; needs length number)
; ( ) DEFINES A REFERENCE
; " " DEFINES A NAME
; ^(i,i,i) DEFINES A POINT
; {^(i,i,i):#[..]} DEFINES A LINKED POINT
; {"","",/,/} DEFINES AN ENTITY OPTION (TITLE, DEFAULTVALUE, NUMERIC, BOOLEAN)
; // DEFINES A LOCAL OPTION (NOT PART OF THE PARENT OBJECT)
*/
				for (int _p = 0; _p < temp.length() - 1; _p++) {
					char c = temp[_p];
					bool cmnt, cons, opt, headA, headB, loc, pnt, list, ref, name, lPnt, headS = 0,0,0,0,0,0,0,0,0,0,0,0;
					int idnlvl = 0;
					
				}
			}
		}	
	}
	bool writeToFile(std::string path, entBase ent) {
		
	}
}
