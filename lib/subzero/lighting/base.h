/*
FILE: subzero/lighting/base.h
TITLE: SubZER0 Rendering Engine - Lighting Base
PURPOSE: Provide basic lighting objects and calculation
VERSION: 1
*/
#include <cmath>
#include <vector>
#include "../../base/bsc_dat_ent.h"
typedef unsigned short int snum;
namespace lighting {
	struct r_mat {
		point RGB, RGB_b;
		snum sharpness, shine, coat, coatMix, blendLevel;
	};
	struct fr_mat {
		r_mat base;
		point shadowRGB, reflectAbsorb;
		snum clearness, clearBlur, clearFrost, coatSharp, blendSharp;
	};
	struct light_field {
		r_mat lMaterial;
		/* r_mat "renamed" data:
		- shine  as the curve level
		- coat  as linear level
		- coatMix  as reflective add
		*/
		point origin, intensityAngles;
		//Angles: X: low, Y: medium, Z: high  (angle in relation to 'Z' line; max 180 deg (CIRCLE!))
		float intensity, iPitch, iYaw;
	};
};
