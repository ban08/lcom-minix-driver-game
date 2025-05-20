// Saw video_gr.h from .minix-src\include\lcom\video_gr.h
#include <lcom/lcf.h>
#include <lcom/video_gr.h>

#include <stdint.h>


//void *(vg_init)(uint16_t mode);
int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color){
    for (int i = 0; i <= len; i++){
        //draw pixel
    }
    printf("Inside %s!!!\n", __func__);
    return 1;
}
//int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
//int (vg_exit)(void);
//int (vg_display_vbe_contr_info)(struct vg_vbe_contr_info *info_p);
