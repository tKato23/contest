//	ヘッダファイルのインクルード
#include "ClearScene.h"

USING_NS_CC;

//	create関数の定義
ClearScene* ClearScene::create()
{
	//	メモリの確保
	ClearScene *pRet = new(std::nothrow) ClearScene();
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
bool ClearScene::init()
{
	// 基底クラスの初期化
	if (!Scene::init())
	{
		// 基底クラスの初期化が失敗なら、異常終了
		return false;
	}

}