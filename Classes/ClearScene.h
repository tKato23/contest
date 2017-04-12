//	多重インクルードの防止
#pragma once

//	ヘッダファイルのインクルード
#include "cocos2d.h"
#include "ui/CocosGUI.h"

// タイトルシーン
class ClearScene : public cocos2d::Scene
{
public:
	// create関数の宣言と定義
	//CREATE_FUNC(ClearScene);
	static ClearScene* create();

	bool init();							//	初期化処理

	void update(float delta) override;		//	更新処理

	cocos2d::Sprite* back;					//	背景画像

	int time;

};