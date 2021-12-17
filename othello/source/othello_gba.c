/* main.c */
#include "gba.h"

#define FRAME_0 (u8 *)0x06000000
#define FRAME_1 (u8 *)0x0600A000

u8 *screen;

void SwitchFrame() {
  REG_DISPCNT ^= BACKBUFFER;
  screen = (REG_DISPCNT & BACKBUFFER) ? FRAME_0 : FRAME_1;
}

void WaitForVsync() {
  while (REG_VCOUNT>=160);
  while (REG_VCOUNT < 160);

  SwitchFrame();
}

void PaletteInit() {
  // TODO かく
}

void Init() {
  SetMode(MODE_4 | BG2_ENABLE);
  screen = FRAME_0;
  PaletteInit();
}

void PutLine(u32 line, u8 index) {
  // TODO かく
  static u32 i = 0;
  for (u32 j = 0; j < 160; j++) {
    PutLine(j, (i + j) & 0xff);
  }
}

void Draw() {
  // TODO 書く
}

int main() {
  Init();
  while (1) {
    Draw();
    WaitForVsync();
  }
}
