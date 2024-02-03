#pragma once
#include"DxLib.h"
//�u���b�N�Ǐ��
#define BLOCK_LEFT  0x01        //0001
#define BLOCK_RIGHT 0x02        //0010
#define BLOCK_UP    0x04        //0100
#define BLOCK_DOWN  0x08        //1000

typedef unsigned char BlockInfo;

/// <summary>
///�����蔻���` 
/// /// </summary>
struct sHitRect
{
	float w;      //�������`���s�N�Z��
	float h;      //�������`�����s�N�Z��
	float worldLX;//����X���W
	float worldLY;//����Y���W
	float worldRX;//�E��X���W
	float worldRY;//�E��Y���W
};
/// <summary>
/// �����蔻�菉����
/// </summary>
/// /// <param name="detRect">�����������������蔻��ϐ�</param>
/// /// <param name="w">�����蔻��̕�</param>
/// /// <param name="h">�����蔻��̍���</param>
void initRect(sHitRect& dstRect, float w, float h);


/// <summary>
/// �����蔻��̈ړ��X�V����
///  </summary>
///  <param name="dstRect">�X�V�����������蔻��ϐ�</param>
///  <param name="w">�����蔻��̍���x���W</param>
///  <param name="h">�����蔻��̍���y���W</param>
void updateWorldRect(sHitRect& dstRect, float x, float y);


/// <summary>
/// �����蔻���`�̕`��
/// </summary> 
/// <param name="rect">�`�悵���������蔻���`</param>
void drawRect(const sHitRect& rect);

/// <summary>
/// �Q�̋�`���Փ˂�������Ԃ�
/// </summary>
/// <param name="rect1">��`�P</param>
/// <param name="rect2">��`�Q</param>
/// <returns>��`�̂Q���Փ˂�����true/false�ŕԂ�</returns>
bool isHitRect(const sHitRect& rect1, const sHitRect& rect2);
/// <summary>
/// 2�̋�`�̏Փ˂߂荞�ݗʂ��v�Z���AmobaleRect�ʒu��ύX����
/// </summary>
/// <param name="movebleRect">�ړ��\��`</param>
/// <param name="statucRect">�Î~��`�i�����Ȃ��u���b�N�Ȃǁj</param>
void calcFixHitPositon(sHitRect& movebleRect, const sHitRect& staticRect);
/// <summary>
///2�̋�`�̏Փ˂߂荞�ݗʂ��v�Z���AmobaleRect�ʒu��ύX����
/// </summary>
/// /// <param name="movebleRect">�ړ��\��`</param>
///  <param name="staticRect">>�Î~��`(�����Ȃ��u���b�N�Ȃ�)</param
///  <param name="blockinfo">�u���b�N���</param>
void calcFixHitPositon(sHitRect& movebleRect, const sHitRect& staticRect,BlockInfo blockinfo);