#include<GSgame.h>
#include"app/Scenes/SceneManager/SceneManager.h"
#include "app/Scenes/GamePlayScene/GamePlayScene.h"

class MyGame : public gslib::Game {
	//シーンマネージャー
	SceneManager scene_manager_;

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
};

//メイン関数
int main() {
	return MyGame().run();
}