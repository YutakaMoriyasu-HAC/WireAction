#include <GSgame.h>
#include "app/Scenes/SceneManager/SceneManager.h"
#include "app/Scenes/GamePlayScene/GamePlayScene.h"


//�}�C�Q�[���N���X
class MyGame : public gslib::Game {
public:
	//�R���X�g���N�^
	MyGame() : gslib::Game{}{

	}
	//�J�n
	void start() override {
		scene_manager_.add("GamePlayScene",new GamePlayScene());
		scene_manager_.change("GamePlayScene");
	}
	//�X�V
	void update(float delta_time) {
		scene_manager_.update(delta_time);
	}
	//�`��
	void draw() override {
		scene_manager_.draw();
	}
	//�I��
	void end() override {
		scene_manager_.end();
		scene_manager_.clear();
	}

private:
	SceneManager scene_manager_;
};

//main�֐�
int main() {
	return MyGame().run();
}