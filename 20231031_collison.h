#pragma once
#include"DxLib.h"
//ブロック壁情報
#define BLOCK_LEFT  0x01        //0001
#define BLOCK_RIGHT 0x02        //0010
#define BLOCK_UP    0x04        //0100
#define BLOCK_DOWN  0x08        //1000

typedef unsigned char BlockInfo;

/// <summary>
///当たり判定矩形 
/// /// </summary>
struct sHitRect
{
	float w;      //当たり矩形幅ピクセル
	float h;      //当たり矩形高さピクセル
	float worldLX;//左上X座標
	float worldLY;//左上Y座標
	float worldRX;//右下X座標
	float worldRY;//右下Y座標
};
/// <summary>
/// 当たり判定初期化
/// </summary>
/// /// <param name="detRect">初期化したい当たり判定変数</param>
/// /// <param name="w">当たり判定の幅</param>
/// /// <param name="h">当たり判定の高さ</param>
void initRect(sHitRect& dstRect, float w, float h);


/// <summary>
/// 当たり判定の移動更新処理
///  </summary>
///  <param name="dstRect">更新したい当たり判定変数</param>
///  <param name="w">当たり判定の左上x座標</param>
///  <param name="h">当たり判定の左上y座標</param>
void updateWorldRect(sHitRect& dstRect, float x, float y);


/// <summary>
/// 当たり判定矩形の描画
/// </summary> 
/// <param name="rect">描画したい当たり判定矩形</param>
void drawRect(const sHitRect& rect);

/// <summary>
/// ２つの矩形が衝突したかを返す
/// </summary>
/// <param name="rect1">矩形１</param>
/// <param name="rect2">矩形２</param>
/// <returns>矩形の２つが衝突したかtrue/falseで返す</returns>
bool isHitRect(const sHitRect& rect1, const sHitRect& rect2);
/// <summary>
/// 2つの矩形の衝突めり込み量を計算し、mobaleRect位置を変更する
/// </summary>
/// <param name="movebleRect">移動可能矩形</param>
/// <param name="statucRect">静止矩形（動かないブロックなど）</param>
void calcFixHitPositon(sHitRect& movebleRect, const sHitRect& staticRect);
/// <summary>
///2つの矩形の衝突めり込み量を計算し、mobaleRect位置を変更する
/// </summary>
/// /// <param name="movebleRect">移動可能矩形</param>
///  <param name="staticRect">>静止矩形(動かないブロックなど)</param
///  <param name="blockinfo">ブロック情報</param>
void calcFixHitPositon(sHitRect& movebleRect, const sHitRect& staticRect,BlockInfo blockinfo);