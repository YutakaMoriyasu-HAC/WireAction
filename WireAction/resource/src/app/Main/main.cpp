#include <GSgame.h>
#include "app/Scenes/SceneManager/SceneManager.h"
#include "app/Scenes/GamePlayScene/GamePlayScene.h"


//マイゲームクラス
class MyGame : public gslib::Game {
public:
	//コンストラクタ
	MyGame() : gslib::Game{}{

	}
	//開始
	void start() override {
		scene_manager_.add("GamePlayScene",new GamePlayScene());
		scene_manager_.change("GamePlayScene");
	}
	//更新
	void update(float delta_time) {
		scene_manager_.update(delta_time);
	}
	//描画
	void draw() override {
		scene_manager_.draw();
	}
	//終了
	void end() override {
		scene_manager_.end();
		scene_manager_.clear();
	}

private:
	SceneManager scene_manager_;
};

//main関数
int main() {
	return MyGame().run();
}