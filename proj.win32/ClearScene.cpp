//	�w�b�_�t�@�C���̃C���N���[�h
#include "ClearScene.h"

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

}