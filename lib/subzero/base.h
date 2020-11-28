/*
FILE: subzero/base.h
TITLE: SubZER0 Rendering Engine - Base
PURPOSE: Provides essential video rendering and some objects for it.
VERSION: 5 (tested on 27 Nov 2020)
COMPILATION NOTE: g++ (file) -lSDL2 -lGL
*/
#define __IMVENG_SUBZERO_BASE
//Import Simple Directmedia Layer for windowing and 2D overlays
#include <SDL2/SDL.h> //update: moved the naming to SDL2/**** to remove unneccesary commands!
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_cpuinfo.h>
//Import OpenGL
#include <GL/gl.h>
//Import standard C++ modules
#include <iostream> //Console logs
#include <string>
#include <vector>
#include <cmath>
//Import entbaseD
#include "../base/bsc_dat_ent.h"
using namespace std;
typedef unsigned char chr;
typedef unsigned int unt; //in Geany, i'm getting something that says a  uint  is already defined, so there
typedef unsigned short int snt;
//using namespace entbaseD;
namespace rendBase {
	struct color{
		chr red, green, blue, alpha;
	};
	GLubyte* clrtoGL(color d) {
		GLubyte bruh[4] = {d.red, d.green, d.blue, d.alpha};
		return (bruh);
	}
	//Screenspace in OpenGL is represented by -1.0f to 1.0f
	//An SDL-OpenGL window object
	struct vertex {
		float x,y,z;
		unt _id;
	};	
	struct vertice { //Connection list between vertex IDs
		vector<unt> cncted_ids;
	};
	class VBOframe { //Vertex Buffer Object Frame
		vector<vertex> verts;
		vector<vertice> connections;
	public:
		VBOframe(vector<vertex> points, vector<vertice> verts) : verts(points), connections(verts) {}
		void addVertex(vertex what, vertice with) {
			verts.push_back(what);
			connections.push_back(with);
		}
		void newSubvertice(unt objID, unt to) {
			//todo
		}
		void removeSubvertice(unt objID, unt to) {
			
		}
		void renderFrame() {
			
		}
	};
	class window {
		SDL_Window* object;
		SDL_Surface* surface;
		snt rldNum, rendNum;
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
			if (c < 0) {
				cout << "Error when initializing Simple Directmedia Layer : " << c << '\n';
			} else {
				if ((this->deepColor) && (this->ultraDeep)) {
					//42-bit mode (expensive!)
					SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 42);
					SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 14);
					SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 14);
					SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 14);
				} else if (this->ultraDeep) {
					//30-bit mode
					SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 30);
					SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 10);
					SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 10);
					SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 10);
				} else if (this->deepColor) {
					//24-bit color mode
					SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
					SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
					SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
					SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
				} else {
					//16-bit color mode
					SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
					SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 4);
					SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 4);
					SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 4);
				}
				SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
				this->object = SDL_CreateWindow( title.c_str() ,SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width,height, SDL_WINDOW_SHOWN);
				if (this->object == NULL) {
					cout << "Error when creating SDL Window!\n";
				} else {
					this->surface = SDL_GetWindowSurface(this->object);
					SDL_FillRect( this->surface,NULL, SDL_MapRGB(this->surface->format, this->SDLclr.red, this->SDLclr.green, this->SDLclr.blue));
					//Red Screen
					SDL_UpdateWindowSurface(this->object);
					cout << "Window and surface initialized. OpenGL-SDL Bridge Variables configured.\n";
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
			this->rldNum++;
			this->refreshSurfaceSDL();
			cout << "Gl Viewport and Identity modes configured!\n";
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
			t.red = (chr)255;
			t.green = (chr)150;
			t.blue = (chr)50;
			t.alpha = (chr)155;
			SDLclr = t;
			makeSDLwindow();
		}
		window(int w, int h, string ti, bool rsc_intens_A, bool rsc_intens_B, bool fScrn, color back, bool autoConfGL) {
			width = w;
			height = h;
			title = ti;
			deepColor = rsc_intens_B;
			ultraDeep = rsc_intens_A;
			fullscreen = fScrn;
			SDLclr = back;
			makeSDLwindow();
			if (autoConfGL) {
				reconfigGL();
			}
		}
		bool GLrender() {
			this->rendNum++;
			return true;
		}
	};
	
}
