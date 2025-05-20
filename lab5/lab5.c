// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

// Any header files included below this line should have been created by you

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/shared/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/shared/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(video_test_init)(uint16_t mode, uint8_t delay) {
  
  // switch the video adapter to the graphics mode specified
  // in the argument using VBE interface and back to the default
  // text mode

  // uses VBE INT 0x10 interface

  reg86_t r86;

  memset(&r86, 0, sizeof(r86));

  r86.intno = 0x10;
  r86.ah = 0x4F;
  r86.al = 0x02;
  r86.bx = mode | BIT(14);


  if (sys_int86(&r86) != 0){
    printf("Error on video_test_init on %s\n", __func__);
    return 1;
  }

  sleep(delay);

  vg_exit();
  return 0;
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  
  //1) map video memory to the process' address space
  
  /*endpoint_t who = 0; // value that identifies the process on whose address space
                  // the physical memory region should be mapped
  void *phaddr; //physical address of the first memory byte in that region
  size_t len = 0; //region's length
  vm_map_phys(who, &phaddr, len); // returns the virtual address on which the physical address range was mapped
  */
  //a process can use the returned virtual address to access the contents in the mapped physical address range

  // to grant a process the permission to map a given address range should use:
  /*endpoint_t proc_ep = 0; //process whose privileges will be affected
  int request = 0;//depend on the privileges to change
  void *p;//depend on the privileges to change
  if (sys_privctl(proc_ep, request, &p) != 0){
    printf("Error on sys_privctl call on %s\n", __func__);
    return 1;
  }
  */
  
  static void *video_mem;
  struct minix_mem_range mr;
  memset(&mr, 0, sizeof(mr));
  unsigned int vram_base;  // VRAM's physical addresss
  unsigned int vram_size;  // VRAM's size, but you can use the frame-buffer size, instead
  int r = 0;

  /* Use VBE function 0x01 to initialize vram_base and vram_size */
  vbe_mode_info_t vmi_p;
  memset(&vmi_p, 0, sizeof(vmi_p));

  // should use VBE function 0x01 - Return VBE Mode Information
  if (vbe_get_mode_info(mode, &vmi_p) != 0){
    printf("Error on vbe_get_mode_info call on %s\n", __func__);
    return 1;
  }

  vram_base = vmi_p.PhysBasePtr;
  vram_size = vmi_p.XResolution * vmi_p.YResolution * vmi_p.BytesPerScanLine / vmi_p.XResolution;
  
  // Allow memory mapping

  mr.mr_base = (phys_bytes) vram_base;	
  mr.mr_limit = mr.mr_base + vram_size;  

  if( (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)) != 0){
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);
  }
  // Map memory

  video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);

  if(video_mem == MAP_FAILED){
    panic("couldn't map video memory");
  }

  //2) change video mode to the one on the argument

  reg86_t r86;
  memset(&r86, 0, sizeof(r86));

  r86.intno = 0x10;
  r86.ah = 0x4F;
  r86.al = 0x02;
  r86.bx = mode | BIT(14);


  if (sys_int86(&r86) != 0){
    printf("Error on video_test_init on %s\n", __func__);
    return 1;
  }


  //3) draw rectangle

  //uint8_t *pixel = video_mem + y * vmi_p.BytesPerScanLine + x * (vmi_p.BytesPerScanLine / vmi_p.XResolution);
  //memcpy((char*)video_mem + vmi_p.BytesPerScanLine/vmi_p.XResolution * vmi_p.XResolution * y + vmi_p.BytesPerScanLine/vmi_p.XResolution * x, &color, vmi_p.BitsPerPixel/8);
  for (int j = y; j <= y+height; j++){
    // TODO: need to use this on video_gr.c file
    /*if (vg_draw_hline(x, y, width, color) != 0){
        printf("Error on vg_draw_hline call on %s\n", __func__);
        return 1;
      }*/
    for (int i = x; i <= x+width; i++){
      memcpy((char*)video_mem + vmi_p.BytesPerScanLine/vmi_p.XResolution * vmi_p.XResolution * j + vmi_p.BytesPerScanLine/vmi_p.XResolution * i, &color, vmi_p.BitsPerPixel/8);
    }
  } 
  
  //4) reset the video mode to minix default's text mode and return after reeiving break code of the ESC key (0x81)

  sleep(5);
  vg_exit();
  return 0;
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step){
  /* To be completed */
  printf("%s(0x%03x, %u, 0x%08x, %d): under construction\n", __func__,
         mode, no_rectangles, first, step);

  return 1;
}
  
int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y){
  /* To be completed */
  printf("%s(%8p, %u, %u): under construction\n", __func__, xpm, x, y);

  return 1;
}


int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf, int16_t speed, uint8_t fr_rate){
  /* To be completed */
  printf("%s(%8p, %u, %u, %u, %u, %d, %u): under construction\n",
         __func__, xpm, xi, yi, xf, yf, speed, fr_rate);

  return 1;
}

int(video_test_controller)() {
  /* This year you do not need to implement this */
  printf("%s(): under construction\n", __func__);

  return 1;
}
