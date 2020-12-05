/*
FILE: subzero/base.h
TITLE: SubZER0 Rendering Engine - Base
PURPOSE: Provides essential video rendering and some objects for it.
VERSION: 13 (tested on 4+ Dec 2020)
COMPILATION NOTE: g++ (file) -lSDL2 -lGL -I/usr/local/include/SDL2
NOTE: One error (other than runtime pertaining to points not rendering) is something not deleting properly and throwing a 'munmap_chunk()' pointer error
*/
#define __IMVENG_SUBZERO_BASE
#define __IMVENG_MAJ 0
#define __IMVENG_MIN 3
#define __IMVENG_ADD 3
#define __IMVENG_BUILD 29
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
//DEFINE CONSTANT FOR LATER
#define pi 3.14159
using namespace std;
typedef unsigned char chr;
typedef unsigned int unt; //in Geany, i'm getting something that says a  uint  is already defined, so there
typedef unsigned short int snt;
typedef unsigned long int lnt;
//using namespace entbaseD;
namespace subzero_base {
	namespace err_and_info {
		//Error statuses and "info" blocks (similar to exceptions) ended up being critical to multiple functions.
		enum status {SUBZERO_OPTIMAL, SUBZERO_PASSING, SUBZERO_GL_ENUM, SUBZERO_GL_VALUE, SUBZERO_GL_OPER, \
			SUBZERO_GL_OVER, SUBZERO_GL_UNDER, SUBZERO_GL_MEM, SUBZERO_GL_FRBUF, SUBZERO_GL_CNTXT, SUBZERO_GL_READY, \
			SUBZERO_GL_BAD, SUBZERO_DIV_ZERO, SUBZERO_BAD_INPUT, SUBZERO_BAD_POINTER, SUBZERO_BAD_OPERATION, \
			SUBZERO_BAD_OUTPUT, SUBZERO_SDL_BAD, SUBZERO_SDL_READY, SUBZERO_SDL_INIT_ERR, SUBZERO_SDL_CONF_ERR, \
			SUBZERO_SDL_X11_ERR, SUBZERO_OBJECT_READY, SUBZERO_OBJECT_NONEXISTENT, SUBZERO_OBJECT_INCOMPLETE, \
			SUBZERO_BAD_LENGTH, SUBZERO_UNKNOWN_ERROR};
		enum severity {SEV_GOOD, SEV_MINIMAL, SEV_LOW, SEV_MEDIUM, SEV_HIGH, SEV_CRITICAL};
		//SZ_info is to relay useful information and print error/notification messages
		//SZ_info combines exceptions, debug info parsing and regular function returns
		//Note: for errors alone, use the first constructor and initialize as an snt - don't need to use extra memory
		template<class T> class SZ_info {
			snt section;
			T _data;
			string location, additional, _debug;
			bool extended;
			status what;
			severity level;
		public:
			SZ_info(string loc, snt sect, status type, severity lvl, string info) : level(lvl), section(sect), location(loc), additional(info), what(type) {
				if (lvl == SEV_GOOD) {
					cout << '[' << loc << '-' << sect << ":STABLE] ";
				} else if (lvl == SEV_MINIMAL) {
					cout << '[' << loc << '-' << sect << ":WARNING] ";
				} else if (lvl == SEV_LOW) {
					cout << '[' << loc << '-' << sect << ":ERROR_MINOR] ";
				} else if (lvl == SEV_MEDIUM) {
					cout << '[' << loc << '-' << sect << ":ERROR] ";
				} else if (lvl == SEV_HIGH) {
					cout << '[' << loc << '-' << sect << ":ERROR_SERIOUS] ";
				} else {
					cout << '[' << loc << '-' << sect << ":FAILURE] ";
				}
				cout << info << endl;
			}
			SZ_info(string _l, string _in, string _dbg, snt _lsub, status _ty, severity _lv, bool _print, T _relay) : level(_lv), section(_lsub), location (_l), additional(_in), what(_ty), _data(_relay), _debug(_dbg) {
				extended = true;
				if (_print) {
					if (_lv == SEV_GOOD) {
						cout << '[' << _l << '-' << _lsub << ":STABLE] ";
					} else if (_lv == SEV_MINIMAL) {
						cout << '[' << _l << '-' << _lsub << ":WARNING] ";
					} else if (_lv == SEV_LOW) {
						cout << '[' << _l << '-' << _lsub << ":ERROR_MINOR] ";
					} else if (_lv == SEV_MEDIUM) {
						cout << '[' << _l << '-' << _lsub << ":ERROR] ";
					} else if (_lv == SEV_HIGH) {
						cout << '[' << _l << '-' << _lsub << ":ERROR_SERIOUS] ";
					} else {
						cout << '[' << _l << '-' << _lsub << ":FAILURE] ";
					}
					cout << _in << endl;
				}
			}
			SZ_info() {
				extended = false;
				location = "Unknown";
				section = 0;
				additional = "No Info";
				_debug = "Empty  SZ_Info  Called.";
				what = SUBZERO_OPTIMAL;
				level = SEV_GOOD;
			} //blank
			const snt get_section() {
				return this->section;
			}
			const string get_location() {
				return this->location;
			}
			const string get_info() {
				return this->additional;
			}
			const status get_status() {
				return this->what;
			}
			const severity get_severity() {
				return this->level;
			}
			const T fetch_relay() {
				if (extended) {
					return _data;
				} else {
					return NULL;
				}
			}
		};
		status GLTestError(string sect, int subsect) {
			int code = (int)glGetError();
			/*
			1280 = INVALID ENUM
			1281 = INVALID VALUE
			1282 = INVALID OPERATION
			1283 = STACK OVERFLOW
			1284 = STACK UNDERFLOW
			1285 = OUT OF MEMORY
			1286 = INVALID (FRAME)BUFFER OPERATION
			1287 = OPENGL CONTEXT LOST		
			*/
			switch (code) {
				cout << "{auto}[gl-" << sect << '-' << subsect;
				case 1280 :
					cout << ":ERROR] Invalid Enumeration Constant passed to OpenGL!!\n";
					return SUBZERO_GL_ENUM;
					break;;
				case 1281 :
					cout << ":ERROR_MINOR] Invalid Value or Variable passed to OpenGL!!\n";
					return SUBZERO_GL_VALUE;
					break;;
				case 1282:
					cout << ":ERROR_SERIOUS] Invalid OpenGL Operation!!\n";
					return SUBZERO_GL_OPER;
					break;;
				case 1283:
					cout << ":ERROR_SERIOUS] OpenGL Buffer/Stack has Overflown!!\n";
					return SUBZERO_GL_OVER;
					break;;
				case 1284:
					cout << ":ERROR_SERIOUS] OpenGL Buffer/Stack has Underflown!!\n";
					return SUBZERO_GL_UNDER;
					break;;
				case 1285:
					cout << ":ERROR] OpenGL has run out of Accessible Memory!!\n";
					return SUBZERO_GL_MEM;
					break;;
				case 1286:
					cout << ":ERROR_LOW] Invalid Frame-Buffer Operation requested in OpenGL!!\n";
					return SUBZERO_GL_FRBUF;
					break;;
				case 1287:
					cout << ":ERROR] OpenGL Context has been Corrupted, Moved or Destroyed!!\n";
					return SUBZERO_GL_CNTXT;
					break;;
				case 0:
					return SUBZERO_GL_READY;
					break;;
				default:
					cout << ":ERROR_SERIOUS] Unknown OpenGL Failure - Code " << code << "!!\n";
					return SUBZERO_UNKNOWN_ERROR;
			}
		}
		
		//end namespace
	}
	namespace rendering_base {
		using namespace subzero_base::err_and_info; //provides errors, info and status messages
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
		};
		vertex vtx(float x, float y, float z) {
			vertex n;
			n.x = x;
			n.y = y;
			n.z = z;
			return n;
		}
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
			bool qualityL = false;
			bool qualityH = false;
			bool fullscreen = false;
			bool vsync = true;
			unt msaaOvr = 0;
			bool overrideMSAA = false;
			SZ_info<int> makeSDLwindow() {
				//Makes window open up.
				int c = SDL_Init(SDL_INIT_VIDEO);
				if (c < 0) {
					return SZ_info<int>("sdl-init",0,SUBZERO_SDL_INIT_ERR,SEV_CRITICAL,"Error code #" + to_string(c) + " when starting SDL.");
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
						return SZ_info<int>("sdl-init",1,SUBZERO_SDL_X11_ERR,SEV_CRITICAL,"Error code #" + to_string(c) + " when opening X11 Window and Surface!");
					} else {
						this->surface = SDL_GetWindowSurface(this->object);
						SDL_FillRect( this->surface,NULL, SDL_MapRGB(this->surface->format, this->SDLclr.red, this->SDLclr.green, this->SDLclr.blue));
						SDL_UpdateWindowSurface(this->object);
						this->isclosed = false;
						return SZ_info<int>("sdl-init","Surface and Window initialized successfully.","",2,SUBZERO_SDL_READY,SEV_GOOD,true,c);
					}
				}
			}
			void refreshSurfaceSDL() {
				SDL_UpdateWindowSurface(this->object);
			}
			SZ_info<snt> reconfigGL() {
				SDL_GL_SetSwapInterval(1); //with Vertical Refresh
				cout << "[sdl-gl-run] Reconfiguring Window for OpenGL.\n";
				if ((this->qualityL) && (this->qualityH)) {
					//42-bit Colors and MSAA 4x
					SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 42);
					SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 14);
					SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 14);
					SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 14);
					SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
					SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
					glEnable(GL_MULTISAMPLE);
					cout << "[sdl-gl] Entering Highest Quality - 42b color; MSAA 4x antialias; Multisampling.\n";
				} else if (this->qualityH) {
					//30-bit Colors and MSAA 2x
					SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 30);
					SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 10);
					SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 10);
					SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 10);
					SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
					SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 2);
					glEnable(GL_MULTISAMPLE);
					cout << "[sdl-gl] Entering Higher Quality - 30b color; MSAA 2x antialias; Multisampling.\n";
				} else if (this->qualityL) {
					//24-bit Colors and MSAA 1x
					SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
					SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
					SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
					SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
					SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
					SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 1);
					glEnable(GL_MULTISAMPLE);
					cout << "[sdl-gl] Entering High Quality - 24b color; MSAA 1x antialias; Multisampling.\n";
				} else {
					//16-bit Colors and Linear Antialiasing (default OpenGL)
					SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
					SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 4);
					SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 4);
					SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 4);
					SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 0);
					cout << "[sdl-gl] Entering Normal Quality - 16b color; linear antialias.\n";
				}
				if (this->overrideMSAA) {
					cout << "[gl] Quality Defaults for MSAA Antialiasing Overridden!\n";
					SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
					SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, this->msaaOvr);
				}
				SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
				this->conGL = SDL_GL_CreateContext(this->object);
				SDL_GL_MakeCurrent(this->object, this->conGL);
				glShadeModel(GL_SMOOTH);
				glEnable(GL_PROGRAM_POINT_SIZE);
				glPointSize(1.0);
				glEnable(GL_LINE_SMOOTH);
				glEnable(GL_DEPTH_TEST);
				glDepthFunc(GL_LESS);
				glDisable(GL_CULL_FACE);
				//glCullFace(GL_BACK);
				//glFrontFace(GL_CCW);
				//glEnable(GL_CULL_FACE);
				glClearColor(0.8,0.8,0.7,0.5); //a nice, light gray
				glViewport(0, 0, this->width, this->height);
				//glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				this->rldNum++;
				SDL_GL_SwapWindow(this->object);
				this->glswapped = true;
				glFlush();
				status bruhchacho = GLTestError("gl-conf",6);
				if (bruhchacho != SUBZERO_GL_READY) {
					return SZ_info<snt>("gl-conf",6,SUBZERO_GL_BAD,SEV_HIGH,"Error when configuring GL!");
				} else {
					return SZ_info<snt>("gl-conf",6,SUBZERO_GL_READY,SEV_GOOD,"GL Configured successfully.");
				}
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
			window(int w, int h, string ti, bool rsc_intens_A, bool rsc_intens_B, bool fScrn, color back, bool autoConfGL, bool vert_sync) {
				width = w;
				height = h;
				title = ti;
				vsync = vert_sync;
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
					glDisable(GL_PROGRAM_POINT_SIZE);
					glDisable(GL_LINE_SMOOTH);
					glDisable(GL_DEPTH_TEST);
					glFinish();
					status bruhchacho = GLTestError("gl-exit",0);
					if (bruhchacho != SUBZERO_GL_READY) {
						SZ_info<snt> t("gl-exit",0,SUBZERO_GL_BAD,SEV_MEDIUM,"Error when exiting GL!");
					} else {
						SZ_info<snt> t("gl-exit",0,SUBZERO_GL_READY,SEV_GOOD,"Exited GL and deleted context.");
					}
					SDL_GL_DeleteContext(this->conGL);
					this->glswapped = false;
				} else {
					SZ_info<snt> t("gl-exit",0,SUBZERO_OPTIMAL,SEV_GOOD,"No GL Contexts should exist. Proceeding.");
				}
				if (this->isclosed == false) {
					SDL_DestroyWindow( this->object);
					SDL_Quit();
					this->isclosed = true;
					SZ_info<snt> t("sdl-exit",0,SUBZERO_OPTIMAL,SEV_GOOD,"Closed SDL Window.");
				} else {
					SZ_info<snt> t("sdl-exit",0,SUBZERO_PASSING,SEV_MINIMAL,"SDL Window not open, but close() was called.");
				}
			}
			void GLswap() {
				SDL_GL_SwapWindow(this->object);
				SDL_UpdateWindowSurface(this->object);
				this->rendNum++;
			}
			~window() {
				if (this->isclosed == false) {
					cout << "[sdl-exit-0:WARN] Deprecated call to in-destructor window closing.\n";
					this->close();
				}
				cout << "[subzero] Deleted Window object.\n";
			}
		};
		//A Vertex Buffer Object ~= Vertice (VBOs combine into a VAO)
		class vertice {
		public:
			lnt pointCount = 3;
			unt VBOid = 0;
			float* convPoints[];
			void loadPoints(vector<vertex> points) {
				lnt pos = 0;
				this->pointCount = 0;
				for (int i=0;i<points.size() - 1;i++) {
					pos++;
					convPoints[pos] = &points.at(i).x;
					pos++;
					convPoints[pos] = &points.at(i).y;
					pos++;
					convPoints[pos] = &points.at(i).z;
					this->pointCount++;
				}
			}
			SZ_info<snt> bufferAsVBO() { //This must be buffered before it is buffered by a shaded_collection VAO!
				GLuint vbo = VBOid;
				glGenBuffers(1, &vbo);
				glBindBuffer(GL_ARRAY_BUFFER, vbo);
				status bruhchacho = GLTestError("gl-vbo-buffer",0);
				if (bruhchacho != SUBZERO_GL_READY) {
					return SZ_info<snt>("gl-vbo-buffer",0,SUBZERO_GL_BAD,SEV_CRITICAL,"Could not Bind VBO Buffers!");
				}
				//Uploads VBO to the GPU
				glBufferData(GL_ARRAY_BUFFER, (3 * pointCount) * sizeof(float), &convPoints[0], GL_STATIC_DRAW);
				if (GLTestError("gl-vbo-buffer",1) != SUBZERO_GL_READY) {
					return SZ_info<snt>("gl-vbo-buffer",1,SUBZERO_GL_BAD,SEV_CRITICAL,"Could not Upload Data to Bound VBO Buffer!");
				} else {
					return SZ_info<snt>("gl-vbo-buffer",1,SUBZERO_GL_READY,SEV_GOOD,"Data uploaded to bound VBO buffer without error.");
				}
			}
			vertice(vector<vertex> pntsToDraw, unt VBO) {
				loadPoints(pntsToDraw);
				VBOid = VBO;
				bufferAsVBO();
			}
		};
		//Vertex Array Object equivalent
		class shaded_collection {
		public:
			vector<vertice> verts;
			unt frameID = 0;
			SZ_info<snt> setup() {
				GLuint vao = this->frameID;
				glGenVertexArrays(1, &vao);
				glBindVertexArray(vao);
				status bruhchacho = GLTestError("gl-setup-vao",0);
				if (bruhchacho != SUBZERO_GL_READY) {
					return SZ_info<snt>("gl-setup-vao",0,SUBZERO_GL_BAD,SEV_CRITICAL,"Could not bind VAO!");
				}
				glEnableVertexAttribArray(vao);
				glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE, 0, NULL);
				glLoadIdentity();
				bruhchacho = GLTestError("gl-setup-vao",1);
				if (bruhchacho != SUBZERO_GL_READY) {
					return SZ_info<snt>("gl-setup-vao",1,SUBZERO_GL_BAD,SEV_CRITICAL,"Could not enable VAO Pointer!");
				} else {
					return SZ_info<snt>("gl-setup-vao",1,SUBZERO_GL_READY,SEV_GOOD,"Vertex Array bound and enabled successfully.");
				}
			}
			SZ_info<snt> loadChildBuffers() {
				for (int i = 0; i < verts.size() - 1; i++) {
					GLuint vbo = verts.at(i).VBOid;
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);
					glBindBuffer(GL_ARRAY_BUFFER, vbo);
					status bruhchacho = GLTestError("gl-load-vao",0);
					if (bruhchacho != SUBZERO_GL_READY) {
						return SZ_info<snt>("gl-load-vao",0,SUBZERO_GL_BAD,SEV_MEDIUM,"Could not load VBO into VAO!");
					}
				}
				glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE, 0, NULL);
				status bruhchacho = GLTestError("gl-load-vao",1);
				if (bruhchacho != SUBZERO_GL_READY) {
					return SZ_info<snt>("gl-load-vao",1,SUBZERO_GL_BAD,SEV_HIGH,"Could not enable new VAO Pointer!");
				} else {
					return SZ_info<snt>("gl-load-vao",1,SUBZERO_GL_READY,SEV_GOOD,"VBOs loaded and bound to VAO.");
				}
			}
			SZ_info<snt> buffer() {
				SZ_info<snt> g = this->setup();
				if (g.get_severity() != SEV_GOOD) {
					return SZ_info<snt>("gl-buffer-vao",0,SUBZERO_GL_BAD,SEV_CRITICAL,"Setup of VAO failed!");
				}
				//Ensures all VBOs are buffered!
				for (int i =0; i < this->verts.size() - 1; i++) {
					SZ_info<snt> t = this->verts.at(i).bufferAsVBO();
					if (t.get_severity() != SEV_GOOD) {
						return SZ_info<snt>("gl-buffer-vao",1,SUBZERO_GL_BAD,SEV_CRITICAL,"Buffering of VBO #" + to_string(i) + " failed!");
					}
				}
				SZ_info<snt> oh = this->loadChildBuffers();
				if (oh.get_severity() != SEV_GOOD) {
					return SZ_info<snt>("gl-buffer-vao",2,SUBZERO_UNKNOWN_ERROR,SEV_HIGH,"Could not finish child VBO buffering!");
				} else {
					status bruhchacho = GLTestError("gl-buffer-vao",3);
					if (bruhchacho != SUBZERO_GL_READY) {
						return SZ_info<snt>("gl-buffer-vao",3,SUBZERO_UNKNOWN_ERROR,SEV_CRITICAL,"Unexpected OpenGL Error!");
					} else {
						return SZ_info<snt>("gl-buffer-vao",3,SUBZERO_GL_READY,SEV_GOOD,"VAO Fully Buffered.");
					}
				}
			}
			SZ_info<snt> render(GLuint shading, window &drawTo) {
				GLuint vao = frameID;
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				status bruhchacho = GLTestError("gl-vao-render",0);
				if (bruhchacho != SUBZERO_GL_READY) {
					return SZ_info<snt>("gl-vao-render",0,SUBZERO_GL_BAD,SEV_MEDIUM,"OpenGL Clear/Previous Operation error?");
				}
				glUseProgram(shading);
				//I got error code 1282 when running this - SOLVED: Compiled Shaders w/ error checking *before* any OpenGL Execution processes.
				bruhchacho = GLTestError("gl-vao-render",1);
				if (bruhchacho != SUBZERO_GL_READY) {
					return SZ_info<snt>("gl-vao-render",1,bruhchacho,SEV_CRITICAL,"OpenGL failed to load Shading Program!");
				}
				glEnableVertexAttribArray(vao);
				glBindVertexArray(vao);
				bruhchacho = GLTestError("gl-vao-render",2);
				if (bruhchacho != SUBZERO_GL_READY) {
					return SZ_info<snt>("gl-vao-render",2,bruhchacho,SEV_HIGH,"OpenGL could not bind loaded VAO!");
				}
				lnt start = 0;
				for (int i=0;i<this->verts.size() - 1;i++) {
					lnt count = this->verts.at(i).pointCount;
					//check if "start" should have 'count' added to it.
					glDrawArrays(GL_TRIANGLE_STRIP, start, count);
					bruhchacho = GLTestError("gl-vao-render",3);
					if (bruhchacho != SUBZERO_GL_READY) {
						return SZ_info<snt>("gl-vao-render",3,bruhchacho,SEV_MEDIUM,"Unexpected error when drawing TRIANGLE_STRIP VAO.");
					}
					drawTo.GLswap();
					start+=count;
				}
				return SZ_info<snt>();
			}
			SZ_info<snt> unbind() {
				glEnableVertexAttribArray(0);
				glDisableVertexAttribArray(this->frameID);
				glBindVertexArray(0);
				if (GLTestError("gl-vao-unbind",0) != SUBZERO_GL_READY) {
					return SZ_info<snt>("gl-vao-unbind",0,SUBZERO_UNKNOWN_ERROR,SEV_CRITICAL,"Unexpected OpenGL Error!");
				} else {
					return SZ_info<snt>("gl-vao-unbind",0,SUBZERO_GL_READY,SEV_GOOD,"Unbound and disabled Vertex Array.");
				}
			}
			shaded_collection(vector<vertice> VBOs, unt VAO, bool runBuffer) {
				verts = VBOs;
				frameID = VAO;
				if (runBuffer) {buffer();}
			}
		};
	}
}

//test range start

//i despise this plaintext shading language.
//i really do.
//why are colors FLOATS of 0 to 1?????? AAAAAA
using namespace subzero_base;
const char* testing_vertshade = "#version 400\nin vec3 point;\nvoid main() {\ngl_Position = vec4(vp, 1.0);\n}";
const char* testing_fragshade = "#version 400\nout vec4 shade;\nvoid main() {\nshade=vec4(0.1,0.1,0.7,1.0);\n}";
GLuint vertShade, fragShade, shadingProgram;
using namespace err_and_info;
SZ_info<snt> compileShaders() {
	bool err = false;
	vertShade = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertShade, 1, &testing_vertshade, NULL);
	if (GLTestError("gl-shader-compile",0) != SUBZERO_GL_READY) {
		return SZ_info<snt>("gl-shader-compile",0,SUBZERO_UNKNOWN_ERROR,SEV_CRITICAL,"Unexpected OpenGL Shader Error!");
	} else {
		SZ_info<snt> t("gl-shader-compile",0,SUBZERO_GL_READY,SEV_GOOD,"Created empty shader (vertex).");
	}
	glCompileShader(vertShade);
	if (GLTestError("gl-shader-compile",1) != SUBZERO_GL_READY) {
		return SZ_info<snt>("gl-shader-compile",1,SUBZERO_GL_BAD,SEV_CRITICAL,"OpenGL was unable to compile Vertex Shader.");
	} else {
		SZ_info<snt> t("gl-shader-compile",1,SUBZERO_GL_READY,SEV_GOOD,"Successfully compiled Vertex Shader.");
	}
	fragShade = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShade, 1, &testing_fragshade, NULL);
	if (GLTestError("gl-shader-compile",2) != SUBZERO_GL_READY) {
		return SZ_info<snt>("gl-shader-compile",2,SUBZERO_UNKNOWN_ERROR,SEV_CRITICAL,"Unexpected OpenGL Shader Error!");
	} else {
		SZ_info<snt> t("gl-shader-compile",2,SUBZERO_GL_READY,SEV_GOOD,"Created empty shader (fragment).");
	}
	glCompileShader(fragShade);
	if (GLTestError("gl-shader-compile",3) != SUBZERO_GL_READY) {
		return SZ_info<snt>("gl-shader-compile",3,SUBZERO_GL_BAD,SEV_CRITICAL,"OpenGL was unable to compile Fragment Shader.");
	} else {
		SZ_info<snt> t("gl-shader-compile",3,SUBZERO_GL_READY,SEV_GOOD,"Successfully compiled Vertex Shader.");
	}
	shadingProgram = glCreateProgram();
	glAttachShader(shadingProgram, fragShade);
	glAttachShader(shadingProgram, vertShade);
	if (GLTestError("gl-shader-compile",4) != SUBZERO_GL_READY) {
		return SZ_info<snt>("gl-shader-compile",4,SUBZERO_OBJECT_INCOMPLETE,SEV_CRITICAL,"OpenGL was unable to attach shaders to program.");
	} else {
		SZ_info<snt> t("gl-shader-compile",4,SUBZERO_GL_READY,SEV_GOOD,"Successfully linked shaders to program.");
	}
	glLinkProgram(shadingProgram);
	if (GLTestError("gl-shader-compile",5) != SUBZERO_GL_READY) {
		return SZ_info<snt>("gl-shader-compile",5,SUBZERO_OBJECT_NONEXISTENT,SEV_CRITICAL,"Failed to link Shader Program.");
	} else {
		return SZ_info<snt>("gl-shader-compile",5,SUBZERO_GL_READY,SEV_GOOD,"Successfully linked shader program.");
	}
}
void runtime_A() {
	/*
	RUNTIME INFORMATION:
	Delta = 1ms
	Max Runtime = 2500ms
	Startup = Initialize Window (method 1) and Do Not Configure GL
	Cleanup = Close Window
	*/
	cout << "[run] Began runtime A\n";
	rendering_base::window nyet(500,500,"Wintest",true,false,false);
	unt tep = 0;
	while (tep < 500) {
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
	Max Runtime = 1500ms
	Startup = Initialize Window (method 2) With Different Background and Configured OpenGL
	Cleanup = None
	*/
	cout << "[run] Began runtime B\n";
	unt tep = 0;
	rendering_base::window bruh(850,100,"uh oh",true,false,false,rendering_base::cl(0xFF,0x35,0x05),true,true);
	while (tep < 1500) {
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
	Startup = Initialize Window (manually) and Configure OpenGL as well as a Triangle Shape
	Cleanup = Unbind Shaded_Collection
	*/
	cout << "[run] Began runtime C\n";
	unt tep = 0;
	rendering_base::window t;
	t.width = 500;
	t.height = 300;
	t.SDLclr = rendering_base::cl(0x00, 0x10, 0xFF);
	t.makeSDLwindow();
	t.refreshSurfaceSDL();
	t.reconfigGL();
	//I'm able to get it to work UP TO drawing; the SDL-GL bridge functions.
	//However, I cannot get the openGL VBOs and VAOs to do anything, which is a bit critical.
	rendering_base::vertex A = rendering_base::vtx(0.0, 1.5, 1.0);
	rendering_base::vertex B = rendering_base::vtx(1.5, 0.0, 1.0);
	rendering_base::vertex C = rendering_base::vtx(-1.5, 0.0, 1.0);
	vector<rendering_base::vertex> verts = {A, B, C};
	cout << "[run-note] Generated Vertexes.\n";
	rendering_base::vertice bruh(verts,0);
	vector<rendering_base::vertice> collection = {bruh};
	rendering_base::shaded_collection frame(collection, 0, true);
	while (tep < 3500) {
		//remember: GL renders per-frame
		frame.render(shadingProgram, t);
		tep+=1;
		SDL_Delay(1);
	}
	frame.unbind();
	cout << "[run] Ended runtime C\n";
}
//NOTE: there is an 'munmap_chunk()' error between the end of 'runtime_C()' and 'cleanup()'
void cleanup() {
	cout << "[run] Cleaning up.\n";
	glDeleteProgram(shadingProgram);
}
void prerun() {
	cout << "ImmersivEngine Build #" << __IMVENG_BUILD << " Version " << __IMVENG_MAJ << "." << __IMVENG_MIN << ":" << __IMVENG_ADD << endl;
	compileShaders();
}
int main() {
	prerun();
	runtime_A();
	SDL_Delay(1);
	runtime_B();
	SDL_Delay(1);
	runtime_C();
	//When we delete a window we call SDL_Quit; can't use SDL_Delay after that.
	//SDL_Delay(1);
	cleanup();
	return 0;
}
