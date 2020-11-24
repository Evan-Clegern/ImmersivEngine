/*
FILE: subzero/base.h
TITLE: SubZER0 Rendering Engine - Base
PURPOSE: Provides essential video rendering and some objects for it.
VERSION: 3
*/
#define __IMVENG_SUBZERO_BASE
//Import Simple Directmedia Layer (no more OpenGL)
#include <SDL.h>
#include <SDL_video.h>
#include <SDL_pixels.h>
#include <SDL_render.h> //SDL 2D Acceleration
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
	class window {
		//from txr_base.h; we're using 0-255 CHARs for RGB
		SDL_window* object;
		SDL_surface* surface;
	public:
		int width, height;
		float xmult, ymult; //how 0.0%-100.0% reflects to pixels
		int initwindow() {
			int c = SDL_init(SDL_INIT_VIDEO);
			if (c < 0) {
				std::cout << "Error when initializing Simple Directmedia Layer : " << c << '\n';
			}
			return c;
		}
	};
}
