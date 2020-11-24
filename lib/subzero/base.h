/*
FILE: subzero/base.h
TITLE: SubZER0 Rendering Engine - Base
PURPOSE: Provides essential video rendering and some objects for it.
VERSION: 3
COMPILATION NOTE: g++ -lSDL2 -ljsoncpp (file) -o out.o
*/
#define __IMVENG_SUBZERO_BASE
//Import Simple Directmedia Layer (no more OpenGL)
#include <SDL.h>
#include <SDL_video.h>
#include <SDL_pixels.h>
#include <SDL_render.h> //SDL 2D Acceleration
#include <SDL_surface.h>
//Import standard C++ modules
#include <iostream> //Console logs
#include <string>
#include <vector>
#include <cmath>
//Import entbaseD
#include "../base/bsc_dat_ent.h"
using namespace std;
using namespace entbaseD;
namespace rendBase {
	class window { //This existing window class is 100% functional as of 24 Nov 2020!!!
		//from txr_base.h; we're using 0-255 CHARs for RGB
		//anyways, SDL uses 0x## RGB colors.
		SDL_Window* object;
		SDL_Surface* surface;
	public:
		int width, height;
		string title = "ImmersivEngine";
		float xmult, ymult; //how 0.0%-100.0% reflects to pixels
		char red = 0xFF;
		char green = 0x00;
		char blue = 0x00;
		short int makeSDLwindow() {
			//Makes window open up.
			int c = SDL_Init(SDL_INIT_VIDEO);
			if (c < 0) {
				cout << "Error when initializing Simple Directmedia Layer : " << c << '\n';
			} else {
				this->object = SDL_CreateWindow( title.c_str() ,SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width,height, SDL_WINDOW_SHOWN);
				if (this->object == NULL) {
					cout << "Error when creating SDL Window!\n";
				} else {
					this->surface = SDL_GetWindowSurface(this->object);
					SDL_FillRect( this->surface,NULL, SDL_MapRGB(this->surface->format, red, green, blue));
					//Red Screen
					SDL_UpdateWindowSurface(this->object);
					cout << "Window and surface initialized.\n";
				}
			}
			return c;
		}
		void refreshSurface() {
			SDL_UpdateWindowSurface(this->object);
		}
		~window() {
			SDL_DestroyWindow( this->object );
			SDL_Quit();
		}
		//Easy constructor
		window(int w, int h, string ti) {
			width = w;
			height = h;
			title = ti;
		}
	};
}
