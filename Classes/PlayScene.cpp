//	�w�b�_�t�@�C���̃C���N���[�h

//	box2D���g���ۂɂ͕K���K�v
#include "Box2D/Box2D.h"
#include "audio\include\AudioEngine.h"
#include <ctime>
#include "PlayScene.h"
#include "ClearScene.h"
#include "OverScene.h"

//	���s�N�Z����1���[�g����
#define PTM_RATIO 32

USING_NS_CC;

//	create�֐��̒�`
PlayScene* PlayScene::create()
{
	//	�������̊m��
	PlayScene *pRet = new(std::nothrow) PlayScene();
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

//	�����V�X�e���̏�����
void PlayScene::initPhysics()
{
	//	�������[���h�̏d�͂�ݒ�
	b2Vec2 gravity;
	//	�������ɖ��t���[��5m����
	gravity.Set(0.0f, -5.0f);
	// ���[���h�𐶐�
	m_pWorld = new b2World(gravity);

	//	�\���p�̃C���X�^���X���쐬
	m_pDraw = new GLESDebugDraw(PTM_RATIO);
	//	�S��ޕ\��(�Sbit�}�X�N)
	uint32 flags = 0xffffffff;
	m_pDraw->SetFlags(flags);
	//	�\���p�C���X�^���X���C���X�^���X�����[���h�ɃZ�b�g
	m_pWorld->SetDebugDraw(m_pDraw);

	//	�ǑS��1�̃{�f�B�̂���
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0, 0); // ��ʍ������Z���^�[��

    b2Body* groundBody = m_pWorld->CreateBody(&groundBodyDef);

	//�@��ʍ���(�s�N�Z���j
	b2Vec2 leftbottom(0, 0);
	//	��ʉE��(�s�N�Z���j
	b2Vec2 rightbottom(960, 0);
	//	��ʍ���(�s�N�Z���j
	b2Vec2 lefttop(0, 640);
	//	��ʉE��(�s�N�Z���j
	b2Vec2 righttop(960, 640);

	b2Vec2 boxLeftbottom(430, 270);
	b2Vec2 boxRightbottom(530, 270);
	b2Vec2 boxLefttop(430, 370);
	b2Vec2 boxRighttop(530, 370);

	//	�s�N�Z�������[�g���ɕϊ�
	leftbottom.x /= PTM_RATIO;
	leftbottom.y /= PTM_RATIO;
	rightbottom.x /= PTM_RATIO;
	rightbottom.y /= PTM_RATIO;
	lefttop.x /= PTM_RATIO;
	lefttop.y /= PTM_RATIO;
	righttop.x /= PTM_RATIO;
	righttop.y /= PTM_RATIO;


	boxLeftbottom.x /= PTM_RATIO;
	boxLeftbottom.y /= PTM_RATIO;
	boxRightbottom.x /= PTM_RATIO;
	boxRightbottom.y /= PTM_RATIO;
	boxLefttop.x /= PTM_RATIO;
	boxLefttop.y /= PTM_RATIO;
	boxRighttop.x /= PTM_RATIO;
	boxRighttop.y /= PTM_RATIO;

	//	�V�F�C�v�ݒ�p�̃C���X�^���X(�g���܂킷�j
	b2EdgeShape groundBox;

	//////////////////////////////////////////
	//	�����x����X�^�e�B�b�N�{�f�B�̍쐬	//
	//////////////////////////////////////////

	//	�l�p�`�̌`��̃f�[�^�����
	b2PolygonShape staticBox;
	//	1m�l���̐����`���w��(0.5�͈�ӂ̒����̔����j
	staticBox.SetAsBox(9.0f, 0.5f);

	//	���[���h�ɐV���ȃ_�C�i�~�b�N�ǉ�����
	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.position.Set(480 / PTM_RATIO, 1260 / PTM_RATIO);
	body2 = m_pWorld->CreateBody(&bodyDef);

	//	�{�f�B�Ƀ^�O�̐ݒ�
	body2->SetUserData(tag);

	//	�X�^�e�B�b�N�{�f�B�Ɏl�p�`�̃t�B�N�`����ǉ�����
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &staticBox;

	body2->CreateFixture(&fixtureDef);

	//	�l�p�`�̌`��̃f�[�^�����
	b2PolygonShape dynamicfloor;
	//	1m�l���̐����`���w��(0.5�͈�ӂ̒����̔����j
	dynamicfloor.SetAsBox(9.0f, 0.5f);

	//	���p�̃X�v���C�g���쐬
	m_floor = Sprite::create("don.png");
	m_floor->setScaleX(2.7f);
	m_floor->setScaleY(0.3f);
	this->addChild(m_floor);

	//	���[���h�ɐV���ȃ_�C�i�~�b�N�{�f�B(���j�ǉ�����
	b2BodyDef floorBodyDef;
	floorBodyDef.type = b2_dynamicBody;
	floorBodyDef.position.Set(480 / PTM_RATIO, 820 / PTM_RATIO);
	floorBodyDef.userData = m_floor;
	b2Body* floorBody = m_pWorld->CreateBody(&floorBodyDef);

	//	�_�C�i�~�b�N�{�f�B�Ɏl�p�`�̃t�B�N�`����ǉ�����
	b2FixtureDef floorFixtureDef;
	floorFixtureDef.shape = &dynamicfloor;
	floorFixtureDef.restitution = 0.0f;				//	�����W���̐ݒ�
	floorFixtureDef.density = 1.0f;					//	���x�̐ݒ�
	floorFixtureDef.friction = 1000.0f;
	floorBody->CreateFixture(&floorFixtureDef);

	//	��(�{�f�B)��ۑ�
	m_floorBody = floorBody;

	//	�f�B�X�^���X�W���C���g�̐ݒ�
	b2DistanceJointDef jd;
	b2Vec2 anchorA = body2->GetWorldCenter();
	b2Vec2 anchorB = floorBody->GetWorldCenter();
	jd.Initialize(body2, floorBody, anchorA, anchorB);
	jd.collideConnected = true;	 //	A��B���݂��ɏՓ˂��邩�ǂ���
	jd.frequencyHz = 0.4f;		 //	�o�l�̉^������(1�b�ŉ������邩�j
	jd.dampingRatio = 0.7f;		 //	�o�l�̌ł�(0�`1�j
	DrawNode* drawNode = DrawNode::create();
	jd.userData = drawNode;
	this->addChild(drawNode);
	m_pWorld->CreateJoint(&jd);

	
	//	�����O�T�̐ݒ�
	for (int i = 0; i < 5; i++)
	{
		//	�^�񒆂̃����O�����ݒu���Ȃ�
		if (i != 2)
		{
			//	���W�������ɓn���č쐬
			Vec2 vec = { -288.0f + 144.0f * i, 0.0f };
			ringCreate(vec);
		}
	}
	

	////	�n��
	//groundBox.Set(leftbottom, rightbottom);
	//groundBody->CreateFixture(&groundBox, 0);

	////�@�V��
	//groundBox.Set(lefttop, righttop);
	//groundBody->CreateFixture(&groundBox, 0);

	////	���[
	//groundBox.Set(lefttop, leftbottom);
	//groundBody->CreateFixture(&groundBox, 0);

	////	�E�[
	//groundBox.Set(righttop, rightbottom);
	//groundBody->CreateFixture(&groundBox, 0);

}

//	�f�X�g���N�^
PlayScene::~PlayScene()
{
	//	���
	delete m_pDraw;
	delete m_pWorld;
}

//	�Q�[���̏�����
bool PlayScene::init()
{
	// ���N���X�̏�����
	if (!Scene::init())
	{
		// ���N���X�̏����������s�Ȃ�A�ُ�I��
		return false;
	}

	//	�������V���b�t��
	srand(static_cast < unsigned int > (time(NULL)));

	//	�^�b�`��L���ɂ���
	//	�C�x���g���X�i�[���쐬����
	EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(PlayScene::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(PlayScene::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(PlayScene::onTouchEnded, this);

	//	�C�x���g���X�i�[��o�^����
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	//	�J�����𑀍�ł���悤�ɂ���
	m_pCamera = getDefaultCamera();

	//	�J�����������������ʒu�Ɉړ�
	m_pCamera->setPositionX(470.0f);
	m_pCamera->setPositionZ(1200.0f);

	//	�^�O�ԍ��̏�����
	m_nextTag = 0;	

	//	���W���}�����Ԋu�ŗ��Ƃ�
	schedule(CC_CALLBACK_1(PlayScene::ojamaCreate, this), 2.0f, "ojama");

	//	�w�i�̐ݒ�
	Sprite* back = Sprite::create("patapon.png");
	back->setPosition(480.0f, 320.0f);
	back->setScaleX(0.4f);
	back->setScaleY(0.3f);
	this->addChild(back);

	//	�������[���h�̏���������
	initPhysics();
	//	�A�b�v�f�[�g�̗L����
	scheduleUpdate();

	//	�Q�[���I�[�o�[�t���O�̏�����
	m_OverFlag = false;

	//	�����������Ȃ̂�true��Ԃ�
	return true;
}

//	�X�V����
void PlayScene::update(float dt)
{
	//	�������[���h�̍X�V(���Ԃ�i�߂�j
	m_pWorld->Step(1.0f / 60.0f, 8, 3);

	//	�������[���h���̑S�Ẵ{�f�B�ɂ��Ẵ��[�v����
	for (b2Body* body = m_pWorld->GetBodyList();
	body != nullptr;
		body = body->GetNext())
	{
		//	�{�f�B�̉摜�f�[�^���擾
		Sprite* box = (Sprite*)body->GetUserData();

		//	�{�f�B�̍��W���擾
		b2Vec2 pos = body->GetPosition();

		//	�{�f�B��nullptr�ł͂Ȃ�
		if (box != nullptr)
		{
			//	�{�f�B�̈ʒu�ɍ��킹�ĉ摜�𓮂�������
			box->setPosition(Vec2(pos.x * PTM_RATIO, pos.y * PTM_RATIO));

			//	�������[���h���̍��̂̊p�x���擾���āA
			float rot = body->GetAngle();
			//	�����p�x��Sprite����]
			box->setRotation(-CC_RADIANS_TO_DEGREES(rot));
		}
		
		//	�{�f�B�̉摜�����Ȃ�true
		if (body->GetUserData() == m_floor)
		{
			//	���̈ʒu���ݒ�̈ʒu�ȉ���true
			if (body->GetPosition().y * PTM_RATIO < -400.0f)
			{
				//	�Q�[���I�[�o�[�t���O�������Ă�����I�[�o�[�V�[���ɔ��
				//	�Q�[���I�[�o�[�t���O�̓{�f�B�̊p�x�����ĕύX�����@
				if (m_OverFlag)
				{
					//	�V�[���̍쐬
					Scene* nextScene = OverScene::create();

					// ���̃V�[���Ɉڍs
					_director->replaceScene(nextScene);
					break;
				}

				//	�V�[���̍쐬
				Scene* nextScene = ClearScene::create();

				// ���̃V�[���Ɉڍs
				_director->replaceScene(nextScene);
			}

			//	�Q�[���I�[�o�[�t���O�̂��߂Ɋp�x���擾����
			float rot = body->GetAngle();
			//	�擾�����p�x�̓��W�A���Ȃ̂œx�ɕϊ�����
			rot = CC_RADIANS_TO_DEGREES(rot);

			//	�ݒ肵���p�x�ȏ��true
			if (rot > 90 || rot < -90)
			{

				//	��̏����𖞂�������W���C���g���؂�ăQ�[���I�[�o�[�̃t���O������
				m_OverFlag = true;

			}
		}


		//	�{�f�B����ʊO�i���j�ɏo����폜
		if (pos.y < -400.0f / PTM_RATIO)
		{
			//	�{�f�B�̍폜
			m_pWorld->DestroyBody(body);
			//	�摜�̍폜
			box->removeFromParent();
		}


	}

	//	�W���C���g�̎��o���̂��߂�DrawNode���쐬
	DrawNode* drawNode = DrawNode::create();
	//drawNode->drawSegment()
	//	�����X�V���邽�߂Ɉ�x����
	drawNode->clear();
	this->addChild(drawNode);

		//�������[���h���̃W���C���g�ɂ��Ă̏���
	for (b2Joint* joint = m_pWorld->GetJointList();
	joint != nullptr;
		joint = joint->GetNext())
	{
		//	�W���C���g�Ɋo�������Ă���W���C���g�����擾
		DrawNode* drawNode = (DrawNode*)joint->GetUserData();

		//	�W���C���g��A,B���W��vec2�ɕϊ�
		b2Vec2 p1 = joint->GetAnchorA();
		Vec2 pos1 = { p1.x * PTM_RATIO , p1.y * PTM_RATIO };
		b2Vec2 p2 = joint->GetAnchorB();
		Vec2 pos2 = { p2.x * PTM_RATIO, p2.y * PTM_RATIO };

		//	��������Ȃ�true
		if (drawNode != nullptr)
		{
			//	�����X�V�̂��߈�x����
			drawNode->clear();
			//	�F�̐�������
			drawNode->drawSegment(pos1, pos2, 3.0f, Color4F(0.1f, 0.1f, 1.0f, 1));

		}

		//	���C���̃W���C���g��T��
		if (joint->GetBodyA()->GetUserData() == tag)
		{
			b2Vec2 pos = joint->GetAnchorB();

			//	�W���C���g�����ȏ�Ȃ�W���C���g�̐F��ς���(��)
			if (pos.y <= 380.0f / PTM_RATIO)
			{
				//	���F�̐�������
				drawNode->drawSegment(pos1, pos2, 3.0f, Color4F(1.0f, 1.0f, 0.0f, 1));
			}

			//	�W���C���g�����ȏ�Ȃ�W���C���g�̐F��ς���(��)
			if (pos.y <= 320.0f / PTM_RATIO)
			{
				//	�ԐF�̐�������
				drawNode->drawSegment(pos1, pos2, 3.0f, Color4F(1.0f, 0.0f, 0.0f, 1));
			}

			//	���W�̈ʒu�����𒴂�����W���C���g��؂�
			//	�������̓Q�[���I�[�o�[�t���O������
			if (pos.y <= 280.0f / PTM_RATIO || m_OverFlag)
			{
				//	�W���C���g������	���͏����Ă��Ȃ��̂Ŏc��
				m_pWorld->DestroyJoint(joint);

			}
			
		}
	}


	//	�{�f�B��nullptr�ł͂Ȃ��@���@�{�f�B���Ԃ牺����I�u�W�F�N�g�̉摜
	if (m_body != nullptr && m_box == (Sprite*)m_body->GetUserData())
	{
		//	���ׂĂ̂Ԃ牺����I�u�W�F�N�g���`�F�b�N���郋�[�v
		for (b2ContactEdge* list = m_body->GetContactList();	//	���X�g�̐擪
		list != nullptr;
			list = list->next)	//	���X�g��i�߂�
		{
			//	�Փˑ���̎擾
			b2Body* other = list->other;
			//	�R���^�N�g���擾
			b2Contact* contact = list->contact;
			//	�Ԃ牺����I�u�W�F�N�g�ɃW���C���g�����݂��Ă��邩�ǂ���
			b2JointEdge* list1 = m_body->GetJointList();
			//	�Ԃ牺����I�u�W�F�N�g�ɃW���C���g�����݂��Ă��邩�ǂ����𒲂ׂ�
			if (list1 == nullptr)
			{
				//	�Փˑ��肪���Ȃ�W���C���g���Ȃ�
				if (m_floor != (Sprite*)other->GetUserData())
				{
					//	�Փˑ���̉摜���擾
					Sprite* spr = (Sprite*)other->GetUserData();
					//	�摜�������Ă���ꍇtrue
					if (spr != nullptr)
					{
						//	�摜�ɂ��Ă���tag��10�̏ꍇtrue
						if (10 != spr->getTag())
						{
							//	�f�B�X�^���X�W���C���g�̐ݒ�
							b2DistanceJointDef jd;							//	�W���C���g�̐ݒ���s�����߂̕ϐ�
							b2Vec2 anchorA = m_body->GetWorldCenter();		//	�W���C���g�ɂȂ���{�f�BA�̍��W
							b2Vec2 anchorB = other->GetWorldCenter();		//	�W���C���g�ɂȂ���{�f�BB�̍��W
							jd.Initialize(m_body, other, anchorA, anchorB);	//	�W���C���g�̏����ݒ�	
							jd.collideConnected = true;						//	A��B���݂��ɏՓ˂��邩�ǂ���
							jd.frequencyHz = 1.0f;							//	�o�l�̉^������(1�b�ŉ������邩�j
							jd.dampingRatio = 1.0f;							//	�o�l�̌ł�(0�`1�j
							DrawNode* drawNode = DrawNode::create();		//	�W���C���g�̐����쐬����
							jd.userData = drawNode;							//	���̏����W���C���g�Ɋo�������Ă���
							this->addChild(drawNode);						//	(*'��'*)
							m_pWorld->CreateJoint(&jd);						//	�W���C���g��ǉ�����
						}
					}
				}
			}
		}
	}
}

//	�\���֐����I�[�o�[���C�h
void PlayScene::draw(Renderer * renderer, const Mat4 & transform, uint32_t flags)
{
	//	�������[���h���f�o�b�O�\��
	m_pWorld->DrawDebugData();
}

//	�^�b�`�J�n���̏���
bool PlayScene::onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * unsed_event)
{
	//	�^�b�`���W���v�Z���Ċ���o��
	Vec2 p = TouchTo2dWorld(touch->getLocation());

	//	�l�p�`�̌`��̃f�[�^�����
	b2PolygonShape dynamicBox;
	//	1m�l���̐����`���w��(0.5�͈�ӂ̒����̔����j
	dynamicBox.SetAsBox(0.5f, 0.5f);

	//	�͂��ޕ��̃X�v���C�g�̍쐬
	m_box = Sprite::create("monhan1.png");
	m_box->setScale(0.2f);
	this->addChild(m_box);

	//���[���h�ɐV���ȃ_�C�i�~�b�N�{�f�B��ǉ�����
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;							//	�`���ݒ�
	bodyDef.position.Set(p.x / PTM_RATIO, p.y / PTM_RATIO);	//	�����ʒu��ݒ�
	bodyDef.userData = m_box;								//	�Ԃ牺����摜���o���Ă���	
	b2Body *body = m_pWorld->CreateBody(&bodyDef);			//	�{�f�B�̐ݒ��K��

	//�_�C�i�~�b�N�{�f�B�Ɏl�p�`�̃t�B�N�`����ǉ�����
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;		//	�`��̐ݒ�
	fixtureDef.density = 1.0f;			//	���x�̐ݒ�
	fixtureDef.restitution = 1;			//	�����W���̐ݒ�	
	body->SetActive(false);				//	�Ԃ牺������̂𐶐������i�K�ł̓{�f�B�͏Փ˂��Ȃ�
	body->CreateFixture(&fixtureDef);	//	�{�f�B�ɐݒ肵���t�B�N�X�`����o�^����


	//	�ŐV�̃{�f�B��ۑ�����
	m_body = body;


	return true;
}

void PlayScene::onTouchMoved(Touch * touch, cocos2d::Event * unsed_event)
{
	//	�^�b�`�������W���J�������猩���X�N���[�����W�����[���h���W��
	Vec2 p = TouchTo2dWorld(touch->getLocation());

	//	�}�E�X�𓮂������Ƃ��}�E�X�ɍ��킹�č��W���X�V����
	b2Vec2 b2pos = { p.x / PTM_RATIO, p.y / PTM_RATIO };
	m_body->SetTransform(b2pos, 0.0f);
}

//	�^�b�`�I�����̏���
void PlayScene::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unsed_event)
{
	//	�͂��ޕ��̃{�f�B���_�C�i�~�b�N�{�f�B�ɕύX����
	m_body->SetType(b2_dynamicBody);
	m_body->SetActive(true);
	//	�͂��ޕ��̃{�f�B�̏d�͂�ݒ�
	m_body->SetGravityScale(4.0f);
}

//	���ז��������_���ȏꏊ�Ɉ��������
void PlayScene::ojamaCreate(float delta)
{
	//	���ז��̎�ނ������_���ɂ��邽�߂̕ϐ��ݒ�
	int r = rand() % 3;


		//	���ז��p�̃X�v���C�g�̍쐬
		m_sprite = Sprite::create("ojamapuyo.png");
		m_sprite->setPosition(Vec2(1000.0f, 1000.0f));
		m_sprite->setScale(0.3f + (0.3 * r));
		m_sprite->setTag(10);
		this->addChild(m_sprite);

		//	�~�`�̌`��̃f�[�^�����
		b2CircleShape circle;
		//	1m�l���̉~�`���w��(0.5�͈�ӂ̒����̔����j
		circle.m_radius = 0.5f + (0.5 * r);

		//	���[���h�ɐV���ȃ_�C�i�~�b�N�{�f�B�ǉ�����
		b2BodyDef ojamaDef;
		ojamaDef.type = b2_dynamicBody;
		ojamaDef.position.Set((rand() % 440 + 100) / PTM_RATIO, 1100.0f / PTM_RATIO);
		ojamaDef.userData = m_sprite;
		b2Body* ojamaBody = m_pWorld->CreateBody(&ojamaDef);
		ojamaBody->SetGravityScale(2.0f);				//	���ז��̏d�͂�ݒ�

		//	�_�C�i�~�b�N�{�f�B�ɉ~�`�̃t�B�N�`����ǉ�����
		b2FixtureDef ojamaFixtureDef;
		ojamaFixtureDef.shape = &circle;
		ojamaFixtureDef.restitution = 0.0f; //0.5f;
		ojamaFixtureDef.density = 0.5f + (0.5 * r);
		ojamaFixtureDef.friction = 10.0f;
		ojamaBody->CreateFixture(&ojamaFixtureDef);

}

//	�����O���쐬����
void PlayScene::ringCreate(Vec2 v)
{
	//	��������Ɉʒu�����炷
	b2Vec2 b2vec2;
	b2vec2.x = v.x / PTM_RATIO;
	b2vec2.y = (v.y - 16.0f) / PTM_RATIO;

	//	�����O�̉摜�̐ݒ�
	m_ring = Sprite::create("ball.png");
	m_ring->setScale(0.2f);
	this->addChild(m_ring);

	//	�~�`�̌`��̃f�[�^�����
	b2CircleShape dynamicRing;
	//	1m�l���̐����`���w��(0.5�͈�ӂ̒����̔����j
	dynamicRing.m_radius = 1.5f;

	//	���[���h�ɐV���ȃ_�C�i�~�b�N�{�f�B(�����O�j�ǉ�����
	b2BodyDef ringBodyDef;
	ringBodyDef.type = b2_dynamicBody;
	ringBodyDef.userData = m_ring;
	b2Body* ringBody = m_pWorld->CreateBody(&ringBodyDef);
	ringBody->SetGravityScale(2.0f);

	//	�_�C�i�~�b�N�{�f�B�Ɏl�p�`�̃t�B�N�`����ǉ�����
	b2FixtureDef ringFixtureDef;
	ringFixtureDef.shape = &dynamicRing;
	ringFixtureDef.restitution = 0.5;
	ringFixtureDef.density = 1.0f;
	ringBody->CreateFixture(&ringFixtureDef);

	//	�f�B�X�^���X�W���C���g�̐ݒ�
	b2DistanceJointDef jdRing;
	b2Vec2 ringAnchorA = m_floorBody->GetWorldCenter();
	b2Vec2 ringAnchorB = ringBody->GetWorldCenter();
	jdRing.Initialize(m_floorBody, ringBody, ringAnchorA + b2vec2, ringAnchorB);
	jdRing.collideConnected = true;	 //	A��B���݂��ɏՓ˂��邩�ǂ���
	jdRing.frequencyHz = 1.0f;		 //	�o�l�̉^������(1�b�ŉ������邩�j
	jdRing.dampingRatio = 1.0f;		 //	�o�l�̌ł�(0�`1�j
	jdRing.length = 0.1f;
	m_pWorld->CreateJoint(&jdRing);

	ringBody->SetTransform(ringAnchorA + b2vec2,0);
}

Vec2 PlayScene::TouchTo2dWorld(Vec2 touch)
{

	//	��O�A�����`����
	Vec3 nearP(touch.x, touch.y, -1.0f), farP(touch.x, touch.y, 1.0f);

	//	��ʂ̃T�C�Y�̎擾
	auto size = Director::getInstance()->getWinSize();
	//	3D���W��Ԃɕϊ�
	nearP = m_pCamera->unproject(nearP);
	farP = m_pCamera->unproject(farP);
	//	��O���牜�Ɍ������x�N�g��
	Vec3 dir(farP - nearP);
	//	�J��������2D���[���h�ւ̋���
	float dist = 0.0f;
	//	2D���[���h�ւ̖@�������ƂɎ�O���牜�̋��������߂�
	float ndd = Vec3::dot(Vec3(0, 0, 1), dir);
	//	ndd�����s���ǂ���
	if (ndd == 0)
		dist = 0.0f;
	//	��O��2D���[���h�̋���
	float ndo = Vec3::dot(Vec3(0, 0, 1), nearP);
	//	2D���[���h�ւ̋����̊��������߂�
	dist = (0 - ndo) / ndd;
	//	���������Ƃ�2D���[���h�ւ̃x�N�g�����Z�o
	Vec3 pos = nearP + dist *  dir;

	//Mat4 nodeToWorld = m_pCamera->getNodeToWorldTransform();
	//Vec3 vec3(pos.x, pos.y, 0);
	//vec3 = m_pCamera->unproject(vec3);
	//Vec3 ret;
	//nodeToWorld.transformPoint(vec3, &ret);

	//	���߂����W��vec2�ɂ���
	Vec2 p = Vec2(pos.x, _director->getWinSize().height - pos.y);
	return p;
}