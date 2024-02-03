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
    ��,�{�b�N�X,�v���C���[,�S�[��,�s�؂�ւ�,��
};
enum MENU
{
    �X�e�[�W�Z���N�g, �Q�[����
};
MENU menu;

const int Stagenum = 2;//�X�e�[�W��
const int Stage1xsize = 9;//�X�e�[�W1���̑傫��
const int Stage1ysize = 4;//�X�e�[�W�P�c�̑傫��
const int Stage2xsize=9;
const int Stage2ysize=7;
const int mapChipSize =64;//�}�b�v�`�b�v��̑傫��
const int map1XNum = 9;    //�}�b�v�̉������̃`�b�v��
const int map1YNum = 4;    //�}�b�v�̏c�����`�b�v��
const int mapImgXNum = 5;  //�}�b�v�`�b�v�̉摜�̉������`�b�v��
const int mapImgYNum = 1;  //�}�b�v�`�b�v�̉摜�̏c�����`�b�v��
const int maxgoalnum=2;
//
// �X�e�[�W�֘A
int nowstage = 0;
//�v���C���[�֘A
    float px, py;//�v���C���[�̍��W
    float vx, vy;//�v���C���[�̑��x
    float animTimer;//�A�j���[�V�����^�C�}�[
    int playerImg;

    int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
    {   //.....................//
        //��{�ݒ�
        //.....................//

        //�E�B���h�E���[�h
        ChangeWindowMode(TRUE);
        //���C�u�����̏�����
        DxLib_Init();
        //�E�B���h�E�̖����ύX
        SetMainWindowText("�q�ɔ�");
        //�w�i
        SetDrawScreen(DX_SCREEN_BACK);
        //�E�B���h�E�T�C�Y�ƃJ���[���[�h
        SetGraphMode(640, 320, 32);
        //---------------------------//
        //�Q�[���̏����ݒ�//
        //---------------------------//
        bool Updateflg[4] = { 0 };//�������ςȂ��𐧌䂷��֐�
        int stageselect = 0;//���̃X�e�[�W�i���X�e�[�W�ڂ��̎w�W�j
        //�摜�ǂݍ���
        int pics[4] = { 0 };
        pics[0] = LoadGraph("�f��/renga.png");
        pics[1] = LoadGraph("�f��/box.png");
        pics[2] = LoadGraph("�f��/player.png");
        pics[3] = LoadGraph("�f��/stair.png");
        SetFontSize(30);
        DrawString(50, 50, "�X�e�[�W�Z���N�g", GetColor(255, 255, 255), TRUE);
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
            //�}�b�v�������[�̕ϐ��錾
            int mapmemory[50][50];
            //�}�b�v�������[�̏�����
            for (int i = 0; i < Stageysize; i++)
            {
                for (int j = 0; j < Stagexsize; j++)
                {
                    mapmemory[i][j] = { 0 };
                }
            }
            //�S�[��
            int goalx[maxgoalnum] = {}, goaly[maxgoalnum] = {};
            int goalnum = 0;
            //�{�b�N�X�̈ʒu
            int boxx[maxgoalnum] = {}, boxy[maxgoalnum] = {};//�S�[���̐��ƃ{�b�N�X�̐��͓���
            int boxnum = 0;
            //����
            srand((unsigned int)time(NULL));
            //�v���C���[�̈ʒu
            int playerx = 5, playery = 2;
            //����
            //---------------------------//
            //�Q�[���̃X�^�[�g//
            //---------------------------//

            //�}�b�v�̓ǂݍ���
            int posx = 0, posy = 0;
            const char* p = map[nowstage];
            while (*p)
            {
                switch (*p++)
                {
                case '#'://��
                    mapmemory[posy][posx] = ��;
                    DrawGraph(posx++ * 16, posy * 16, pics[0], 0);
                    break;
                case '$'://�{�b�N�X
                    mapmemory[posy][posx] = �{�b�N�X;
                    boxx[boxnum] = posx, boxy[boxnum] = posy;//�{�b�N�X�̈ʒu���X�V
                    DrawGraph(posx++ * 16, posy * 16, pics[1], 0);
                    boxnum++;
                    break;
                case '/'://�s�؂�ւ�
                    mapmemory[posy][posx] = �s�؂�ւ�;
                    posx = 0;
                    posy++;
                    break;
                case'@'://�v���C���[
                    mapmemory[posy][posx] = �v���C���[;
                    playerx = posx;
                    playery = posy;
                    DrawGraph(posx++ * 16, posy * 16, pics[2], 0);
                    break;
                case'.'://�S�[���i�{�b�N�X��u���ׂ��Ƃ��j
                    goalx[goalnum] = posx, goaly[goalnum] = posy;//�S�[���ʒu���L��
                    mapmemory[posy][posx] = �S�[��;
                    DrawGraph(posx++ * 16, posy * 16, pics[3], 0);
                    goalnum++;
                    break;
                default:
                    mapmemory[posy][posx] = ��;//�����Ȃ��Ƃ���
                    posx++;
                    break;
                }
            }
            bool clearflg = 0;//�N���A���ǂ����Ǘ�����t���O
            //�Q�[�����[�v�X�^�[�g
            while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
            {
                //�摜�X�V����
                ClearDrawScreen();
                posx = 0, posy = 0;
                int n = 0;
                //�t���O�Ǘ�
                char onstateflgcount = 0;
                bool onstateflg[maxgoalnum] = {};//�S�[����ɉ�������Ă��邩
                bool onflg = 0;//�S�[����ɏ��Ƃ�
                boxnum = 0;
                //�}�b�v�`��------------------------------------------------------------
                while (n < Stagexsize*Stageysize)
                {
                    switch (mapmemory[posy][posx])
                    {
                    case 0://��
                        //mapmemory[posy][posx] = 0;
                        DrawGraph(posx++ * 16, posy * 16, pics[0], 0);
                        break;
                    case 1://�{�b�N�X
                        //mapmemory[posy][posx] = 1;
                        boxx[boxnum] = posx, boxy[boxnum] = posy;//�{�b�N�X�̈ʒu���X�V
                        boxnum++;
                        if (boxnum > goalnum)
                        {
                            boxnum = goalnum;
                        }
                        DrawGraph(posx++ * 16, posy * 16, pics[1], 0);
                        break;
                    case 4://�s�؂�ւ�
                        //mapmemory[posy][posx] = 4;
                        posx = 0;
                        posy++;
                        break;
                    case 2://�v���C���[
                        //mapmemory[posy][posx] = 2;
                        playerx = posx;
                        playery = posy;
                        DrawGraph(posx++ * 16, posy * 16, pics[2], 0);
                        break;
                    case 3://�S�[���i�{�b�N�X��u���ׂ��Ƃ��j
                        //mapmemory[posy][posx] = 3;
                        DrawGraph(posx++ * 16, posy * 16, pics[3], 0);
                        break;
                    default://�����Ȃ��Ƃ���
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
                //�����S�[���̏�ɔ������ׂď悹���Ȃ�
                if (clearflg)
                {
                    
                    DrawString(320, 120, "STAGE CLEAR", GetColor(255, 255, 255), TRUE);
                    WaitKey();
                    nowstage++;
                    break;
                }
                if (CheckHitKey(KEY_INPUT_R))//R���������烊�Z�b�g
                {
                    break;
                }
                //------------------///�����������Ƃ�-------------------------------------------------------------------------------------
                if (CheckHitKey(KEY_INPUT_LEFT))
                {
                    bool canflg = 1;//�ړ��ł��邩�ǂ������ׂ�t���O
                    bool boxflg = 0;//�{�b�N�X�𓮂������ǂ����̃t���O
                    if (!Updateflg[0])
                    {
                        if (mapmemory[playery][playerx - 1] == ��)//�ׂ���
                        {
                            canflg = 0;
                        }
                        if (mapmemory[playery][playerx - 1] == �{�b�N�X)//�ׂ��{�b�N�X�ł��ׂ̗��ǂ������̓{�b�N�X
                        {
                            if (mapmemory[playery][playerx - 2] == �� || mapmemory[playery][playerx - 2] == �{�b�N�X)
                            {
                                canflg = 0;
                            }
                            boxflg = 1;
                        }
                        if (canflg)//�������Ƃ��ł���ꍇ
                        {
                            if (mapmemory[playery][playerx - 1] == �S�[�� || mapmemory[playery][playerx - 2] == �S�[��)//�����ړ��悪�S�[���Ȃ�
                            {
                                onflg = 1;
                            }
                            if (onflg)//�ړ��悪�S�[���Ȃ�
                            {
                                //���𓮂����ꍇ��ɔ��̏���������B���̓S�[���̏�ɏ��
                                if (boxflg)
                                {
                                    mapmemory[playery][playerx] = ��;//�ړ������Ƃ���͋󔒂ɂȂ�B
                                    mapmemory[playery][playerx - 1] = �v���C���[;//���̂������Ƃ���ɂ̓v���C���[���ړ��B
                                    mapmemory[playery][playerx - 2] = �{�b�N�X;//������������̓S�[���B
                                }
                                mapmemory[playery][playerx] = ��;//�ړ������Ƃ���͋󔒂ɂȂ�B
                                mapmemory[playery][playerx - 1] = �v���C���[;//���̂������Ƃ���ɂ̓v���C���[���ړ��B
                                canflg = 0;
                            }
                            else
                            {
                                onflg = 0;

                                //���𓮂����ꍇ��ɔ��𓮂������������Ă���v���C���[������
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
                                        mapmemory[goaly[i]][goalx[i]] = �S�[��;
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
                //-----------///�E���������Ƃ�-------------------------------------------------------------------------------------
                if (CheckHitKey(KEY_INPUT_RIGHT))
                {
                    bool canflg = 1;//�ړ��ł��邩�ǂ������ׂ�t���O
                    bool boxflg = 0;//�{�b�N�X�𓮂������ǂ����̃t���O
                    if (!Updateflg[1])
                    {
                        if (mapmemory[playery][playerx + 1] == 0)//�ׂ���
                        {
                            canflg = 0;
                        }
                        if (mapmemory[playery][playerx + 1] == 1)//�ׂ��{�b�N�X�ł��ׂ̗��ǂ������̓{�b�N�X
                        {
                            if (mapmemory[playery][playerx + 2] == �{�b�N�X || mapmemory[playery][playerx + 2] == ��)
                            {
                                canflg = 0;
                            }
                            boxflg = 1;
                        }
                        if (canflg)
                        {
                            if (mapmemory[playery][playerx + 1] == �S�[�� || mapmemory[playery][playerx + 2] == �S�[��)//�����ړ��悪�S�[���Ȃ�
                            {
                                onflg = 1;
                            }
                            if (onflg)//�ړ��悪�S�[���Ȃ�
                            {
                                //���𓮂����ꍇ��ɔ��̏���������B���̓S�[���̏�ɏ��
                                if (boxflg)
                                {
                                    mapmemory[playery][playerx] = ��;//�ړ������Ƃ���͋󔒂ɂȂ�B
                                    mapmemory[playery][playerx + 1] = �v���C���[;//���̂������Ƃ���ɂ̓v���C���[���ړ��B
                                    mapmemory[playery][playerx + 2] = �{�b�N�X;//������������̓S�[���B�܂�N���A
                                }
                                else
                                {
                                    mapmemory[playery][playerx] = ��;//�ړ������Ƃ���͋󔒂ɂȂ�B
                                    mapmemory[playery][playerx + 1] = �v���C���[;//���̂������Ƃ���ɂ̓v���C���[���ړ��B
                                }

                                canflg = 0;
                            }
                            else
                            {
                                onflg = 0;
                                //���𓮂����ꍇ��ɔ��̏���������
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
                //-----------///����������Ƃ�-------------------------------------------------------------------------------------
                if (CheckHitKey(KEY_INPUT_UP))
                {
                    bool canflg = 1;//�ړ��ł��邩�ǂ������ׂ�t���O
                    bool boxflg = 0;//�{�b�N�X�𓮂������ǂ����̃t���O
                    if (!Updateflg[2])
                    {
                        if (mapmemory[playery - 1][playerx] == 0)//�ׂ���
                        {
                            canflg = 0;
                        }
                        if (mapmemory[playery - 1][playerx] == 1)//�ׂ��{�b�N�X�ł��ׂ̗��ǂ������̓{�b�N�X
                        {
                            if (mapmemory[playery - 2][playerx] == 0 || mapmemory[playery - 2][playerx] == 2)
                            {
                                canflg = 0;
                            }
                            boxflg = 1;
                        }
                        if (canflg)
                        {
                            if (mapmemory[playery - 1][playerx] == 3 || mapmemory[playery - 2][playerx] == 3)//�����ړ��悪�S�[���Ȃ�
                            {
                                onflg = 1;
                            }
                            if (onflg)//�ړ��悪�S�[���Ȃ�
                            {
                                //���𓮂����ꍇ��ɔ��̏���������B���̓S�[���̏�ɏ��
                                if (boxflg)
                                {
                                    mapmemory[playery][playerx] = ��;//�ړ������Ƃ���͋󔒂ɂȂ�B
                                    mapmemory[playery - 1][playerx] = �v���C���[;//���̂������Ƃ���ɂ̓v���C���[���ړ��B
                                    mapmemory[playery - 2][playerx] = �{�b�N�X;//������������̓S�[���B�܂�N���A
                                }
                                else
                                {
                                    mapmemory[playery][playerx] = ��;//�ړ������Ƃ���͋󔒂ɂȂ�B
                                    mapmemory[playery - 1][playerx] = �v���C���[;//���̂������Ƃ���ɂ̓v���C���[���ړ��B
                                    canflg = 0;
                                }
                            }
                            else
                            {
                                onflg = 0;
                                //���𓮂����ꍇ��ɔ��̏���������
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
                //----------------///�����������Ƃ�-------------------------------------------------------------------------------------
                if (CheckHitKey(KEY_INPUT_DOWN))
                {
                    bool canflg = 1;//�ړ��ł��邩�ǂ������ׂ�t���O
                    bool boxflg = 0;//�{�b�N�X�𓮂������ǂ����̃t���O
                    if (!Updateflg[3])
                    {
                        if (mapmemory[playery + 1][playerx] == ��)//�ׂ���
                        {
                            canflg = 0;
                        }
                        if (mapmemory[playery + 1][playerx] == �{�b�N�X)//�ׂ��{�b�N�X�ł��ׂ̗��ǂ������̓{�b�N�X
                        {
                            if (mapmemory[playery + 2][playerx] == �� || mapmemory[playery + 2][playerx] == �{�b�N�X)
                            {
                                canflg = 0;
                            }
                            boxflg = 1;
                        }
                        if (canflg)
                        {
                            if (mapmemory[playery + 1][playerx] == �S�[�� || mapmemory[playery + 2][playerx] == �{�b�N�X)//�����ړ��悪�S�[���Ȃ�
                            {
                                onflg = 1;
                            }
                            if (onflg)//�ړ��悪�S�[���Ȃ�
                            {
                                //���𓮂����ꍇ��ɔ��̏���������B���̓S�[���̏�ɏ��
                                if (boxflg)
                                {
                                    mapmemory[playery][playerx] = ��;//�ړ������Ƃ���͋󔒂ɂȂ�B
                                    mapmemory[playery + 1][playerx] = �v���C���[;//���̂������Ƃ���ɂ̓v���C���[���ړ��B
                                    mapmemory[playery + 2][playerx] = �{�b�N�X;//������������̓S�[���B�܂�N���A
                                }
                                else
                                {
                                    mapmemory[playery][playerx] = 5;//�ړ������Ƃ���͋󔒂ɂȂ�B
                                    mapmemory[playery + 1][playerx] = 2;//���̂������Ƃ���ɂ̓v���C���[���ړ��B
                                }
                                canflg = 0;
                            }
                            else
                            {
                                onflg = 0;
                                //���𓮂����ꍇ��ɔ��̏���������
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
                                        mapmemory[goaly[i]][goalx[i]] = �S�[��;
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
                //�G���[�̏ꍇ�̓��[�v�𔲂���
                //-------------------------------//
                if (ProcessMessage() != 0)
                {
                    break;
                }
            }
        }

        //�Q�[���̏I��
        return 0;
    }
     
    