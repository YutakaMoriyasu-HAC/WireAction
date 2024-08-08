#include"GamePlayScene.h"

#include"app/NameList/Assets.h"
#include"PlaySceneState.h"

#include "app/Actors/Actor/Actor.h"
#include"PlayingState.h"
#include"PauseState.h"


#include<gslib.h>


const int SE_COUNT = 10; // 同時に鳴らせるSEの数


GamePlayScene::GamePlayScene()
{
	stateMachine_.addState<PlayingState>(stateTag::PlayingState, &stateMachine_, &world_);
	stateMachine_.addState<PauseState>(stateTag::PauseState, &stateMachine_);

}

void GamePlayScene::start()
{
	stateMachine_.reset(stateTag::PlayingState);


}

void GamePlayScene::update(float delta_time)
{

	stateMachine_.update();
	stateMachine_.lateUpdate();


}

void GamePlayScene::draw()
{
	stateMachine_.draw();
	stateMachine_.lateDraw();
}



std::string GamePlayScene::next() const
{
	return "TitleScene";
}

void GamePlayScene::end()
{
	//world_.actorsClear();
	world_.clear();
	clearAssets();



	// シャドウマップを削除
	gsDeleteShadowMap();
}




void GamePlayScene::updatePlaying(float delta_time)
{
	//リザルト中タイマを初期化
	resultTimer_ = 0.0f;
}

void GamePlayScene::updateResult(float delta_time)
{
	// リザルトの更新
	//リザルトシーンのアップデート関数をここに書く
	// リザルト中タイマの更新
	resultTimer_ += delta_time;
	// 最低1秒間はリザルト表示、スペースキーを押したらシーン終了
	if (resultTimer_ >= 60.0f && gsGetKeyTrigger(GKEY_SPACE))
	{
		isEnd_ = true;
	}
}

void GamePlayScene::loadAssets()
{
	
}

void GamePlayScene::clearAssets()
{
	


}

bool GamePlayScene::getOption()
{
	return is_option;
}

void GamePlayScene::setOption(bool option)
{
	is_option = option;
}

bool GamePlayScene::getExit()
{
	return is_exit;
}

void GamePlayScene::setExit(bool exit)
{
	is_exit = exit;
}

bool GamePlayScene::getTutorial()
{
	return is_tutorial;
}

void GamePlayScene::setTutorial(bool tutorial)
{
	is_tutorial = tutorial;
}

bool GamePlayScene::getClose()
{
	return is_close;
}

void GamePlayScene::setClose(bool close)
{
	is_close = close;
}
