//	ヘッダファイルのインクルード
#include "OverScene.h"
#include "PlayScene.h"

USING_NS_CC;

//	create関数の定義
OverScene* OverScene::create()
{
	//	メモリの確保
	OverScene *pRet = new(std::nothrow) OverScene();
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

// 初期化
bool OverScene::init()
{
	// 基底クラスの初期化
	if (!Scene::init())
	{
		// 基底クラスの初期化が失敗なら、異常終了
		return false;
	}

	back = Sprite::create("GameOver.png");
	back->setPosition(320.0f, 480.0f);
	this->addChild(back);

	//	タイムカウントの初期化
	time = 0;

	// 毎フレーム更新を有効化
	scheduleUpdate();

	return true;
}

// 毎フレーム更新
void OverScene::update(float delta)
{
	//	カウントを1増やす
	time++;

	//	カウントが250以上になったらシーンを切り替える
	if (time > 250)
	{
		// 次のシーンを作成する
		Scene* nextScene = PlayScene::create();

		//// 次のシーンに移行
		_director->replaceScene(nextScene);
	}
}