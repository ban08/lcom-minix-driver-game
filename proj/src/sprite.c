#include "sprite.h"
#include "video_card.h"
#include <stdlib.h>

Sprite *load_sprite(xpm_map_t xpm) {
    Sprite *sp = malloc(sizeof(Sprite));
    if (!sp) return NULL;

    xpm_image_t img;
    sp->pixels = (uint32_t *) xpm_load(xpm, XPM_8_8_8_8, &img);
    if (!sp->pixels) {
    printf("xpm_load failed!\n");
    free(sp);
    return NULL;
}


    sp->width = img.width;
    sp->height = img.height;
    return sp;
}

void destroy_sprite(Sprite *sprite) {
    if (sprite) 
    {
        free(sprite);
    }
}

int draw_sprite(Sprite *sprite, int x, int y) {
    for (int row = 0; row < sprite->height; row++) {
        for (int col = 0; col < sprite->width; col++) {

            uint32_t pixel = sprite->pixels[row * sprite->width + col];
            if ((pixel >> 24) == 0x00) continue; // transparency
            vg_draw_pixel(x + col, y + row, pixel);
        }
    }
    return 0;
}
