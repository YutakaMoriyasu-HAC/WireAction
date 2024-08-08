#ifndef SCENE_GAMEPLAYSCENE_GAMEPLAYSCENE_H_
#define SCENE_GAMEPLAYSCENE_GAMEPLAYSCENE_H_

#include "app/Scenes/IScene/IScene.h"
#include "app/Worlds/World.h"
#include "app/StateMachine/StateMachine.h"
#include <gslib.h>

class GamePlayScene : public IScene
{
public:
	GamePlayScene();
	//開始
	virtual void start() override;
	//更新
	virtual void update(float delta_time) override;
	//描画
	virtual void draw() override;
	//次のシーンを返す
	virtual std::string next() const override;
	//終了
	virtual void end() override;
private:
	//ゲーム内の情報の更新
	void updatePlaying(float delta_time);
	//リザルト状態の更新
	void updateResult(float delta_time);
private:
	void loadAssets() override;
	void clearAssets();
public:
	bool getOption();
	void setOption(bool option);

	bool getExit();
	void setExit(bool exit);

	bool getTutorial();
	void setTutorial(bool tutorial);

	bool getClose();
	void setClose(bool close);


private:
	World world_;
	app::StateMachine stateMachine_;

	//リザルト用タイマ
	float resultTimer_{ 0.0f };

	float is_option{ false };
	float is_exit{ false };
	float is_tutorial{ false };
	float is_close{ false };

};

#endif