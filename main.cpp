#include <windows.h>
#include "DxLib.h"
#include<time.h>
#include<stdlib.h>
#include<stdio.h>
#pragma warning(disable:4996)
const char* map[] = {
    "#####/"
    "#   ####/"
    "#   $@.#/"
    "########/",

    "########/"
    "#  @  .#/"
    "#  $   #/"
    "#### ###/"
    "#    $ #/"
    "#  .   #/"
    "########/",
};
enum STATUS
{
    壁,ボックス,プレイヤー,ゴール,行切り替え,空白
};
enum MENU
{
    ステージセレクト, ゲーム中
};
MENU menu;

const int Stagenum = 2;//ステージ数
const int Stage1xsize = 9;//ステージ1横の大きさ
const int Stage1ysize = 4;//ステージ１縦の大きさ
const int Stage2xsize=9;
const int Stage2ysize=7;
const int mapChipSize =64;//マップチップ一個の大きさ
const int map1XNum = 9;    //マップの横方向のチップ数
const int map1YNum = 4;    //マップの縦方向チップ数
const int mapImgXNum = 5;  //マップチップの画像の横方向チップ数
const int mapImgYNum = 1;  //マップチップの画像の縦方向チップ数
const int maxgoalnum=2;
//
// ステージ関連
int nowstage = 0;
//プレイヤー関連
    float px, py;//プレイヤーの座標
    float vx, vy;//プレイヤーの速度
    float animTimer;//アニメーションタイマー
    int playerImg;

    int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
    {   //.....................//
        //基本設定
        //.....................//

        //ウィンドウモード
        ChangeWindowMode(TRUE);
        //ライブラリの初期化
        DxLib_Init();
        //ウィンドウの命名変更
        SetMainWindowText("倉庫番");
        //背景
        SetDrawScreen(DX_SCREEN_BACK);
        //ウィンドウサイズとカラーモード
        SetGraphMode(640, 320, 32);
        //---------------------------//
        //ゲームの初期設定//
        //---------------------------//
        bool Updateflg[4] = { 0 };//押しっぱなしを制御する関数
        int stageselect = 0;//今のステージ（何ステージ目かの指標）
        //画像読み込み
        int pics[4] = { 0 };
        pics[0] = LoadGraph("素材/renga.png");
        pics[1] = LoadGraph("素材/box.png");
        pics[2] = LoadGraph("素材/player.png");
        pics[3] = LoadGraph("素材/stair.png");
        SetFontSize(30);
        DrawString(50, 50, "ステージセレクト", GetColor(255, 255, 255), TRUE);
        stageselect = KeyInputNumber(50, 50, Stagenum,1 , FALSE);
        nowstage = stageselect-1;
        while (nowstage < Stagenum && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
        {

            int Stagexsize=0, Stageysize=0;
            switch (nowstage)
            {
            case 0:
                Stagexsize = Stage1xsize;
                Stageysize = Stage1ysize;
                break;
            case 1:
                Stagexsize = Stage2xsize;
                Stageysize = Stage2ysize;
                break;
            }
            //マップメモリーの変数宣言
            int mapmemory[50][50];
            //マップメモリーの初期化
            for (int i = 0; i < Stageysize; i++)
            {
                for (int j = 0; j < Stagexsize; j++)
                {
                    mapmemory[i][j] = { 0 };
                }
            }
            //ゴール
            int goalx[maxgoalnum] = {}, goaly[maxgoalnum] = {};
            int goalnum = 0;
            //ボックスの位置
            int boxx[maxgoalnum] = {}, boxy[maxgoalnum] = {};//ゴールの数とボックスの数は同じ
            int boxnum = 0;
            //乱数
            srand((unsigned int)time(NULL));
            //プレイヤーの位置
            int playerx = 5, playery = 2;
            //判定
            //---------------------------//
            //ゲームのスタート//
            //---------------------------//

            //マップの読み込み
            int posx = 0, posy = 0;
            const char* p = map[nowstage];
            while (*p)
            {
                switch (*p++)
                {
                case '#'://壁
                    mapmemory[posy][posx] = 壁;
                    DrawGraph(posx++ * 16, posy * 16, pics[0], 0);
                    break;
                case '$'://ボックス
                    mapmemory[posy][posx] = ボックス;
                    boxx[boxnum] = posx, boxy[boxnum] = posy;//ボックスの位置を更新
                    DrawGraph(posx++ * 16, posy * 16, pics[1], 0);
                    boxnum++;
                    break;
                case '/'://行切り替え
                    mapmemory[posy][posx] = 行切り替え;
                    posx = 0;
                    posy++;
                    break;
                case'@'://プレイヤー
                    mapmemory[posy][posx] = プレイヤー;
                    playerx = posx;
                    playery = posy;
                    DrawGraph(posx++ * 16, posy * 16, pics[2], 0);
                    break;
                case'.'://ゴール（ボックスを置くべきとこ）
                    goalx[goalnum] = posx, goaly[goalnum] = posy;//ゴール位置を記憶
                    mapmemory[posy][posx] = ゴール;
                    DrawGraph(posx++ * 16, posy * 16, pics[3], 0);
                    goalnum++;
                    break;
                default:
                    mapmemory[posy][posx] = 空白;//何もないところ
                    posx++;
                    break;
                }
            }
            bool clearflg = 0;//クリアかどうか管理するフラグ
            //ゲームループスタート
            while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
            {
                //画像更新処理
                ClearDrawScreen();
                posx = 0, posy = 0;
                int n = 0;
                //フラグ管理
                char onstateflgcount = 0;
                bool onstateflg[maxgoalnum] = {};//ゴール上に何か乗っているか
                bool onflg = 0;//ゴール上に乗るとき
                boxnum = 0;
                //マップ描画------------------------------------------------------------
                while (n < Stagexsize*Stageysize)
                {
                    switch (mapmemory[posy][posx])
                    {
                    case 0://壁
                        //mapmemory[posy][posx] = 0;
                        DrawGraph(posx++ * 16, posy * 16, pics[0], 0);
                        break;
                    case 1://ボックス
                        //mapmemory[posy][posx] = 1;
                        boxx[boxnum] = posx, boxy[boxnum] = posy;//ボックスの位置を更新
                        boxnum++;
                        if (boxnum > goalnum)
                        {
                            boxnum = goalnum;
                        }
                        DrawGraph(posx++ * 16, posy * 16, pics[1], 0);
                        break;
                    case 4://行切り替え
                        //mapmemory[posy][posx] = 4;
                        posx = 0;
                        posy++;
                        break;
                    case 2://プレイヤー
                        //mapmemory[posy][posx] = 2;
                        playerx = posx;
                        playery = posy;
                        DrawGraph(posx++ * 16, posy * 16, pics[2], 0);
                        break;
                    case 3://ゴール（ボックスを置くべきとこ）
                        //mapmemory[posy][posx] = 3;
                        DrawGraph(posx++ * 16, posy * 16, pics[3], 0);
                        break;
                    default://何もないところ
                        //mapmemory[posy][posx] = 5;
                        posx++;
                        break;
                    }
                    n++;
                }
                //------------------------------------------------------------------------------------------------------------
                for (char i = 0; i < goalnum; i++)
                {
                    if (goalx[i] == boxx[i] && goaly[i] == boxy[i])
                    {
                        onstateflgcount++;
                        onstateflg[i] = 1;
                    }
                    else
                    {
                        onstateflg[i] = 0;
                    }
                    if (onstateflgcount == goalnum)
                    {
                        clearflg = 1;
                    }
                }
                //もしゴールの上に箱をすべて乗せたなら
                if (clearflg)
                {
                    
                    DrawString(320, 120, "STAGE CLEAR", GetColor(255, 255, 255), TRUE);
                    WaitKey();
                    nowstage++;
                    break;
                }
                if (CheckHitKey(KEY_INPUT_R))//Rを押したらリセット
                {
                    break;
                }
                //------------------///左を押したとき-------------------------------------------------------------------------------------
                if (CheckHitKey(KEY_INPUT_LEFT))
                {
                    bool canflg = 1;//移動できるかどうか調べるフラグ
                    bool boxflg = 0;//ボックスを動かすかどうかのフラグ
                    if (!Updateflg[0])
                    {
                        if (mapmemory[playery][playerx - 1] == 壁)//隣が壁
                        {
                            canflg = 0;
                        }
                        if (mapmemory[playery][playerx - 1] == ボックス)//隣がボックスでその隣が壁もしくはボックス
                        {
                            if (mapmemory[playery][playerx - 2] == 壁 || mapmemory[playery][playerx - 2] == ボックス)
                            {
                                canflg = 0;
                            }
                            boxflg = 1;
                        }
                        if (canflg)//動くことができる場合
                        {
                            if (mapmemory[playery][playerx - 1] == ゴール || mapmemory[playery][playerx - 2] == ゴール)//もし移動先がゴールなら
                            {
                                onflg = 1;
                            }
                            if (onflg)//移動先がゴールなら
                            {
                                //箱を動かす場合先に箱の処理をする。箱はゴールの上に乗る
                                if (boxflg)
                                {
                                    mapmemory[playery][playerx] = 空白;//移動したところは空白になる。
                                    mapmemory[playery][playerx - 1] = プレイヤー;//箱のあったところにはプレイヤーが移動。
                                    mapmemory[playery][playerx - 2] = ボックス;//箱を押した先はゴール。
                                }
                                mapmemory[playery][playerx] = 空白;//移動したところは空白になる。
                                mapmemory[playery][playerx - 1] = プレイヤー;//箱のあったところにはプレイヤーが移動。
                                canflg = 0;
                            }
                            else
                            {
                                onflg = 0;

                                //箱を動かす場合先に箱を動かす処理をしてからプレイヤーが動く
                                if (boxflg)
                                {
                                    int memory = mapmemory[playery][playerx - 1];
                                    mapmemory[playery][playerx - 1] = mapmemory[playery][playerx - 2];
                                    mapmemory[playery][playerx - 2] = memory;
                                }
                                int memory = mapmemory[playery][playerx];
                                mapmemory[playery][playerx] = mapmemory[playery][playerx - 1];
                                mapmemory[playery][playerx - 1] = memory;
                                for (char i = 0; i < goalnum; i++)
                                {
                                    if (!onstateflg[i])
                                    {
                                        mapmemory[goaly[i]][goalx[i]] = ゴール;
                                    }
                                }
                                canflg = 0;
                            }
                        }
                        Updateflg[0] = 1;
                    }
                }
                else
                {
                    Updateflg[0] = 0;
                }
                //-----------///右を押したとき-------------------------------------------------------------------------------------
                if (CheckHitKey(KEY_INPUT_RIGHT))
                {
                    bool canflg = 1;//移動できるかどうか調べるフラグ
                    bool boxflg = 0;//ボックスを動かすかどうかのフラグ
                    if (!Updateflg[1])
                    {
                        if (mapmemory[playery][playerx + 1] == 0)//隣が壁
                        {
                            canflg = 0;
                        }
                        if (mapmemory[playery][playerx + 1] == 1)//隣がボックスでその隣が壁もしくはボックス
                        {
                            if (mapmemory[playery][playerx + 2] == ボックス || mapmemory[playery][playerx + 2] == 壁)
                            {
                                canflg = 0;
                            }
                            boxflg = 1;
                        }
                        if (canflg)
                        {
                            if (mapmemory[playery][playerx + 1] == ゴール || mapmemory[playery][playerx + 2] == ゴール)//もし移動先がゴールなら
                            {
                                onflg = 1;
                            }
                            if (onflg)//移動先がゴールなら
                            {
                                //箱を動かす場合先に箱の処理をする。箱はゴールの上に乗る
                                if (boxflg)
                                {
                                    mapmemory[playery][playerx] = 空白;//移動したところは空白になる。
                                    mapmemory[playery][playerx + 1] = プレイヤー;//箱のあったところにはプレイヤーが移動。
                                    mapmemory[playery][playerx + 2] = ボックス;//箱を押した先はゴール。つまりクリア
                                }
                                else
                                {
                                    mapmemory[playery][playerx] = 空白;//移動したところは空白になる。
                                    mapmemory[playery][playerx + 1] = プレイヤー;//箱のあったところにはプレイヤーが移動。
                                }

                                canflg = 0;
                            }
                            else
                            {
                                onflg = 0;
                                //箱を動かす場合先に箱の処理をする
                                if (boxflg)
                                {
                                    int memory = mapmemory[playery][playerx + 1];
                                    mapmemory[playery][playerx + 1] = mapmemory[playery][playerx + 2];
                                    mapmemory[playery][playerx + 2] = memory;
                                }
                                int memory = mapmemory[playery][playerx];
                                mapmemory[playery][playerx] = mapmemory[playery][playerx + 1];
                                mapmemory[playery][playerx + 1] = memory;
                                for (char i = 0; i < goalnum; i++)
                                {
                                    if (!onstateflg[i])
                                    {
                                        mapmemory[goaly[i]][goalx[i]] = 3;
                                    }
                                }
                                canflg = 0;
                            }
                        }
                        Updateflg[1] = 1;
                    }
                }
                else
                {
                    Updateflg[1] = 0;
                }
                //-----------///上を押したとき-------------------------------------------------------------------------------------
                if (CheckHitKey(KEY_INPUT_UP))
                {
                    bool canflg = 1;//移動できるかどうか調べるフラグ
                    bool boxflg = 0;//ボックスを動かすかどうかのフラグ
                    if (!Updateflg[2])
                    {
                        if (mapmemory[playery - 1][playerx] == 0)//隣が壁
                        {
                            canflg = 0;
                        }
                        if (mapmemory[playery - 1][playerx] == 1)//隣がボックスでその隣が壁もしくはボックス
                        {
                            if (mapmemory[playery - 2][playerx] == 0 || mapmemory[playery - 2][playerx] == 2)
                            {
                                canflg = 0;
                            }
                            boxflg = 1;
                        }
                        if (canflg)
                        {
                            if (mapmemory[playery - 1][playerx] == 3 || mapmemory[playery - 2][playerx] == 3)//もし移動先がゴールなら
                            {
                                onflg = 1;
                            }
                            if (onflg)//移動先がゴールなら
                            {
                                //箱を動かす場合先に箱の処理をする。箱はゴールの上に乗る
                                if (boxflg)
                                {
                                    mapmemory[playery][playerx] = 空白;//移動したところは空白になる。
                                    mapmemory[playery - 1][playerx] = プレイヤー;//箱のあったところにはプレイヤーが移動。
                                    mapmemory[playery - 2][playerx] = ボックス;//箱を押した先はゴール。つまりクリア
                                }
                                else
                                {
                                    mapmemory[playery][playerx] = 空白;//移動したところは空白になる。
                                    mapmemory[playery - 1][playerx] = プレイヤー;//箱のあったところにはプレイヤーが移動。
                                    canflg = 0;
                                }
                            }
                            else
                            {
                                onflg = 0;
                                //箱を動かす場合先に箱の処理をする
                                if (boxflg)
                                {
                                    int memory = mapmemory[playery - 1][playerx];
                                    mapmemory[playery - 1][playerx] = mapmemory[playery - 2][playerx];
                                    mapmemory[playery - 2][playerx] = memory;
                                }
                                int memory = mapmemory[playery][playerx];
                                mapmemory[playery][playerx] = mapmemory[playery - 1][playerx];
                                mapmemory[playery - 1][playerx] = memory;
                                for (char i = 0; i < goalnum; i++)
                                {
                                    if (!onstateflg[i])
                                    {
                                        mapmemory[goaly[i]][goalx[i]] = 3;
                                    }
                                }
                                canflg = 0;
                            }
                        }
                        Updateflg[2] = 1;
                    }
                }
                else
                {
                    Updateflg[2] = 0;
                }
                //----------------///下を押したとき-------------------------------------------------------------------------------------
                if (CheckHitKey(KEY_INPUT_DOWN))
                {
                    bool canflg = 1;//移動できるかどうか調べるフラグ
                    bool boxflg = 0;//ボックスを動かすかどうかのフラグ
                    if (!Updateflg[3])
                    {
                        if (mapmemory[playery + 1][playerx] == 壁)//隣が壁
                        {
                            canflg = 0;
                        }
                        if (mapmemory[playery + 1][playerx] == ボックス)//隣がボックスでその隣が壁もしくはボックス
                        {
                            if (mapmemory[playery + 2][playerx] == 壁 || mapmemory[playery + 2][playerx] == ボックス)
                            {
                                canflg = 0;
                            }
                            boxflg = 1;
                        }
                        if (canflg)
                        {
                            if (mapmemory[playery + 1][playerx] == ゴール || mapmemory[playery + 2][playerx] == ボックス)//もし移動先がゴールなら
                            {
                                onflg = 1;
                            }
                            if (onflg)//移動先がゴールなら
                            {
                                //箱を動かす場合先に箱の処理をする。箱はゴールの上に乗る
                                if (boxflg)
                                {
                                    mapmemory[playery][playerx] = 空白;//移動したところは空白になる。
                                    mapmemory[playery + 1][playerx] = プレイヤー;//箱のあったところにはプレイヤーが移動。
                                    mapmemory[playery + 2][playerx] = ボックス;//箱を押した先はゴール。つまりクリア
                                }
                                else
                                {
                                    mapmemory[playery][playerx] = 5;//移動したところは空白になる。
                                    mapmemory[playery + 1][playerx] = 2;//箱のあったところにはプレイヤーが移動。
                                }
                                canflg = 0;
                            }
                            else
                            {
                                onflg = 0;
                                //箱を動かす場合先に箱の処理をする
                                if (boxflg)
                                {
                                    int memory = mapmemory[playery + 1][playerx];
                                    mapmemory[playery + 1][playerx] = mapmemory[playery + 2][playerx];
                                    mapmemory[playery + 2][playerx] = memory;
                                }
                                int memory = mapmemory[playery][playerx];
                                mapmemory[playery][playerx] = mapmemory[playery + 1][playerx];
                                mapmemory[playery + 1][playerx] = memory;
                                for (char i = 0; i < goalnum; i++)
                                {
                                    if (!onstateflg[i])
                                    {
                                        mapmemory[goaly[i]][goalx[i]] = ゴール;
                                    }
                                }
                                canflg = 0;
                            }
                        }
                        Updateflg[3] = 1;
                    }
                }
                else
                {
                    Updateflg[3] = 0;
                }
                ScreenFlip();
                //-------------------------------//
                //エラーの場合はループを抜ける
                //-------------------------------//
                if (ProcessMessage() != 0)
                {
                    break;
                }
            }
        }

        //ゲームの終了
        return 0;
    }
     
    