#include<GSgame.h>
#include"app/Scenes/SceneManager/SceneManager.h"
#include "app/Scenes/GamePlayScene/GamePlayScene.h"

class MyGame : public gslib::Game {
	//�V�[���}�l�[�W���[
	SceneManager scene_manager_;

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
};

//���C���֐�
int main() {
	return MyGame().run();
}