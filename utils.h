#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#define DEBUG 1

#if DEBUG
	#define DEBUGMSG(msg) \
			do{ \
				fprintf(stderr, "(%s, %d) %s\n", __FILE__, __LINE__, msg); \
			} while(0);
#else
	#define DEBUGMSG(msg) \
			do{ } while(0);
#endif


#define ERRNOMEM "Could not allocate something!"
#define ERRPARAM "One or more parameters are NULL!"
#define ERRFILE "Could not open/access file!"
#define ERRPOINTER "Pointer is NULL when it shouldn't be!"

#define FILLRECT(which, a, b, c, d) \
		do{ \
			which.x = a; \
			which.y = b; \
			which.w = c; \
			which.h = d; \
		} while(0);

SDL_Texture* UtilsLoadTexture(SDL_Renderer* renderer, const char* path, int* w, int* h);
