#ifndef _NTR_GRAPH_H_
#define _NTR_GRAPH_H_ 1
#include <SDL2/SDL.h>

typedef struct {
	int x,y;
} Pos;
typedef struct {
	float x,y;
} PosF;
typedef struct {
	double x,y;
} PosLF;

#define POS(x, y) ((Pos) { x,y })
#define POSF(x, y) ((PosF) { x,y })
#define POSLF(x, y) ((PosLF) { x,y })

#define POS_ADD(A, B) ((Pos) { (A).x+(B).x, (A).y+(B)+y })
#define POSF_ADD(A, B) ((PosF) { (A).x+(B).x, (A).y+(B).y })
#define POS_ADD2(A, B) { (A).x += (B).x; (A).y += (B).y; }

#define POS_DIST(A, B) sqrt(((B).x-(A).x)*((B).x-(A).x) + ((B).y-(A).y)*((B).y-(A).y))
#define POS_MAGN(A) sqrt((A).x*(A).x + (A).y*(A).y)
#define POS_MUL(V, A) { (V).x*(A), (V).y*(A) }
#define POS_DIV(V, A) { (V).x/(A), (V).y/(A) }

#define TILE_SZ (167)
#define DELAY (1000/60)

extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern int width, height;

/** Initializes graphics, for 
 * deinitialization see 'graphDestroy' */
void graphInit(const char *title, int w, int h);
/** Clears up after 'graphInit' */
void graphDestroy(void);
// Loads texture from BMP file ('fname')
SDL_Texture *textureLoad(const char *fname);

#endif // !_NTR_GRAPH_H_

