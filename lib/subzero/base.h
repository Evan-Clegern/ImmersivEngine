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
	class window {
		//from txr_base.h; we're using 0-255 CHARs for RGB
		//anyways, SDL uses 0x## RGB colors.
		SDL_window* object;
		SDL_surface* surface;
	public:
		int width, height;
		string title = "ImmersivEngine";
		float xmult, ymult; //how 0.0%-100.0% reflects to pixels
		short int makeSDLwindow() {
			int c = SDL_init(SDL_INIT_VIDEO);
			if (c < 0) {
				cout << "Error when initializing Simple Directmedia Layer : " << c << '\n';
			} else {
				this->object = SDL_CreateWindow(title,SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width,height, SDL_WINDOW_SHOWN);
				if (this->object == NULL) {
					cout << "Error when creating SDL Window!\n";
				} else {
					this->surface = SDL_GetWindowSurface(this->object);
					SDL_FillRect( this->surface,NULL, SDL_MapRGB(this->surface.format, 0xFF, 0xFF, 0xFF));
					//White Screen
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
		//There is loadMedia() options for SDL
	};
}
