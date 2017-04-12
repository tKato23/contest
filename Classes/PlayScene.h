//	���d�C���N���[�h�̖h�~
#pragma once

//	�w�b�_�t�@�C���̃C���N���[�h
#include "cocos2d.h"
#include "ui/CocosGUI.h"
//	box2d���g���ɂ͕K���K�v
#include "Box2D/Box2D.h"
#include "GLES-Render.h"


// �v���C�V�[��
class PlayScene : public cocos2d::Scene
{
public:
	~PlayScene();				//	�f�X�g���N�^

	void update(float dt);		//	�X�V����
	void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4& transform, uint32_t flags);		//	�`�揈��

	//	�^�b�`�֌W�̏���
	bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unsed_event);			
	void onTouchMoved(cocos2d::Touch * touch, cocos2d::Event * unsed_event);
	void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unsed_event);

	void ojamaCreate(float delta);		//	���ז������������
	void ringCreate(cocos2d::Vec2 v);	//	�����O���쐬����	

	cocos2d::Vec2 TouchTo2dWorld(cocos2d::Vec2 touchPos);

	// create�֐��̐錾�ƒ�`
	//CREATE_FUNC(PlayScene);
	static PlayScene* create();

	bool init();						//	������

	void initPhysics();					//	�������[���h�̏�����

	b2World *m_pWorld;					//	�������[���h�̍쐬
	GLESDebugDraw *m_pDraw;				//	DrawNode��ۑ�����ϐ�
	b2Body *body2;						//	�����x����{�f�B
	b2Body *m_body;						//	�͂��ޕ��̃{�f�B
	b2Body *m_floorBody;				//	���̃{�f�B
	int m_nextTag;						//	�^�O��ۑ�

	cocos2d::Sprite* m_box;				//	�͂��ޕ��p�̃X�v���C�g	
	cocos2d::Sprite* m_floor;			//	���p�̃X�v���C�g
	cocos2d::Sprite* m_ring;			//	�����O�p�̃X�v���C�g
	cocos2d::Sprite* tag;				//	����p�X�v���C�g
	cocos2d::Sprite* m_sprite;			//	���ז��p�̃X�v���C�g

	cocos2d::Camera* m_pCamera;			//	�v���C�V�[���̃J����

	bool m_OverFlag;					//	true�ŃQ�[���I�[�o�[�ɂ���
};
