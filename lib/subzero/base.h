/*
FILE: subzero/base.h
TITLE: SubZER0 Rendering Engine - Base
PURPOSE: Provides essential video rendering and some objects for it.
VERSION: 3
COMPILATION NOTE: g++ -lSDL2 -ljsoncpp (file) -o out.o
*/
#define __IMVENG_SUBZERO_BASE
//Import Simple Directmedia Layer for windowing and 2D overlays
#include <SDL.h>
#include <SDL_video.h>
#include <SDL_surface.h>
//Import OpenGL
#include <gl.h>
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
	struct color{
		char red, green, blue, alpha;
	};
	GLubyte* clrtoGL(color d) {
		GLubyte* bruh[] = {d.red, d.green, d.blue, d.alpha};
		return bruh;
	}
	//Screenspace in OpenGL is represented by -1.0f to 1.0f
	//An SDL-OpenGL window object
	class window {
		SDL_Window* object;
		SDL_Surface* surface;
		SDL_VideoInfo* info;
	public:
		int width, height;
		string title = "ImmersivEngine";
		color SDLclr;
		bool deepColor = true;
		bool ultraDeep = false;
		bool fullscreen = false;
		short int makeSDLwindow() {
			//Makes window open up.
			int c = SDL_Init(SDL_INIT_VIDEO);
			this->info = SDL_GetVideoInfo();
			if ((c < 0) or (!this->info)) {
				cout << "Error when initializing Simple Directmedia Layer : " << c << '\n';
			} else {
				if ((this->deepColor) && (this->ultraDeep)) {
					//Very Resource Intensive
					SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 40);
					SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 17);
					SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 17);
					SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 17);
				} else if (this->ultraDeep) {
					SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);
					SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 14);
					SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 14);
					SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 14);
				} else if (this->deepColor) {
					//8-bit colors and 24-bit depth
					SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
					SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 11);
					SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 1);
					SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 11);
				} else {
					//5-bit colors and 16-bit depth
					SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
					SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
					SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
					SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
				}
				SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
				int flags;
				if (this->fullscreen) {
					flags = SDL_OPENGL | SDL_FULLSCREEN;	
				} else {
					flags = SDL_OPENGL;
				}
				if (SDL_SetVideoMode(width, height, info->vfmt->BitsPerPixel, flags) == 0) {
					cout << "Error when initializing OpenGL Video mode!";
				}
				this->object = SDL_CreateWindow( title.c_str() ,SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width,height, SDL_WINDOW_SHOWN);
				if (this->object == NULL) {
					cout << "Error when creating SDL Window!\n";
				} else {
					this->surface = SDL_GetWindowSurface(this->object);
					SDL_FillRect( this->surface,NULL, SDL_MapRGB(this->surface->format, this->SDLclr.red, this->SDLclr.green, this->SDLclr.blue));
					//Red Screen
					SDL_UpdateWindowSurface(this->object);
					cout << "Window and surface initialized. OpenGL configured.\n";
				}
			}
			return c;
		}
		void refreshSurfaceSDL() {
			SDL_UpdateWindowSurface(this->object);
		}
		~window() {
			SDL_DestroyWindow( this->object );
			SDL_Quit();
		}
		//Easy constructor
		window(int w, int h, string ti, bool rsc_intens_A, bool rsc_intens_B, bool fScrn) {
			width = w;
			height = h;
			title = ti;
			deepColor = rsc_intens_B;
			ultraDeep = rsc_intens_A;
			fullscreen = fScrn;
			color t;
			t.red = 255;
			t.green = 50;
			t.blue = 50;
			t.alpha = 255
			SDLclr = t;
			makeSDLwindow();
		}
		void reconfigGL() {
			float ratio = (float)this->width;
			ratio/=(float)this->height;
			glShadeModel(GL_SMOOTH);
			glCullFace(GL_BACK);
			glFrontFace(GL_CCW);
			glEnable(GL_CULL_FACE);
			glClearColor(0,0,0,0);
			glViewport(0, 0, this->width, this->height);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
		}
	};
	//TODO: Finish OpenGL-SDL Bridging
	//Check tutorials/documentation. CRITICAL!!!!!
}
