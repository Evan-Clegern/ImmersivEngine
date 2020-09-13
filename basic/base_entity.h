/*
FILE: base_entity.h
TITLE: Basic Entity Layout Provider
PURPOSE: Provides format bases for entities
VERSION: 1
*/
namespace entbase {
	struct point {
		float posX,posY,posZ;
		unsigned int pitch, yaw, roll;
		//Pitch = X-Z; forward rotation
		//Yaw = X-Y; top rotation
		//Roll = Y-Z; side rotation
	};
	namespace geometries {
		struct rectPrism {
			point a,b,c,d,e,f,g,h;
		};
		struct pyramid {
			point ba,bb,bc,bd,top;
		};
	}
}
