//	���d�C���N���[�h�̖h�~
#pragma once

//	�w�b�_�t�@�C���̃C���N���[�h
#include "cocos2d.h"
#include "ui/CocosGUI.h"

// �I�[�o�[�V�[��
class OverScene : public cocos2d::Scene
{
public:
	// create�֐��̐錾�ƒ�`
	//CREATE_FUNC(OverScene);
	static OverScene* create();

	//	������
	bool init();

	//	�X�V����
	void update(float delta) override;

	cocos2d::Sprite* back;		//	�w�i�摜

	int time;					//	�V�[���؂�ւ��p�J�E���g�ϐ�
};
