#include "gba.h"
#include "sprite.h"

/**
 * @brief スプライトの初期化
 * 全てのスプライどを画面外に移動させるだけでsの他のフラグは初期化させない
 * @return ** void
 */
void SpriteInit() {
  for (u32 i = 0; i < 128; i++) {
    SpriteSetCoordinate(i, SCREEN_WIDTH, SCREEN_HEIGHT);
  }
}

/**
 * @brief スプライトの座標を設定する
 * @param num オブジェクト番号
 * @param x X軸の絶対座標
 * @param y Y軸の絶対座標
 */
void SpriteSetCoordinate(u32 num, u32 x, u32 y) {
  SP(num)->X = x;
  SP(num)->Y = y;
}

/**
 * @brief スプライトの形状を設定
 *
 * @param num オブジェクト番号
 * @param shape 設定する形状
 */
void SpriteSetShape(u32 num, enum SPRITE_SHAPES shape) {
  SP(num)->Shape = shape;
}

/**
 * @brief スプライトのサイズを設定
 *
 * @param num オブジェクト番号
 * @param size 設定するサイズ
 */
void SpriteSetSize(u32 num, enum SPRITE_SIZE size) {
  SP(num)->Size = size;
}

/**
 * TODO ここなんでこう書いてこうなるのか そもそも SPRITE_SIZECODEがどんな値なのか
 * @brief スプライトのサイズと形状を設定
 *
 * @param num オブジェクト番号
 * @param form 設定するサイズと形状
 *  Sprite_8x8
 *  Sprite_16_8 など
 */
void SpriteSetForm(u32 num, enum SPRITE_SIZECODE form) {
  SpriteSetShape(num, form>>2);
  SpriteSetShape(num, form & 0x03);
}

/**
 * @brief スプライトにキャラクタをセットする
 *
 * @param num スプライト番号
 * @param character キャラクタ番号
 */
void SpriteSetCharacter(u32 num, u32 character) {
  SP(num)->Character = character;
}

/**
 * @brief スプライトのカラーパレットを設定
 *
 * @param num オブジェクト番号
 * @param palette 設定するパレット番号
 * paletteが負の時、256色/1パレットモードを設定する
 */
void SpriteSetPalette(u32 num, s32 palette) {
  if (palette < 0) {
    SP(num)->ColorMode = Color_256x1;
  } else {
    SP(num)->ColorMode = Color_16x16;
    SP(num)->Palette = palette;
  }
}

/**
 * TODO 番号が0なのは大丈夫なのか
 * @brief オブジェクトをセットアップ
 *
 * @param character 設定するキャラクタ番号
 * @param palette 設定するパレット番号
 * @param form 設定するサイズと形状
 * @param x 設定するX座標
 * @param y 設定するY座標
 * @return u32 セットアップしたオブジェクトの番号
 */
u32 SpriteSetUp(u32 character, s32 palette, enum SPRITE_SIZECODE form, u32 x, u32 y) {
  static u32 numOfSprites = 0;
  SpriteSetCharacter(numOfSprites, character);
  SpriteSetPalette(numOfSprites, palette);
  SpriteSetForm(numOfSprites, form);
  SpriteSetCoordinate(numOfSprites, x, y);
  return numOfSprites++;
}

/**
 * @brief スプライトを移動させる
 *
 * @param num オブジェクトの番号
 * @param dx  X方向の移動量
 * @param dy Y方向の移動量
 */
void SpriteMove(u32 num, s32 dx, s32 dy) {
  SP(num)->X += dx;
  SP(num)->Y += dy;
}

/**
 * @brief スプライトを水平方向に反転
 *
 * @param num オブジェクト番号
 */
void SpriteHFlip(u32 num) {
  SP(num)->HFlip ^= 1;
}

/**
 * @brief スプライトを水平方向に反転
 *
 * @param num オブジェクト番号
 */
void SpriteVFlip(u32 num) {
  SP(num)->VFlip ^= 1;
}

/**
 * ビット演算の意味がわからない
 * @brief スプライトの幅を取得
 *
 * @param num
 * @return u32
 */
u32 SpriteGetWidth(u32 num) {
  enum SPRITE_SIZECODE form = SP(num)->Shape << 2 | SP(num)->Size;

  switch (form) {
    case Sprite_8x8:
    case Sprite_8x16:
    case Sprite_8x32:
      return 8;

    case Sprite_16x8:
    case Sprite_16x16:
    case Sprite_16x32:
      return 16;

    case Sprite_32x8:
    case Sprite_32x16:
    case Sprite_32x32:
    case Sprite_32x64:
      return 32;

    case Sprite_64x32:
    case Sprite_64x64:
      return 64;
  }

  return 0;
}

/**
 * @brief スプライトの高さを取得
 *
 * @param num スプライトの番号
 * @return u32
 */
u32 SpriteGetHeight(u32 num) {
  enum SPRITE_SIZECODE form = SP(num)->Shape << 2 | SP(num)->Size;

  switch (form) {
    case Sprite_8x8:
    case Sprite_16x8:
    case Sprite_32x8:
      return 8;

    case Sprite_8x16:
    case Sprite_16x16:
    case Sprite_32x16:
      return 16;

    case Sprite_8x32:
    case Sprite_16x32:
    case Sprite_32x32:
    case Sprite_64x32:
      return 32;

    case Sprite_32x64:
    case Sprite_64x64:
      return 64;
  }

  return 0;
}

/**
 * @brief X軸方向に画面に写っているのか
 *
 * @param num オブジェクトの番号
 * @return u32 画面に写っているか
 */
u32 SpriteIsVisibleX(u32 num) {
  return SP(num)->X < SCREEN_WIDTH - SpriteGetWidth(num);
}

/**
 * @brief Y軸方向に画面に写っているか
 *
 * @param num オブジェクト番号
 * @return u32
 */
u32 SpriteIsVisibleY(u32 num) {
  return SP(num)->Y < SCREEN_HEIGHT - SpriteGetHeight(num);
}

/**
 * @brief 画面に写っているか
 *
 * @param num オブジェクト番号
 * @return u32
 */
u32 SpriteIsVisible(u32 num) {
  return SpriteIsVisibleX(num) && SpriteIsVisibleY(num);
}
