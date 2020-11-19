/*
FILE: mapbase.h
TITLE: Map System Base Header
PURPOSE: Provides brushes and a majority of I/O with map geometry.
VERSION: 1
*/
#include "bsc_dat_ent.h"
#include <iostream> //For consoles
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include "dist/json/json.h"
#include "dist/json/json-forwards.h"
#include "dist/jsoncpp.cpp"
namespace geom_basic {
    //Brush should be for In-World objects opposed to entities.
    struct brush_face {
      std::vector<point> assumedLinks;
      point midpoint;
      //These assume that each point connects to its two neighboring points.
      unsigned int faceID, parentID;
    };
    struct brush_flink {
      brush_face& a, b;
      point sharedpntA, sharedpntB;
    };
    struct brush {
      std::vector<brush_face> faces;
      std::vector<brush_flink> facelinks;
      std::vector<std::vector<point>> nested_disp_bounds;	
      unsigned int brushID;
    };
}
