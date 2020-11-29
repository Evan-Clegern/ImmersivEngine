/*
FILE: subzero/base.h
TITLE: SubZER0 Rendering Engine - Base
PURPOSE: Provides essential video rendering and some objects for it.
VERSION: 6 (tested on 25 Nov 2020)
COMPILATION NOTE: g++ (file) -lSDL2 -lGL -I/usr/local/include/SDL2
*/
#define __IMVENG_SUBZERO_BASE
#define GL_GLEXT_PROTOTYPES //supposedly needed for GL BUFFERS
//Import Simple Directmedia Layer for windowing and 2D overlays
#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_cpuinfo.h>
#include <SDL2/SDL_hints.h>
//Import OpenGL
#include <GL/gl.h>
#include <GL/glext.h> //Buffers and the sort.
//Import standard C++ modules
#include <iostream> //Console logs
#include <string>
#include <vector>
#include <cmath>
//DEFINE CONSTANTS
#define pi 3.14159
using namespace std;
typedef unsigned char chr;
typedef unsigned int unt; //in Geany, i'm getting something that says a  uint  is already defined, so there
typedef unsigned short int snt;
typedef unsigned long int lnt;
//using namespace entbaseD;
namespace rendBase {
	struct color{
		chr red, green, blue, alpha;
	};
	color cl(chr red, chr green, chr blue) {
		color t;
		t.red = red;
		t.green = green;
		t.blue = blue;
		t.alpha = 0xFF;
		return t;
	}
	color cla(chr red, chr green, chr blue, chr alpha) {
		color t;
		t.red = red;
		t.blue = blue;
		t.alpha = alpha;
		t.green = green;
		return t;
	}
	//Screenspace in OpenGL is represented by -1.0f to 1.0f
	//An SDL-OpenGL window object
	struct vertex {
		float x, y, z;
		unt _id;
	};
	//A Vertex Buffer Object ~= Vertice (VBOs combine into a VAO)
	class vertice {
	public:
		vector<unt> cncted_ids;
		lnt pointCount = 3;
		unt VBOid = 0;
		float* convPoints[];
		void loadPoints(vector<vertex> points) {
			lnt pos = 0;
			for (int i=0;i<points.size() - 1;i++) {
				pos++;
				//it wants to convert float* to float ????????
				convPoints[pos] = &points.at(i).x;
				pos++;
				convPoints[pos] = &points.at(i).y;
				pos++;
				convPoints[pos] = &points.at(i).z;
			}
		}
		void bufferAsVBO() {
			GLuint vbo = VBOid;
			glGenBuffers(1, &vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, (3 * pointCount) * sizeof(float), convPoints, GL_STATIC_DRAW);
		}
	};
	//Vertex Array Object
	class frame {
		vector<vertice> verts;
		unt frameID = 0;
		void bufferFrameAndVBOs() {
			GLuint vao = frameID;
			glGenVertexArrays(1, &vao);
			glBindVertexArray(vao);
			glEnableVertexAttribArray(0);
			for (int i = 0; i < verts.size() - 1; i++) {
				glBindBuffer(GL_ARRAY_BUFFER, verts.at(i).VBOid);
			}
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		}
	};
	class window {
		SDL_Window* object;
		SDL_Surface* surface;
		SDL_GLContext conGL;
		bool isclosed = true;
		bool glswapped = false;
		snt rldNum, rendNum;
	public:
		int width, height;
		string title = "ImmersivEngine";
		color SDLclr;
		bool qualityL = true;
		bool qualityH = false;
		bool fullscreen = false;
		bool vsync = true;
		bool depthTest = true;
		unt msaaOvr = 0;
		bool overrideMSAA = false;
		short int makeSDLwindow() {
			//Makes window open up.
			int c = SDL_Init(SDL_INIT_VIDEO);
			if (c < 0) {
				cout << "[sdl] Error when initializing Simple Directmedia Layer : " << c << '\n';
			} else {
				SDL_SetHint("SDL_HINT_RENDER_DRIVER", "opengl");
				SDL_SetHint("SDL_HINT_RENDER_SCALE_QUALITY", "linear"); //best filtering mode provided by openGL
				SDL_SetHint("SDL_HINT_RENDER_OPENGL_SHADERS", "1"); //enable OpenGL shaders
				SDL_SetHint("SDL_HINT_GRAB_KEYBOARD", "1");
				if (this->vsync) {
					SDL_SetHint("SDL_HINT_RENDER_VSYNC", "1");
				} else {
					SDL_SetHint("SDL_HINT_RENDER_VSYNC", "0");
					//GL-side?
				}
				if (this->fullscreen) {
					this->object = SDL_CreateWindow( title.c_str() ,SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width,height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN);
				} else {
					this->object = SDL_CreateWindow( title.c_str() ,SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width,height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
				}
				if (this->object == NULL) {
					cout << "[sdl] Error when creating SDL Window!\n";
				} else {
					this->surface = SDL_GetWindowSurface(this->object);
					SDL_FillRect( this->surface,NULL, SDL_MapRGB(this->surface->format, this->SDLclr.red, this->SDLclr.green, this->SDLclr.blue));
					//Red Screen
					SDL_UpdateWindowSurface(this->object);
					this->isclosed = false;
					cout << "[sdl] Window and surface initialized.\n";
				}
			}
			return c;
		}
		void refreshSurfaceSDL() {
			SDL_UpdateWindowSurface(this->object);
		}
		void reconfigGL() {
			if ((this->qualityL) && (this->qualityH)) {
				//42-bit Colors and MSAA 4x
				SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 42);
				SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 14);
				SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 14);
				SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 14);
				SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
				SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
				cout << "[sdl-gl] Entering Highest Quality - 42b color; MSAA 4x antialias.\n";
			} else if (this->qualityH) {
				//30-bit Colors and MSAA 2x
				SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 30);
				SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 10);
				SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 10);
				SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 10);
				SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
				SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 2);
				cout << "[sdl-gl] Entering Higher Quality - 30b color; MSAA 2x antialias.\n";
			} else if (this->qualityL) {
				//24-bit Colors and MSAA 1x
				SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
				SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
				SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
				SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
				SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
				SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 1);
				cout << "[sdl-gl] Entering High Quality - 24b color; MSAA 1x antialias.\n";
			} else {
				//16-bit Colors and Linear Antialiasing (default OpenGL)
				SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
				SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 4);
				SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 4);
				SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 4);
				SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 0);
				cout << "[sdl-gl] Entering Low Quality - 16b color; linear antialias.\n";
			}
			if (this->overrideMSAA) {
				cout << "[gl] Quality Defaults for MSAA Antialiasing Overridden!\n";
				SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
				SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, this->msaaOvr);
			}
			if (this->depthTest) {
				glEnable(GL_DEPTH_TEST);
				glDepthFunc(GL_LESS);
				cout << "[gl] Enabled 'less' Depth Test mode.\n";
			}
			SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
			float ratio = (float)this->width;
			ratio/=(float)this->height;
			glShadeModel(GL_SMOOTH);
			glCullFace(GL_BACK);
			glFrontFace(GL_CCW);
			glEnable(GL_CULL_FACE);
			glPointSize(1.0);
			glClearColor(0,0,0,0);
			glViewport(0, 0, this->width, this->height);
			glMatrixMode(GL_PROJECTION);
			this->conGL = SDL_GL_CreateContext(this->object);
			glLoadIdentity();
			SDL_GL_SwapWindow(this->object);
			SDL_GL_MakeCurrent(this->object, this->conGL);
			glClear(GL_COLOR_BUFFER_BIT);
			this->rldNum++;
			this->refreshSurfaceSDL();
			this->glswapped = true;
			cout << "[gl] GL Viewport, Attributes and Identity modes configured!\n";
		}
		//Easy constructor 
		//Higher + High Quality = Highest Quality (42-bit colors and MSAA 4x Antialiasing)
		window(int w, int h, string ti, bool higher_quality, bool high_quality, bool fScrn) {
			width = w;
			height = h;
			title = ti;
			qualityL = higher_quality;
			qualityH = high_quality;
			fullscreen = fScrn;
			color t;
			t.red = (chr)255;
			t.green = (chr)150;
			t.blue = (chr)50;
			t.alpha = (chr)155;
			SDLclr = t;
			makeSDLwindow();
		}
		window(int w, int h, string ti, bool rsc_intens_A, bool rsc_intens_B, bool fScrn, color back, bool autoConfGL, bool vert_sync, bool depthrend) {
			width = w;
			height = h;
			title = ti;
			vsync = vert_sync;
			depthTest = depthrend;
			qualityL = rsc_intens_B;
			qualityH = rsc_intens_A;
			fullscreen = fScrn;
			SDLclr = back;
			makeSDLwindow();
			if (autoConfGL) {
				reconfigGL();
			}
		}
		window(int w, int h) {
			width = w;
			height = h;
			color t;
			t.red = (chr)255;
			t.green = (chr)150;
			t.blue = (chr)50;
			t.alpha = (chr)155;
			SDLclr = t;
			makeSDLwindow();
		}
		window() {}
		void close() {
			if (this->glswapped) {
				SDL_GL_DeleteContext(this->conGL);
				this->glswapped = false;
			}
			if (this->isclosed == false) {
				SDL_DestroyWindow( this->object);
				SDL_Quit();
				cout << "[sdl-gl] GL Viewport and SDL Window closed!\n";
				this->isclosed = true;
			} else {
				cout << "[sdl-gl] Window already closed.\n";
			}
		}
		void GLrender() {
			this->rendNum++;
			SDL_GL_SwapWindow(this->object);
			SDL_GL_MakeCurrent(this->object, this->conGL);
		}
		~window() {
			if (this->isclosed == false) {
				cout << "[sdl] Close function called thru destructor.\n";
				this->close();
			} else {
				cout << "[sdl] Destroyed closed window.\n";
			}
		}
	};
}
//Test Code
void runtime_A() {
	/*
	RUNTIME INFORMATION:
	Delta = 1ms
	Max Runtime = 2800ms
	Startup = Initialize Window (method 1) and Do Not Configure GL
	Cleanup = Close Window
	*/
	cout << "[run] Began runtime A\n";
	rendBase::window nyet(500,500,"GL test",true,false,false);
	unt tep = 0;
	while (tep < 2800) {
		tep++;
		SDL_Delay(1);
	}
	nyet.close();
	cout << "[run] Ended runtime A\n";
}
void runtime_B() {
	/*
	RUNTIME INFORMATION:
	Delta = 2ms
	Max Runtime = 4500ms
	Startup = Initialize Window (method 2) With Different Background and Configured OpenGL
	Cleanup = None
	*/
	cout << "[run] Began runtime B\n";
	unt tep = 0;
	rendBase::window bruh(850,100,"uh oh",true,false,false,rendBase::cl(0xFF,0x35,0x05),true,true,false);
	while (tep < 4500) {
		tep+=2;
		SDL_Delay(2);
	}
	cout << "[run] Ended runtime B\n";
}
void runtime_C() {
	/*
	RUNTIME INFORMATION:
	Delta = 1ms
	Max Runtime = 3500ms
	Startup = Initialize Window (manually) and Configure OpenGL, and then draw a Triangle.
	Cleanup = None
	*/
	cout << "[run] Began runtime C\n";
	unt tep = 0;
	rendBase::window t;
	t.width = 500;
	t.height = 300;
	t.SDLclr = rendBase::cl(0x00, 0x10, 0xFF);
	t.makeSDLwindow();
	t.reconfigGL();
	t.refreshSurfaceSDL();
	while (tep < 3500) {
		tep+=1;
		SDL_Delay(1);
	}
	cout << "[run] Ended runtime C\n";
}
//Note: OpenGL, when fully configured but given nothing to display, "screenshots" whatever is in its position.
int main() {
	runtime_A();
	SDL_Delay(1);
	runtime_B();
	runtime_C();
	return 0;
}
