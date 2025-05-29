#ifndef SPRITE_H
#define SPRITE_H
#include "sprites/start.xpm"


#include <lcom/lcf.h>

typedef struct {
    uint32_t *pixels;
    int width, height;
} Sprite;


xpm_image_t start_img;



static const char * test_xpm[] = {
"2 2 2 1",
"  c #FFFFFF",
". c #000000",
" .",
". "
};

/* dino.xpm */
static const char *dino_xpm[] = {
"16 16 3 1",
"  c #FFFFFF",
". c #00FF00",
"X c #000000",
"                ",
"     ....       ",
"    ......      ",
"   .........    ",
"   ...XXX...    ",
"   ..XXXXX..    ",
"   ..X...X..    ",
"   ..X...X..... ",
"   ..X...XXXXXX.",
"   ..X........X.",
"   ..XX.......X.",
"   ...XX.....XX.",
"    ...XXXXXXX..",
"     ..X...X....",
"     .X....X....",
"     ......     "
};



Sprite *load_sprite(xpm_map_t xpm);
void destroy_sprite(Sprite *sp);
int draw_sprite(Sprite *sp, int x, int y);

#endif
