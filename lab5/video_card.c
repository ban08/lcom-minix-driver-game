#include <lcom/lcf.h>
#include "video_card.h"
#include <math.h>

vbe_mode_info_t mode_info;


uint8_t* frame_buffer;


#include <lcom/lab5.h>
#include <lcom/vbe.h>
#include <lcom/utils.h>

int vg_exit() {
    return set_text_mode();
}

int vbe_get_mode_info(uint16_t mode, vbe_mode_info_t *vmi_p) {
  mmap_t mapa;

  reg86_t r;

  int to_alloc = sizeof(vbe_mode_info_t);

  if (lm_alloc(to_alloc, &mapa) == NULL) 
  {


    return 1;
  }

  memset(&r, 0, sizeof(r));

  r.ax = 0x4F01;

  r.cx = mode;

  r.es = PB2BASE(mapa.phys);

  r.di = PB2OFF(mapa.phys);

  r.intno = 0x10;

  if (sys_int86(&r) != 0) 
  {
    lm_free(&mapa);


    return 1;
  }

  if (r.ah != 0x00) 
  {
    lm_free(&mapa);
    return 1;
  }

  memcpy(vmi_p, mapa.virt, sizeof(vbe_mode_info_t));

  lm_free(&mapa);

  return 0;
}



int (set_graphic_mode)(uint16_t s_mode) {
    reg86_t regs;

    int regs_size = sizeof(regs);

    memset(&regs, 0, regs_size);

    regs.intno = 0x10;

    regs.ah = 0x4F; //"high byte"

    regs.al = 0x02; //"low byte"

    regs.bx = 1 << 14 | s_mode;

    if (sys_int86(&regs) == 0) {

        return 0;
    }

    printf("Function set_graphic_mode failed!\n");

    return 1;
}



int (set_text_mode)() {
    
    reg86_t reg;

    int regs_size = sizeof(reg);

    memset(&reg, 0, regs_size);

    reg.intno = 0x10;

    reg.ah = 0x00; // "high byte"
    reg.al = 0x03; //"low byte"
    
    reg.bx = 0x0000;


    if(sys_int86(&reg) == 0) {

        return 0;
    }


    printf("Function set_text_mode failed!\n");

    return 1;
}



int (set_frame_buffer)(uint16_t mode){

    size_t info_size = sizeof(mode_info);
    
    memset(&mode_info, 0, info_size);

    if (vbe_get_mode_info(mode, &mode_info) != 0) 
    {

        return 1;
    }

    uint32_t vertical_res = mode_info.YResolution;

    uint32_t horizontal_res = mode_info.XResolution;

    uint32_t bpp  = (mode_info.BitsPerPixel + 7) / 8;

    uint32_t f_size = horizontal_res * vertical_res * bpp;

    struct minix_mem_range mem_range;

    mem_range.mr_base = mode_info.PhysBasePtr;

    mem_range.mr_limit = mem_range.mr_base + f_size;

    if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mem_range) != 0) 
    {

        printf("Error during memory mapping!\n");

        return 1;
    }

    frame_buffer = vm_map_phys(SELF, (void*) mem_range.mr_base, f_size);

    if (frame_buffer != NULL) 
    {
        return 0; //success

    }

    printf("Error during memory mapping!\n");

    return 1;
}


int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color) {

  if(x > mode_info.XResolution) 
  {

    return 1;
  }
  if (y > mode_info.YResolution)
  {
    return 1;
  }
  
  unsigned bpp = (mode_info.BitsPerPixel + 7) / 8;

  unsigned int idx = (mode_info.XResolution * y + x) * bpp;

  if (memcpy(&frame_buffer[idx], &color, bpp) != NULL)
  {
    return 0; //success
  }

  return 1;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {

  for (unsigned iter = 0 ; iter < len ; ++iter)
  {

    if (vg_draw_pixel(x + iter, y, color) != 0) 
    {

        return 1;
    }
  }
  return 0; //desenhou a linha horizontal toda sem erros
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  for(unsigned iter = 0; iter < height ; iter++)
  {
    if (vg_draw_hline(x, y + iter, width, color) != 0) 
    {
      vg_exit();
      return 1;
    }
  }

  return 0;
}

int (print_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {

  xpm_image_t imagem;

  uint8_t *cores = xpm_load(xpm, XPM_INDEXED, &imagem);

  int alt = imagem.height;

  int larg = imagem.width;

  for (int h_iter = 0 ; h_iter < alt ; ++h_iter) 
  {
    for (int w_iter = 0 ; w_iter < larg ; ++w_iter) 
    {
      if (vg_draw_pixel(x + w_iter, y + h_iter, *cores) != 0)
      {
        return 1;
      }
      cores++; 
    }
  }

  return 0;


}


int normalize_color(uint32_t color, uint32_t *new_color) {
  int bpp = mode_info.BitsPerPixel;
  
  if (bpp == 32)
  {
    *new_color = color;

  } else 
  {
    *new_color = color & ((1 << bpp) - 1);


  }
  return 0;
}


uint32_t get_direct_color(uint32_t red, uint32_t green, uint32_t blue) {

  return (red << mode_info.RedFieldPosition) |
    (green << mode_info.GreenFieldPosition) |
    (blue << mode_info.BlueFieldPosition);
}

uint32_t get_indexed_color(uint16_t col, uint16_t row, uint8_t step, uint32_t first, uint8_t n) {
  uint32_t index = row * n + col;
  uint32_t max_color = 1 << mode_info.BitsPerPixel;
  return (first + index * step) % max_color;
}

uint32_t get_red_component(unsigned col_index, uint8_t step, uint32_t first_color) {
  uint32_t base_red = extract_red(first_color);
  uint32_t max_red = 1 << mode_info.RedMaskSize;
  return (base_red + col_index * step) % max_red;
}

uint32_t get_green_component(unsigned row_index, uint8_t step, uint32_t first_color) {
    uint32_t base_green = extract_green(first_color);
    uint32_t max_green = 1 << mode_info.GreenMaskSize;
    return (base_green + row_index * step) % max_green;
}

uint32_t get_blue_component(unsigned col_index, unsigned row_index, uint8_t step, uint32_t first_color) {
    uint32_t base_blue = extract_blue(first_color);
    uint32_t max_blue = 1 << mode_info.BlueMaskSize;
    return (base_blue + (row_index + col_index) * step) % max_blue;
}

uint32_t extract_red(uint32_t color) {
    uint32_t mask = (1 << mode_info.RedMaskSize) - 1;
    return (color >> mode_info.RedFieldPosition) & mask;
}

uint32_t extract_green(uint32_t color) {
    uint32_t mask = (1 << mode_info.GreenMaskSize) - 1;
    return (color >> mode_info.GreenFieldPosition) & mask;
}

uint32_t extract_blue(uint32_t color) {
    uint32_t mask = (1 << mode_info.BlueMaskSize) - 1;
    return (color >> mode_info.BlueFieldPosition) & mask;
}
