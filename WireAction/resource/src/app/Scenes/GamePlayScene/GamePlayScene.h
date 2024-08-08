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
	//�J�n
	virtual void start() override;
	//�X�V
	virtual void update(float delta_time) override;
	//�`��
	virtual void draw() override;
	//���̃V�[����Ԃ�
	virtual std::string next() const override;
	//�I��
	virtual void end() override;
private:
	//�Q�[�����̏��̍X�V
	void updatePlaying(float delta_time);
	//���U���g��Ԃ̍X�V
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

	//���U���g�p�^�C�}
	float resultTimer_{ 0.0f };

	float is_option{ false };
	float is_exit{ false };
	float is_tutorial{ false };
	float is_close{ false };

};

#endif