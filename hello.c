#include "neslib.h"
#include "vrambuf.h"
#include "bcd.h"

// link the pattern table into CHR ROM
//#link "chr_generic.s"
//#link "vrambuf.c"

/*{pal:"nes",layout:"nes"}*/
const char PALETTE[32] =
  {
    0x04, // screen color
    0x03, 0x31, 0x32, 0x00, // background palette 0
    0x12, 0x12, 0x07, 0x00, // background palette 1
    0x1A, 0x1A, 0x05, 0x00, // background palette 2
    0x00, 0x1A, 0x20, 0x00, // background palette 3
    0x00, 0x26, 0x0E, 0x00, // sprite palette 0
    0x17, 0x15, 0x06, 0x00, // sprite palette 1
    0x22, 0x31, 0x0C, 0x00, // sprite palette 2
    0x1D, 0x37, 0x2B, // sprite palette 3
  };

#define DEF_METASPRITE_2x2(name,code,pal)\
const unsigned char name[]={\
        0,      0,      (code)+0,   pal, \
        0,      8,      (code)+1,   pal, \
        8,      0,      (code)+2,   pal, \
        8,      8,      (code)+3,   pal, \
        128};

// define a 2x2 metasprite, flipped horizontally
#define DEF_METASPRITE_2x2_FLIP(name,code,pal)\
const unsigned char name[]={\
        8,      0,      (code)+0,   (pal)|OAM_FLIP_H, \
        8,      8,      (code)+1,   (pal)|OAM_FLIP_H, \
        0,      0,      (code)+2,   (pal)|OAM_FLIP_H, \
        0,      8,      (code)+3,   (pal)|OAM_FLIP_H, \
        128};

const char GROUND[32] =
{
  0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f,
  0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f,
  0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f,
  0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f
};

const char GROUNDTOP[32] =
{
  0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a,
  0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a,
  0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a,
  0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a
};

const char ATTRIBUTE_TABLE[0x40] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // rows 0-3
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // rows 4-7
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, // rows 8-11
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // rows 12-15
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, // rows 16-19
  0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, // rows 20-23
  0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, // rows 24-27
  0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA  // rows 28-29
};

DEF_METASPRITE_2x2(playerRStand, 0xd8, 1);
DEF_METASPRITE_2x2_FLIP(playerLStand, 0xd8, 1);
DEF_METASPRITE_2x2(doorSprite, 0xc4, 2);

const unsigned char* const playerStand[2] = {
  playerLStand, playerRStand
};

// main function, run after console reset
void main(void) {

  unsigned char x = 50;
  unsigned char att = 0x40;
  int face = 1;
  unsigned char xDir = 2;
  
  unsigned char y = 143;
  unsigned char yoff = 5;
  unsigned char yDir = 1;
  
  int bx = 0;
  int by = 20;
  
  
  // set palette colors
  //pal_col(0,0x04);	// set screen to dark blue
  //pal_col(1,0x1c);	// fuchsia
  //pal_col(2,0x33);	// grey
  //pal_col(3,0x3c);	// white

  //pal_col(17, 0x2a);
  //pal_col(14, 0x03);
  //pal_col(15, 0x12);
  

  
  pal_all(PALETTE);
  
  // write text to name table
  vram_adr(NTADR_A(1,1));		// set address
  vram_write("This is", 7);	// write bytes to video RAM
  
  vram_adr(NTADR_A(1,2));		// set address
  vram_write("Elijah Maloy's", 14);
  
  vram_adr(NTADR_A(1,3));		// set address
  vram_write("first NES 'Game'!", 18);
  
  /*
  while(by < 30)
  {
    bx = 0;
    while(bx < 1)
    {
      vram_adr(NTADR_A(bx,by));
      //vram_write("\x0f\x0A\x0A\x0A\x0A\x0A\x0A\x0A\x0A\x0A\x0a", 11);
      if(by == 20)
      {
        vram_write(GROUND, 32);
      }
      else
      {
        vram_write(GROUNDTOP, 32);
      }
      bx += 2;
    }
    by += 1;
  }
  */
  // fill nametable with diamonds
  vram_adr(NTADR_A(0,20));
  vram_fill(0x0b, 32*8);
  
  vram_adr(0x23c0);
  vram_write(ATTRIBUTE_TABLE, sizeof(ATTRIBUTE_TABLE));
  
  // copy attribute table from PRG ROM to VRAM
  
  
    // enable PPU rendering (turn on screen)
  ppu_on_all();
  

  vrambuf_clear();
  set_vram_update(updbuf);
  
  
  
  // infinite loop
  while (1)
  {
    char cur_oam = 0;
    x += xDir;
    yoff += yDir;
    
    if(x > 235)
    {
      xDir -= 2;
      att = 0x0;
      face = 0;
    }
    
    if(x < 8)
    {
      xDir += 2;
      att = 0x40;
      face = 1;
    }
    
    if(yoff > 10)
    {
      yDir -= 2;
    }
    if(yoff < 5)
    {
      yDir += 2;
    }
    
    cur_oam = oam_meta_spr(x, y, cur_oam, playerStand[face]);
    cur_oam = oam_meta_spr(232, y, cur_oam, doorSprite);
    cur_oam = oam_spr(x+(8*face), (y+yoff)-16, 0x19, att, cur_oam);
    
    if(x>232-16)
    {
      vrambuf_put(NTADR_A(1,5), "On the door!", 12);
    }
    else
    {
      vrambuf_put(NTADR_A(1,5), "           ", 12);
    }
    vrambuf_flush();
    //ppu_wait_frame();
  }
}
