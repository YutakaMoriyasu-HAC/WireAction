#include<GSgame.h>
#include"app/Scenes/SceneManager/SceneManager.h"
#include "app/Scenes/GamePlayScene/GamePlayScene.h"
#include "app/screen/ScreenSize.h"
#include"app/Input/InputManager.h"
#include "app/Tween/Tween.h"

class MyGame : public gslib::Game {

public:

	//シーンマネージャー
	SceneManager scene_manager_;

#ifdef _DEBUG
	// コンストラクタ
	MyGame() : gslib::Game{ app::ScreenSize::width,app::ScreenSize::height
	}{
		isEnd_ = false;
	}
#else
	// コンストラクタ
	MyGame() : gslib::Game{ app::ScreenSize::width,app::ScreenSize::height , true } {
		isEnd_ = false;
	}

#endif

	//開始
	void start() override {
		//ゲームプレイシーンの追加
		scene_manager_.add("GamePlayScene", new GamePlayScene());
		//タイトルシーンから開始
		scene_manager_.change("GamePlayScene");
	}

	//更新
	void update(float delta_time) override {
		//ワールドクラスの更新
		scene_manager_.update(delta_time);
		// Tweenの更新
		Tween::update(delta_time);

		// 終了
		if (InputManager::IsGameEndState())
		{
			isEnd_ = true;
		}
	}

	//描画
	void draw() override {
		//ワールドクラスの描画
		scene_manager_.draw();
	}

	//終了
	void end() override {
		//シーンの終了
		scene_manager_.end();
		//シーンの消去
		scene_manager_.clear();
	}

	bool is_running() override {
		return !isEnd_;
	}


private:
	bool isEnd_;

};

//メイン関数
int main() {
	return MyGame().run();
}