#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"

// �^�C�g���V�[��
class ClearScene : public cocos2d::Scene
{
public:
	// create�֐��̐錾�ƒ�`
	//CREATE_FUNC(ClearScene);
	static ClearScene* create();

	bool init();

	void update(float delta) override;

};
