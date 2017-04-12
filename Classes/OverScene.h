//	多重インクルードの防止
#pragma once

//	ヘッダファイルのインクルード
#include "cocos2d.h"
#include "ui/CocosGUI.h"

// オーバーシーン
class OverScene : public cocos2d::Scene
{
public:
	// create関数の宣言と定義
	//CREATE_FUNC(OverScene);
	static OverScene* create();

	//	初期化
	bool init();

	//	更新処理
	void update(float delta) override;

	cocos2d::Sprite* back;		//	背景画像

	int time;					//	シーン切り替え用カウント変数
};
