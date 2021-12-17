#include "gba.h"
#include "sprite.h"
#include "spr.h"

void WaitForVsync() {
  while(REG_VCOUNT >= 160);
  while(REG_VCOUNT < 160);
}

void Init() {
  SetMode(MODE_0 | OBJ_ENABLE | OBJ_1D_MAP);
  u16* oam = OBJ_BASE_ADR;
  u16 *pal = OBJ_COLORS;

  for (u32 i = 0; i < sprTilesLen / 2; i++) {
    oam[i] = sprTiles[i];
  }

  for (u32 i = 0; i < 16 / 2; i++) {
    pal[i] = sprPal[i];
  }

  SpriteInit();
}

void PrintNum(u32 n) {
  #define digit 3
  static u32 spr[digit];

  static u32 isInitialized = false;
  if (!isInitialized) {
    isInitialized = true;
    u32 x = digit * 8;

    for (u32 i = 0; i < digit; i++) {
      x -= 8;
      spr[i] = SpriteSetUp(0, 0, Sprite_8x8, x, 0);
    }
  }

  for (u32 i = 0; i < digit; i++) {
    SpriteSetCharacter(spr[i], n%10);
    n /= 10;
  }

  #undef digit
}

int main() {
  Init();

  u32 spr = SpriteSetUp(9, 0, Sprite_16x32, 0, 50);

  u32 dx = 1;
  u32 count = 0;

  while (1) {
    SpriteMove(spr, dx, 0);
    if (!SpriteIsVisibleX(spr)) {
      dx *= -1;
      SpriteHFlip(spr);
    }

    count++;
    PrintNum(count);
    WaitForVsync();
  }
}