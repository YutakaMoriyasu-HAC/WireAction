#ifndef CAMERA_ROTATE_AROUND_H_
#define CAMERA_ROTATE_AROUND_H_

#include "app/Actors/Actor/Actor.h"

//�O�l�̃J�����N���X
class CameraRotateAround : public Actor {
public:
	//�R���X�g���N�^
	CameraRotateAround(IWorld* world, const GSvector3& position, const GSvector3& at);
	//�X�V
	virtual void update(float delta_time) override;
	//�`��
	virtual void draw() const override;

private:
	//y������̉�]�p�x
	float yaw_{ 0.0f };
	//x������̉�]�p�x
	float pitch_{ 0.0f };
};

#endif