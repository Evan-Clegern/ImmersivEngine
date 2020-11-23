/*
FILE: mapbase.h
TITLE: Map System Base Header
PURPOSE: Provides basic map geometry and data
VERSION: 2
*/
#include "bsc_dat_ent.h"
#include <iostream> //For console data
#include <fstream>
#include <string>
#include <vector>
#include "dist/json/json.h"
#include "dist/json/json-forwards.h"
#include "dist/jsoncpp.cpp"
namespace geom_basic {
    struct point2d {
        float A, B;
    };
    //Brush should be for In-World objects opposed to entities.
    struct brush_face {
        std::vector<point> assumedLinks;
        point midpoint;
        //These assume that each point connects to its two neighboring points.
        unsigned int faceID;
        std::string fullpath; //B{brushID}:{faceID} -> B0:0
    };
    struct brush_flink {
        brush_face& a, b;
        point sharedpntA, sharedpntB;
    };
    struct brush_disp {
        std::vector<point2d> x_bound;
        std::vector<point2d> y_bound;
        std::vector<point2d> z_bound;
    };
    struct brush {
        std::vector<brush_face> faces;
        std::vector<brush_flink> facelinks;
        brush_disp disp_bounds;
        unsigned int brushID;
	};
	struct terrain_slice {
		//This needs to be a 5x5 square, where Z has the variance.
		linked_point* points[24];
		unsigned short int chunkChildID, obscuration;
		//Obscuration is 0-100; helps with visibilty calculations
		float avgHgt, maxHgt, minHgt, variance;
	};
	struct terrain_chunk {
		//15x15 of terrain_slices; therefore, 75x75 points (5,625 pnt)
		terrain_slice* slices[224];
		unsigned int chunkNum, biomeTypeNum;
		float maxHgt, minHgt, avgHgt, varianceHgt, avgObscuration, normalTemperature;
	};
    struct map_terrain {
        std::vector<terrain_chunk> chunks;
		float width, length, height;
		unsigned short int chunkPerRow, totalChunks, uniqueBiomes;
    };
}
struct map_header {
	/*
		"iecai-vers": 1.25,
		"map-iecai-vers": 0.1,
		"file-vers": 1,
		"world-data": {
			"title":"Example Brushes",
			"description":"",
			"validated":false,
			"runnable":false,
		 },
		"author": "Evan Clegern",
		"iecai-purpose": "map",
		"comment": "Shows example of map layout, and brushes. 'impmf' is 'ImmersivEngine Plaintext Map File'"
	*/
	std::string wrTitle, wrDesc, author, comment;
	int fileVers;
	float mapEngVer, engVer;
};
