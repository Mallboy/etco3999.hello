#include "neslib.h"
#include "vrambuf.h"
#include "bcd.h"

// link the pattern table into CHR ROM
//#link "chr_generic.s"

// main function, run after console reset
void main(void) {

  unsigned char x = 0;
  unsigned char y = 0;
  // set palette colors
  //pal_col(0,0x04);	// set screen to dark blue
  //pal_col(1,0x1c);	// fuchsia
  //pal_col(2,0x33);	// grey
  //pal_col(3,0x3c);	// white

  //pal_col(17, 0x2a);
  //pal_col(14, 0x03);
  //pal_col(15, 0x12);
  
  const char PALETTE[32] =
  {
    0x04, // screen color
    0x1c, 0x33, 0x3c, 0x0, // background palette 0
    0x1c, 0x20, 0x2c, 0x0, // background palette 1
    0x00, 0x1a, 0x20, 0x0, // background palette 2
    0x00, 0x1a, 0x20, 0x0, // background palette 3
    0x23, 0x31, 0x41, 0x0, // sprite palette 0
    0x00, 0x37, 0x25, 0x0, // sprite palette 1
    0x36, 0x21, 0x19, 0x0, // sprite palette 2
    0x1d, 0x37, 0x2b, // sprite palette 3
  };
  
  pal_all(PALETTE);
  
  // write text to name table
  vram_adr(NTADR_A(2,2));		// set address
  vram_write("HELL\x19, W\x19RLD!\x11", 14);	// write bytes to video RAM

  // enable PPU rendering (turn on screen)
  ppu_on_all();
  

  // infinite loop
  while (1)
  {
    char cur_oam = 0;
    x += 2;
    
    cur_oam = oam_spr(x, 30, 0x19, 0x0, cur_oam);
    ppu_wait_frame();
  }
}
