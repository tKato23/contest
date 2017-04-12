//	ヘッダファイルのインクルード

//	box2Dを使う際には必ず必要
#include "Box2D/Box2D.h"
#include "audio\include\AudioEngine.h"
#include <ctime>
#include "PlayScene.h"
#include "ClearScene.h"
#include "OverScene.h"

//	何ピクセルで1メートルか
#define PTM_RATIO 32

USING_NS_CC;

//	create関数の定義
PlayScene* PlayScene::create()
{
	//	メモリの確保
	PlayScene *pRet = new(std::nothrow) PlayScene();
	//	メモリが確保でき、正常に初期化できたら
	if (pRet && pRet->init())
	{
		//	自動開放マネージャに登録
		pRet->autorelease();
		//	初期化したオブジェクトをreturnする
		return pRet;
	}
	else
	{
		//	初期化でエラーが出たので、メモリを開放して異常終了
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

//	物理システムの初期化
void PlayScene::initPhysics()
{
	//	物理ワールドの重力を設定
	b2Vec2 gravity;
	//	下方向に毎フレーム5m加速
	gravity.Set(0.0f, -5.0f);
	// ワールドを生成
	m_pWorld = new b2World(gravity);

	//	表示用のインスタンスを作成
	m_pDraw = new GLESDebugDraw(PTM_RATIO);
	//	全種類表示(全bitマスク)
	uint32 flags = 0xffffffff;
	m_pDraw->SetFlags(flags);
	//	表示用インスタンスをインスタンスをワールドにセット
	m_pWorld->SetDebugDraw(m_pDraw);

	//	壁全体1つのボディのする
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0, 0); // 画面左下をセンターに

    b2Body* groundBody = m_pWorld->CreateBody(&groundBodyDef);

	//　画面左下(ピクセル）
	b2Vec2 leftbottom(0, 0);
	//	画面右下(ピクセル）
	b2Vec2 rightbottom(960, 0);
	//	画面左上(ピクセル）
	b2Vec2 lefttop(0, 640);
	//	画面右上(ピクセル）
	b2Vec2 righttop(960, 640);

	b2Vec2 boxLeftbottom(430, 270);
	b2Vec2 boxRightbottom(530, 270);
	b2Vec2 boxLefttop(430, 370);
	b2Vec2 boxRighttop(530, 370);

	//	ピクセル→メートルに変換
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

	//	シェイプ設定用のインスタンス(使いまわす）
	b2EdgeShape groundBox;

	//////////////////////////////////////////
	//	床を支えるスタティックボディの作成	//
	//////////////////////////////////////////

	//	四角形の形状のデータを作る
	b2PolygonShape staticBox;
	//	1m四方の正方形を指定(0.5は一辺の長さの半分）
	staticBox.SetAsBox(9.0f, 0.5f);

	//	ワールドに新たなダイナミック追加する
	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.position.Set(480 / PTM_RATIO, 1260 / PTM_RATIO);
	body2 = m_pWorld->CreateBody(&bodyDef);

	//	ボディにタグの設定
	body2->SetUserData(tag);

	//	スタティックボディに四角形のフィクチャを追加する
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &staticBox;

	body2->CreateFixture(&fixtureDef);

	//	四角形の形状のデータを作る
	b2PolygonShape dynamicfloor;
	//	1m四方の正方形を指定(0.5は一辺の長さの半分）
	dynamicfloor.SetAsBox(9.0f, 0.5f);

	//	床用のスプライトを作成
	m_floor = Sprite::create("don.png");
	m_floor->setScaleX(2.7f);
	m_floor->setScaleY(0.3f);
	this->addChild(m_floor);

	//	ワールドに新たなダイナミックボディ(床）追加する
	b2BodyDef floorBodyDef;
	floorBodyDef.type = b2_dynamicBody;
	floorBodyDef.position.Set(480 / PTM_RATIO, 820 / PTM_RATIO);
	floorBodyDef.userData = m_floor;
	b2Body* floorBody = m_pWorld->CreateBody(&floorBodyDef);

	//	ダイナミックボディに四角形のフィクチャを追加する
	b2FixtureDef floorFixtureDef;
	floorFixtureDef.shape = &dynamicfloor;
	floorFixtureDef.restitution = 0.0f;				//	反発係数の設定
	floorFixtureDef.density = 1.0f;					//	密度の設定
	floorFixtureDef.friction = 1000.0f;
	floorBody->CreateFixture(&floorFixtureDef);

	//	床(ボディ)を保存
	m_floorBody = floorBody;

	//	ディスタンスジョイントの設定
	b2DistanceJointDef jd;
	b2Vec2 anchorA = body2->GetWorldCenter();
	b2Vec2 anchorB = floorBody->GetWorldCenter();
	jd.Initialize(body2, floorBody, anchorA, anchorB);
	jd.collideConnected = true;	 //	AとBが互いに衝突するかどうか
	jd.frequencyHz = 0.4f;		 //	バネの運動周期(1秒で何周するか）
	jd.dampingRatio = 0.7f;		 //	バネの固さ(0～1）
	DrawNode* drawNode = DrawNode::create();
	jd.userData = drawNode;
	this->addChild(drawNode);
	m_pWorld->CreateJoint(&jd);

	
	//	リング５個の設定
	for (int i = 0; i < 5; i++)
	{
		//	真ん中のリングだけ設置しない
		if (i != 2)
		{
			//	座標を引数に渡して作成
			Vec2 vec = { -288.0f + 144.0f * i, 0.0f };
			ringCreate(vec);
		}
	}
	

	////	地面
	//groundBox.Set(leftbottom, rightbottom);
	//groundBody->CreateFixture(&groundBox, 0);

	////　天井
	//groundBox.Set(lefttop, righttop);
	//groundBody->CreateFixture(&groundBox, 0);

	////	左端
	//groundBox.Set(lefttop, leftbottom);
	//groundBody->CreateFixture(&groundBox, 0);

	////	右端
	//groundBox.Set(righttop, rightbottom);
	//groundBody->CreateFixture(&groundBox, 0);

}

//	デストラクタ
PlayScene::~PlayScene()
{
	//	解放
	delete m_pDraw;
	delete m_pWorld;
}

//	ゲームの初期化
bool PlayScene::init()
{
	// 基底クラスの初期化
	if (!Scene::init())
	{
		// 基底クラスの初期化が失敗なら、異常終了
		return false;
	}

	//	乱数をシャッフル
	srand(static_cast < unsigned int > (time(NULL)));

	//	タッチを有効にする
	//	イベントリスナーを作成する
	EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(PlayScene::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(PlayScene::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(PlayScene::onTouchEnded, this);

	//	イベントリスナーを登録する
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	//	カメラを操作できるようにする
	m_pCamera = getDefaultCamera();

	//	カメラを少し引いた位置に移動
	m_pCamera->setPositionX(470.0f);
	m_pCamera->setPositionZ(1200.0f);

	//	タグ番号の初期化
	m_nextTag = 0;	

	//	おジャマを一定間隔で落とす
	schedule(CC_CALLBACK_1(PlayScene::ojamaCreate, this), 2.0f, "ojama");

	//	背景の設定
	Sprite* back = Sprite::create("patapon.png");
	back->setPosition(480.0f, 320.0f);
	back->setScaleX(0.4f);
	back->setScaleY(0.3f);
	this->addChild(back);

	//	物理ワールドの初期化処理
	initPhysics();
	//	アップデートの有効化
	scheduleUpdate();

	//	ゲームオーバーフラグの初期化
	m_OverFlag = false;

	//	初期化成功なのでtrueを返す
	return true;
}

//	更新処理
void PlayScene::update(float dt)
{
	//	物理ワールドの更新(時間を進める）
	m_pWorld->Step(1.0f / 60.0f, 8, 3);

	//	物理ワールド内の全てのボディについてのループ処理
	for (b2Body* body = m_pWorld->GetBodyList();
	body != nullptr;
		body = body->GetNext())
	{
		//	ボディの画像データを取得
		Sprite* box = (Sprite*)body->GetUserData();

		//	ボディの座標を取得
		b2Vec2 pos = body->GetPosition();

		//	ボディがnullptrではない
		if (box != nullptr)
		{
			//	ボディの位置に合わせて画像を動かす処理
			box->setPosition(Vec2(pos.x * PTM_RATIO, pos.y * PTM_RATIO));

			//	物理ワールド内の剛体の角度を取得して、
			float rot = body->GetAngle();
			//	同じ角度にSpriteを回転
			box->setRotation(-CC_RADIANS_TO_DEGREES(rot));
		}
		
		//	ボディの画像が床ならtrue
		if (body->GetUserData() == m_floor)
		{
			//	床の位置が設定の位置以下でtrue
			if (body->GetPosition().y * PTM_RATIO < -400.0f)
			{
				//	ゲームオーバーフラグが立っていたらオーバーシーンに飛ぶ
				//	ゲームオーバーフラグはボディの角度を見て変更される　
				if (m_OverFlag)
				{
					//	シーンの作成
					Scene* nextScene = OverScene::create();

					// 次のシーンに移行
					_director->replaceScene(nextScene);
					break;
				}

				//	シーンの作成
				Scene* nextScene = ClearScene::create();

				// 次のシーンに移行
				_director->replaceScene(nextScene);
			}

			//	ゲームオーバーフラグのために角度を取得する
			float rot = body->GetAngle();
			//	取得した角度はラジアンなので度に変換する
			rot = CC_RADIANS_TO_DEGREES(rot);

			//	設定した角度以上でtrue
			if (rot > 90 || rot < -90)
			{

				//	上の条件を満たしたらジョイントが切れてゲームオーバーのフラグが立つ
				m_OverFlag = true;

			}
		}


		//	ボディが画面外（下）に出たら削除
		if (pos.y < -400.0f / PTM_RATIO)
		{
			//	ボディの削除
			m_pWorld->DestroyBody(body);
			//	画像の削除
			box->removeFromParent();
		}


	}

	//	ジョイントの視覚化のためのDrawNodeを作成
	DrawNode* drawNode = DrawNode::create();
	//drawNode->drawSegment()
	//	線を更新するために一度消す
	drawNode->clear();
	this->addChild(drawNode);

		//物理ワールド内のジョイントについての処理
	for (b2Joint* joint = m_pWorld->GetJointList();
	joint != nullptr;
		joint = joint->GetNext())
	{
		//	ジョイントに覚えさせているジョイント線を取得
		DrawNode* drawNode = (DrawNode*)joint->GetUserData();

		//	ジョイントのA,B座標をvec2に変換
		b2Vec2 p1 = joint->GetAnchorA();
		Vec2 pos1 = { p1.x * PTM_RATIO , p1.y * PTM_RATIO };
		b2Vec2 p2 = joint->GetAnchorB();
		Vec2 pos2 = { p2.x * PTM_RATIO, p2.y * PTM_RATIO };

		//	線があるならtrue
		if (drawNode != nullptr)
		{
			//	線を更新のため一度消す
			drawNode->clear();
			//	青色の線を引く
			drawNode->drawSegment(pos1, pos2, 3.0f, Color4F(0.1f, 0.1f, 1.0f, 1));

		}

		//	メインのジョイントを探す
		if (joint->GetBodyA()->GetUserData() == tag)
		{
			b2Vec2 pos = joint->GetAnchorB();

			//	ジョイントが一定以上ならジョイントの色を変える(黄)
			if (pos.y <= 380.0f / PTM_RATIO)
			{
				//	黄色の線を引く
				drawNode->drawSegment(pos1, pos2, 3.0f, Color4F(1.0f, 1.0f, 0.0f, 1));
			}

			//	ジョイントが一定以上ならジョイントの色を変える(赤)
			if (pos.y <= 320.0f / PTM_RATIO)
			{
				//	赤色の線を引く
				drawNode->drawSegment(pos1, pos2, 3.0f, Color4F(1.0f, 0.0f, 0.0f, 1));
			}

			//	座標の位置が一定を超えたらジョイントを切る
			//	もしくはゲームオーバーフラグが立つ
			if (pos.y <= 280.0f / PTM_RATIO || m_OverFlag)
			{
				//	ジョイントを消す	線は消していないので残る
				m_pWorld->DestroyJoint(joint);

			}
			
		}
	}


	//	ボディがnullptrではない　＆　ボディがぶら下げるオブジェクトの画像
	if (m_body != nullptr && m_box == (Sprite*)m_body->GetUserData())
	{
		//	すべてのぶら下げるオブジェクトをチェックするループ
		for (b2ContactEdge* list = m_body->GetContactList();	//	リストの先頭
		list != nullptr;
			list = list->next)	//	リストを進める
		{
			//	衝突相手の取得
			b2Body* other = list->other;
			//	コンタクトを取得
			b2Contact* contact = list->contact;
			//	ぶら下げるオブジェクトにジョイントが存在しているかどうか
			b2JointEdge* list1 = m_body->GetJointList();
			//	ぶら下げるオブジェクトにジョイントが存在しているかどうかを調べる
			if (list1 == nullptr)
			{
				//	衝突相手が床ならジョイントしない
				if (m_floor != (Sprite*)other->GetUserData())
				{
					//	衝突相手の画像を取得
					Sprite* spr = (Sprite*)other->GetUserData();
					//	画像が入っている場合true
					if (spr != nullptr)
					{
						//	画像についているtagが10の場合true
						if (10 != spr->getTag())
						{
							//	ディスタンスジョイントの設定
							b2DistanceJointDef jd;							//	ジョイントの設定を行うための変数
							b2Vec2 anchorA = m_body->GetWorldCenter();		//	ジョイントにつなげるボディAの座標
							b2Vec2 anchorB = other->GetWorldCenter();		//	ジョイントにつなげるボディBの座標
							jd.Initialize(m_body, other, anchorA, anchorB);	//	ジョイントの初期設定	
							jd.collideConnected = true;						//	AとBが互いに衝突するかどうか
							jd.frequencyHz = 1.0f;							//	バネの運動周期(1秒で何周するか）
							jd.dampingRatio = 1.0f;							//	バネの固さ(0～1）
							DrawNode* drawNode = DrawNode::create();		//	ジョイントの線を作成する
							jd.userData = drawNode;							//	線の情報をジョイントに覚えさせておく
							this->addChild(drawNode);						//	(*'ω'*)
							m_pWorld->CreateJoint(&jd);						//	ジョイントを追加する
						}
					}
				}
			}
		}
	}
}

//	表示関数をオーバーライド
void PlayScene::draw(Renderer * renderer, const Mat4 & transform, uint32_t flags)
{
	//	物理ワールドをデバッグ表示
	m_pWorld->DrawDebugData();
}

//	タッチ開始時の処理
bool PlayScene::onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * unsed_event)
{
	//	タッチ座標を計算して割り出す
	Vec2 p = TouchTo2dWorld(touch->getLocation());

	//	四角形の形状のデータを作る
	b2PolygonShape dynamicBox;
	//	1m四方の正方形を指定(0.5は一辺の長さの半分）
	dynamicBox.SetAsBox(0.5f, 0.5f);

	//	はさむ物のスプライトの作成
	m_box = Sprite::create("monhan1.png");
	m_box->setScale(0.2f);
	this->addChild(m_box);

	//ワールドに新たなダイナミックボディを追加する
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;							//	形状を設定
	bodyDef.position.Set(p.x / PTM_RATIO, p.y / PTM_RATIO);	//	初期位置を設定
	bodyDef.userData = m_box;								//	ぶら下げる画像を覚えておく	
	b2Body *body = m_pWorld->CreateBody(&bodyDef);			//	ボディの設定を適応

	//ダイナミックボディに四角形のフィクチャを追加する
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;		//	形状の設定
	fixtureDef.density = 1.0f;			//	密度の設定
	fixtureDef.restitution = 1;			//	反発係数の設定	
	body->SetActive(false);				//	ぶら下げるものを生成した段階ではボディは衝突しない
	body->CreateFixture(&fixtureDef);	//	ボディに設定したフィクスチャを登録する


	//	最新のボディを保存する
	m_body = body;


	return true;
}

void PlayScene::onTouchMoved(Touch * touch, cocos2d::Event * unsed_event)
{
	//	タッチした座標をカメラから見たスクリーン座標をワールド座標に
	Vec2 p = TouchTo2dWorld(touch->getLocation());

	//	マウスを動かしたときマウスに合わせて座標を更新する
	b2Vec2 b2pos = { p.x / PTM_RATIO, p.y / PTM_RATIO };
	m_body->SetTransform(b2pos, 0.0f);
}

//	タッチ終了時の処理
void PlayScene::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unsed_event)
{
	//	はさむ物のボディをダイナミックボディに変更する
	m_body->SetType(b2_dynamicBody);
	m_body->SetActive(true);
	//	はさむ物のボディの重力を設定
	m_body->SetGravityScale(4.0f);
}

//	お邪魔をランダムな場所に一つ生成する
void PlayScene::ojamaCreate(float delta)
{
	//	お邪魔の種類をランダムにするための変数設定
	int r = rand() % 3;


		//	お邪魔用のスプライトの作成
		m_sprite = Sprite::create("ojamapuyo.png");
		m_sprite->setPosition(Vec2(1000.0f, 1000.0f));
		m_sprite->setScale(0.3f + (0.3 * r));
		m_sprite->setTag(10);
		this->addChild(m_sprite);

		//	円形の形状のデータを作る
		b2CircleShape circle;
		//	1m四方の円形を指定(0.5は一辺の長さの半分）
		circle.m_radius = 0.5f + (0.5 * r);

		//	ワールドに新たなダイナミックボディ追加する
		b2BodyDef ojamaDef;
		ojamaDef.type = b2_dynamicBody;
		ojamaDef.position.Set((rand() % 440 + 100) / PTM_RATIO, 1100.0f / PTM_RATIO);
		ojamaDef.userData = m_sprite;
		b2Body* ojamaBody = m_pWorld->CreateBody(&ojamaDef);
		ojamaBody->SetGravityScale(2.0f);				//	お邪魔の重力を設定

		//	ダイナミックボディに円形のフィクチャを追加する
		b2FixtureDef ojamaFixtureDef;
		ojamaFixtureDef.shape = &circle;
		ojamaFixtureDef.restitution = 0.0f; //0.5f;
		ojamaFixtureDef.density = 0.5f + (0.5 * r);
		ojamaFixtureDef.friction = 10.0f;
		ojamaBody->CreateFixture(&ojamaFixtureDef);

}

//	リングを作成する
void PlayScene::ringCreate(Vec2 v)
{
	//	引数を基準に位置をずらす
	b2Vec2 b2vec2;
	b2vec2.x = v.x / PTM_RATIO;
	b2vec2.y = (v.y - 16.0f) / PTM_RATIO;

	//	リングの画像の設定
	m_ring = Sprite::create("ball.png");
	m_ring->setScale(0.2f);
	this->addChild(m_ring);

	//	円形の形状のデータを作る
	b2CircleShape dynamicRing;
	//	1m四方の正方形を指定(0.5は一辺の長さの半分）
	dynamicRing.m_radius = 1.5f;

	//	ワールドに新たなダイナミックボディ(リング）追加する
	b2BodyDef ringBodyDef;
	ringBodyDef.type = b2_dynamicBody;
	ringBodyDef.userData = m_ring;
	b2Body* ringBody = m_pWorld->CreateBody(&ringBodyDef);
	ringBody->SetGravityScale(2.0f);

	//	ダイナミックボディに四角形のフィクチャを追加する
	b2FixtureDef ringFixtureDef;
	ringFixtureDef.shape = &dynamicRing;
	ringFixtureDef.restitution = 0.5;
	ringFixtureDef.density = 1.0f;
	ringBody->CreateFixture(&ringFixtureDef);

	//	ディスタンスジョイントの設定
	b2DistanceJointDef jdRing;
	b2Vec2 ringAnchorA = m_floorBody->GetWorldCenter();
	b2Vec2 ringAnchorB = ringBody->GetWorldCenter();
	jdRing.Initialize(m_floorBody, ringBody, ringAnchorA + b2vec2, ringAnchorB);
	jdRing.collideConnected = true;	 //	AとBが互いに衝突するかどうか
	jdRing.frequencyHz = 1.0f;		 //	バネの運動周期(1秒で何周するか）
	jdRing.dampingRatio = 1.0f;		 //	バネの固さ(0～1）
	jdRing.length = 0.1f;
	m_pWorld->CreateJoint(&jdRing);

	ringBody->SetTransform(ringAnchorA + b2vec2,0);
}

Vec2 PlayScene::TouchTo2dWorld(Vec2 touch)
{

	//	手前、奥を定義する
	Vec3 nearP(touch.x, touch.y, -1.0f), farP(touch.x, touch.y, 1.0f);

	//	画面のサイズの取得
	auto size = Director::getInstance()->getWinSize();
	//	3D座標空間に変換
	nearP = m_pCamera->unproject(nearP);
	farP = m_pCamera->unproject(farP);
	//	手前から奥に向かうベクトル
	Vec3 dir(farP - nearP);
	//	カメラから2Dワールドへの距離
	float dist = 0.0f;
	//	2Dワールドへの法線をもとに手前から奥の距離を求める
	float ndd = Vec3::dot(Vec3(0, 0, 1), dir);
	//	nddが平行かどうか
	if (ndd == 0)
		dist = 0.0f;
	//	手前と2Dワールドの距離
	float ndo = Vec3::dot(Vec3(0, 0, 1), nearP);
	//	2Dワールドへの距離の割合を求める
	dist = (0 - ndo) / ndd;
	//	割合をもとに2Dワールドへのベクトルを算出
	Vec3 pos = nearP + dist *  dir;

	//Mat4 nodeToWorld = m_pCamera->getNodeToWorldTransform();
	//Vec3 vec3(pos.x, pos.y, 0);
	//vec3 = m_pCamera->unproject(vec3);
	//Vec3 ret;
	//nodeToWorld.transformPoint(vec3, &ret);

	//	求めた座標をvec2にする
	Vec2 p = Vec2(pos.x, _director->getWinSize().height - pos.y);
	return p;
}