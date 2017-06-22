#include <stdio.h>

#define DEBUGMSG(msg) \
		do{ \
			fprintf(stderr, "(%s, %d) %s\n", __FILE__, __LINE__, msg); \
		} while(0);

#define ERRNOMEM "Could not allocate something!"
#define ERRNULL "Pointer's null when it shouldn't be!"

#define FILLRECT(which, a, b, c, d) \
		do{ \
			which.x = a; \
			which.y = b; \
			which.w = c; \
			which.h = d; \
		} while(0);

