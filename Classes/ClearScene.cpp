//	�w�b�_�t�@�C���̃C���N���[�h
#include "ClearScene.h"
#include "PlayScene.h"

USING_NS_CC;

//	create�֐��̒�`
ClearScene* ClearScene::create()
{
	//	�������̊m��
	ClearScene *pRet = new(std::nothrow) ClearScene();
	//	���������m�ۂł��A����ɏ������ł�����
	if (pRet && pRet->init())
	{
		//	�����J���}�l�[�W���ɓo�^
		pRet->autorelease();
		//	�����������I�u�W�F�N�g��return����
		return pRet;
	}
	else
	{
		//	�������ŃG���[���o���̂ŁA���������J�����Ĉُ�I��
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

// ������
bool ClearScene::init()
{
	// ���N���X�̏�����
	if (!Scene::init())
	{
		// ���N���X�̏����������s�Ȃ�A�ُ�I��
		return false;
	}

	//	�w�i�摜�̐ݒ�
	back = Sprite::create("GameClear.png");
	back->setPosition(320.0f, 480.0f);
	this->addChild(back);

	//	�J�E���g�̏�����
	time = 0;

	// ���t���[���X�V��L����
	scheduleUpdate();

	return true;
}

// ���t���[���X�V
void ClearScene::update(float delta)
{
	//	�J�E���g��1���₷
	time++;

	//	�J�E���g��250�ȏ�ɂȂ�����V�[����؂�ւ���
	if (time > 250)
	{
		// ���̃V�[�����쐬����
		Scene* nextScene = PlayScene::create();

		//// ���̃V�[���Ɉڍs
		_director->replaceScene(nextScene);
	}
}