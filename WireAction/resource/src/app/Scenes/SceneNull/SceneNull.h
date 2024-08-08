#ifndef SCENE_NULL_H_
#define SCENE_MULL_H_

#include "../IScene/IScene.h"

//�k���V�[��
class SceneNull : public IScene {
public:
	//�J�n
	virtual void start() override {}
	//�X�V
	virtual void update(float delta_time) override {}
	//�`��
	virtual void draw() override {}
	//���̃V�[����Ԃ�
	virtual std::string next() const override {
		return "sceneNull";
	}
	//�I��
	virtual void end() override {}
};

#endif