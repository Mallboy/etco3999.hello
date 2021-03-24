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

struct Bomb
{
  unsigned char y;
  unsigned char yoff;
  unsigned char yDir;
};

struct Player
{
  unsigned char x;
  unsigned char y;
  //unsigned char dir; // Lower 4 x, Higher 4 y
  unsigned char xDir;
  unsigned char yDir;
  unsigned char att;
  int face;
};

struct Controller
{
  unsigned char a;
  unsigned char b;
  unsigned char select;
  unsigned char start;
  unsigned char up;
  unsigned char down;
  unsigned char left;
  unsigned char right;
};

struct Controller controller = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

// main function, run after console reset
void main(void) {
  
  struct Player player = {50, 143, 0, 0, 0x40, 1};
  
  struct Bomb bomb = {143, 5, 1};
  
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
    char pad_result = pad_poll(0);
    
    char cur_oam = 0;
    
    player.x += player.xDir;
    bomb.yoff += bomb.yDir;
    
    if(pad_result & controller.left)
    {
      player.xDir = -2;
      player.att = 0x0;
      player.face = 0;
    }
    else if(pad_result & controller.right)
    {
      player.xDir = 2;
      player.att = 0x40;
      player.face = 1;
    }
    else
    {
      player.xDir = 0; 
    }
    
    if(bomb.yoff > 10)
    {
      bomb.yDir -= 2;
    }
    if(bomb.yoff < 5)
    {
      bomb.yDir += 2;
    }
    
    cur_oam = oam_meta_spr(player.x, player.y, cur_oam, playerStand[player.face]);
    cur_oam = oam_meta_spr(232, 143, cur_oam, doorSprite);
    cur_oam = oam_spr(player.x+(8*player.face), (bomb.y+bomb.yoff)-16, 0x19, player.att, cur_oam);
    cur_oam = oam_spr(player.x+(12-(8*player.face)), (bomb.y-bomb.yoff), 0x19, player.att, cur_oam);
    cur_oam = oam_spr(player.x+(4-(8*player.face)), (bomb.y-bomb.yoff), 0x19, player.att, cur_oam);
    cur_oam = oam_spr(player.x+(8 - (8*player.face)), (bomb.y+bomb.yoff)-16, 0x19, player.att, cur_oam);
    
    if(player.x>232-16)
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
