#include "neslib.h"
#include "vrambuf.h"
#include "bcd.h"

// link the pattern table into CHR ROM
//#link "chr_generic.s"

// main function, run after console reset
void main(void) {

  // set palette colors
  pal_col(0,0x04);	// set screen to dark blue
  pal_col(1,0x1c);	// fuchsia
  pal_col(2,0x33);	// grey
  pal_col(3,0x3c);	// white

  // write text to name table
  vram_adr(NTADR_A(2,2));		// set address
  vram_write("HELL\x19, W\x19RLD!\x11", 14);	// write bytes to video RAM

  // enable PPU rendering (turn on screen)
  ppu_on_all();

  // infinite loop
  while (1) ;
}
