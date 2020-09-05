#define __IECAI_BASE_STRUCTURE__
#define __IECAI_VERSION 1
namespace base {
	template<class T> struct list {
		T data[];
		int len;
		void append(T _data) {
			data[len] = _data;
			len++;
		}
		bool del(int ID) {
			if (ID >= len) {return false;}
			T temp[];
			for (int i=ID+1; i < len-1; i++) {
				temp[i - (ID - 1)] = data[i];
			}
			data = temp;
			return true;
		}
	};
	struct point {
		float x, y, z;
		unsigned int ID;
	};
	struct object_config_supply {
		bool healthItem, ammoItem, dispenser;
		unsigned int uses, valueIDRestore; //Set 'uses' to zero to have infinite.
		unsigned float restore;
	};
	struct object_config_vehicle {
		bool NPCusable, NPCrideable;
		unsigned float speed, acceleration, deceleration;
	};
	struct object_config_door {
		bool remoteAccess, rotating;
		unsigned float speed, rotationAmnt, width;
	};
	struct object_config_path {
		unsigned int group, pid;
		list<int&> connections;
		bool forcePath;
	};
	struct object_config_hintpath : object_config_path {
		
	};
	//The object <class> is its object_config_********
	template<class T> struct object_class { //Mirror these values as they appear in the engine when defining.
		char* name[], desc[];
		float volume;
		list<point> dimensions;
		point basePoint;
		T& config_base;
	};
}