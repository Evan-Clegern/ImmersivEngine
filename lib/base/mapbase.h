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
}
