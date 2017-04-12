#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"

// タイトルシーン
class ClearScene : public cocos2d::Scene
{
public:
	// create関数の宣言と定義
	//CREATE_FUNC(ClearScene);
	static ClearScene* create();

	bool init();

	void update(float delta) override;

};
