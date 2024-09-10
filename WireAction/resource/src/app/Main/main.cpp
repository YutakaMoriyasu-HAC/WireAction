#include<GSgame.h>
#include"app/Scenes/SceneManager/SceneManager.h"
#include "app/Scenes/GamePlayScene/GamePlayScene.h"
#include "app/screen/ScreenSize.h"
#include"app/Input/InputManager.h"
#include "app/Tween/Tween.h"

class MyGame : public gslib::Game {

public:

	//�V�[���}�l�[�W���[
	SceneManager scene_manager_;

#ifdef _DEBUG
	// �R���X�g���N�^
	MyGame() : gslib::Game{ app::ScreenSize::width,app::ScreenSize::height
	}{
		isEnd_ = false;
	}
#else
	// �R���X�g���N�^
	MyGame() : gslib::Game{ app::ScreenSize::width,app::ScreenSize::height , true } {
		isEnd_ = false;
	}

#endif

	//�J�n
	void start() override {
		//�Q�[���v���C�V�[���̒ǉ�
		scene_manager_.add("GamePlayScene", new GamePlayScene());
		//�^�C�g���V�[������J�n
		scene_manager_.change("GamePlayScene");
	}

	//�X�V
	void update(float delta_time) override {
		//���[���h�N���X�̍X�V
		scene_manager_.update(delta_time);
		// Tween�̍X�V
		Tween::update(delta_time);

		// �I��
		if (InputManager::IsGameEndState())
		{
			isEnd_ = true;
		}
	}

	//�`��
	void draw() override {
		//���[���h�N���X�̕`��
		scene_manager_.draw();
	}

	//�I��
	void end() override {
		//�V�[���̏I��
		scene_manager_.end();
		//�V�[���̏���
		scene_manager_.clear();
	}

	bool is_running() override {
		return !isEnd_;
	}


private:
	bool isEnd_;

};

//���C���֐�
int main() {
	return MyGame().run();
}