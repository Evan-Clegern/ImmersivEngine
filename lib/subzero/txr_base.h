/*
FILE: subzero/txr_base.h
TITLE: SubZER0 Rendering Engine - Lighting Base
PURPOSE: Provide basic lighting objects, materials and vis calculationa
VERSION: 2
*/
#include <cmath>
#include <vector>
#include "../base/bsc_dat_ent.h"
typedef unsigned char snum; //0 to 255
typedef unsigned short int sint; //0 to 65,536
namespace lighting {
	struct colornum {
		snum Red, Green, Blue;
	};
	
	class light_field {
		//Light_Field calculations are out of 100% of the Intensity (this side of the lighting is only for intensity/tracing calculations!)
		float* cheaprange[14];
		//Raycasting arrays of stepped distances for rays. Cheaprange is in steps of (max_distance)/15
		sint Hmax_distance, Mmax_distance, Lmax_distance;
		float* expenserange[99];
		bool expensive;
		snum Rlow, Rmedium, Rhigh; //intensityAngles translated into 'Z'
	public:
		r_voxel projected_data;
		/* r_mat "renamed" data:
		- shine  as the inverse multiplier for RED anisotropy color add (0xFF = (1/255 * (R light)) + R surface )
		- coat  as the inverse multiplier for BLUE anisotropy color add
		- alpha  as the inverse multiplier for GREEN anisotropy color add
		- coatMix  as anisotropy blur-blend level
		*/
		point origin, intensityAngles;
		//Angles: X: low, Y: medium, Z: high  (angle in relation to 'Z' line; max 180 deg (CIRCLE!))
		//If X = 180, Y = 75 and Z = 25, it'd be like a light bulb in a translucent holder
		//The light reaches everywhere, but is brighter in the direction of the holder
		snum intensity, iRotX, iRotZ;
		virtual float get_level_at(point input) {
			//Apply light shifting
			point where = input + p(iRotX, 0.0, iRotZ);
			float intens = this->intensity;
			if (where.posZ <= Rhigh) {
				intens+=1; //100% of intensity, +1
			} else if (where.posZ <= Rmedium) {
				intens*=0.8; //80% of intensity
			} else if (where.posZ <=Rlow) {
				intens*=0.55; //44% of intensity
			} else {
				return 0.0; //not in range
			}
			float dist = this->origin >> where;
			//Removes NaN's
			dist += (this->intensity / 100);
			///PSEUDOLINear provides a scaled "cutoff" subtraction from each equation
			//Stronger lights have a stronger dropoff rate than weaker ones, but still have longer range
			float l = ((intens / dist) + pow(0.95, dist) - (0.01 * dist));
			if (l > 100.0) {
				l = 100.0;
			} else if (l < 0.0) {
				l = 0.0;
			}
			return l;
		}
		void set_Zmax() {
			
		}
		void calc_all_max() {
			sint dist = 1;
			float intens = this->intensity;
			float temp = ((intens / dist) + pow(0.95, dist) - (0.01 * dist));
			while (temp > 0) {
				dist++;
				temp = ((intens / dist) + pow(0.95, dist) - (0.01 * dist));
			}
			this->Hmax_distance = dist;
			dist = 1;
			intens*=0.8;
			temp = ((intens / dist) + pow(0.95, dist) - (0.01 * dist));
			while (temp > 0) {
				dist++;
				temp = ((intens / dist) + pow(0.95, dist) - (0.01 * dist));
			}
			this->Mmax_distance = dist;
			dist = 1;
			intens*=0.55;
			temp = ((intens / dist) + pow(0.95, dist) - (0.01 * dist));
			while (temp > 0) {
				dist++;
				temp = ((intens / dist) + pow(0.95, dist) - (0.01 * dist));
			}
			this->Lmax_distance = dist;
		}
		light_field(bool expense, point where, point angl, 
		float cast_ray(point location) {
			
		}
	};
};
