/*
FILE: subzero/base.h
TITLE: SubZER0 Rendering Engine - Base
PURPOSE: Provides layout of OpenGL Objects and some functions
VERSION: 2
*/
#define __IMVENG_SUBZERO_BASE
//Import FreeGLUT
#include <GL/glut.h>
//Import basic OpenGL mathematics modules
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
//Import standard C++ modules
#include <iostream> //Console logs
#include <string>
#include <vector>
#include <cmath>
//Import entbaseD
#include "../base/bsc_dat_ent.h"
//Import mapbase
#include "../base/mapbase.h"
namespace rendBase {
	//Use relative floats on the screenspace when rendering.
	namespace overlay { //2D render objects which have higher priority in render ORDER
		struct UIobj {
			
		};
	}
	namespace world { //3D rendering objects
		class camera {
			//OpenGL Mathematics matrixes representing camera
			glm::mat4 glm_proj, glm_view;
			//Recolor Map - 20x20 points in RGB that add blended-intensity recolor across the screen
			point* rcmap[399];
		public:
			void applyRotation(point effective) {
				this->glm_view = glm::rotate(this->glm_view, ????); //TODO: check  glm  docs abt this function
			}
			void updateRCM(point* newmap[399]) {
				this->rcmap = newmap;
			}
		};
	}
}
