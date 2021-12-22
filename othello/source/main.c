/* main.c */

#include "gba.h"
#include "sprite.h"
#include "spr.h"

void WaitForVsync()
{
  while (REG_VCOUNT >= 160)
    ;
  while (REG_VCOUNT < 160)
    ;
}

void Init()
{
  SetMode(MODE_0 | OBJ_ENABLE | OBJ_1D_MAP);

  u16 *oam = OBJ_BASE_ADR; /* キャラクタデータ */
  u16 *pal = OBJ_COLORS;   /* パレットデータ */

  /* キャラクタデータの読み込み */
  for (u32 i = 0; i < sprTilesLen / 2; i++)
  {
    oam[i] = sprTiles[i];
  }

  /* パレットデータの読み込み */
  for (u32 i = 0; i < 16; i++)
  {
    pal[i] = sprPal[i];
  }

  /* 全てのスプライトを画面外に移動 */
  SpriteInit();
}

void PrintNum(u32 n)
{
#define digit 3 /* 表示する桁数 */
  static u32 spr[digit];

  /* 初めてPrintNum()が呼ばれたときの処理 */
  static u32 isInitialized = false;
  if (!isInitialized)
  {
    isInitialized = true;
    u32 x = digit * 8;
    /* スプライトの設定 */
    for (u32 i = 0; i < digit; i++)
    {
      x -= 8;
      spr[i] = SpriteSetUp(0, 0, Sprite_8x8, x, 0);
    }
  }

  /* スプライトに表示する桁を割り当てる */
  for (u32 i = 0; i < digit; i++)
  {
    SpriteSetCharacter(spr[i], n % 10);
    n /= 10;
  }
#undef digit
}

u32 setUpEmptyTile(u32 x, u32 y) {
  return SpriteSetUp(0, 0, Sprite_16x16, x, y);
}

void changeWhiteTile(u32 num){
  SpriteSetCharacter(num, 4);
}

void changeBlackTile(u32 num)
{
  SpriteSetCharacter(num, 8);
}

u32 genObjectNum(u32 x, u32 y) {
  return y * 8 + x;
}

typedef struct
{
  u32 tileNums[8][8];
  s32 stateData[8][8];
} othelloData;

othelloData InitOthelloData() {
  othelloData data;

  for (u32 j = 0; j < 8; j++)
  {
    for (u32 i = 0; i < 8; i++)
    {
      data.tileNums[j][i] = setUpEmptyTile(16 + 16 * i, 16 + 16 * j);
      data.stateData[j][i] = 0;
    }
  }
  data.stateData[3][3] = -1;
  data.stateData[4][4] = -1;
  data.stateData[3][4] = 1;
  data.stateData[4][3] = 1;
  return data;
}

void RewriteMap(othelloData* data) {
  for (u32 j = 0; j < 8; j++)
  {
    for (u32 i = 0; i < 8; i++)
    {
      if (data->stateData[j][i] == 1) {
        changeWhiteTile(data->tileNums[j][i]);
      }
      else if (data->stateData[j][i] == -1) {
        changeBlackTile(data->tileNums[j][i]);
      }
    }
  }
}

// TODO オセロの盤面に関するものは、別のファイル作るべきかも

/**
 * @brief 左方向にひっくり返すことができるかを判定
 *
 * @param data
 * @param x
 * @param y
 * @param color
 * @return u32
 */
u32 CheckLeft(othelloData *data, u32 x, u32 y, u32 color)
{
  return 1;
}

/**
 * @brief 石を置いて盤面を変更する
 *
 * @param data
 * @param x
 * @param y
 * @param color
 */
void PutStone(othelloData *data, u32 x, u32 y, u32 color)
{
}

int main(void)
{
  Init();
  othelloData data;
  data = InitOthelloData();

  while (1)
  {
    WaitForVsync();
    RewriteMap(&data);
  }
}
