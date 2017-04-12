//	多重インクルードの防止
#pragma once

//	ヘッダファイルのインクルード
#include "cocos2d.h"
#include "ui/CocosGUI.h"
//	box2dを使うには必ず必要
#include "Box2D/Box2D.h"
#include "GLES-Render.h"


// プレイシーン
class PlayScene : public cocos2d::Scene
{
public:
	~PlayScene();				//	デストラクタ

	void update(float dt);		//	更新処理
	void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4& transform, uint32_t flags);		//	描画処理

	//	タッチ関係の処理
	bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unsed_event);			
	void onTouchMoved(cocos2d::Touch * touch, cocos2d::Event * unsed_event);
	void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unsed_event);

	void ojamaCreate(float delta);		//	お邪魔を一つ生成する
	void ringCreate(cocos2d::Vec2 v);	//	リングを作成する	

	cocos2d::Vec2 TouchTo2dWorld(cocos2d::Vec2 touchPos);

	// create関数の宣言と定義
	//CREATE_FUNC(PlayScene);
	static PlayScene* create();

	bool init();						//	初期化

	void initPhysics();					//	物理ワールドの初期化

	b2World *m_pWorld;					//	物理ワールドの作成
	GLESDebugDraw *m_pDraw;				//	DrawNodeを保存する変数
	b2Body *body2;						//	床を支えるボディ
	b2Body *m_body;						//	はさむ物のボディ
	b2Body *m_floorBody;				//	床のボディ
	int m_nextTag;						//	タグを保存

	cocos2d::Sprite* m_box;				//	はさむ物用のスプライト	
	cocos2d::Sprite* m_floor;			//	床用のスプライト
	cocos2d::Sprite* m_ring;			//	リング用のスプライト
	cocos2d::Sprite* tag;				//	判定用スプライト
	cocos2d::Sprite* m_sprite;			//	お邪魔用のスプライト

	cocos2d::Camera* m_pCamera;			//	プレイシーンのカメラ

	bool m_OverFlag;					//	trueでゲームオーバーにいく
};
