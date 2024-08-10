#ifndef CAMERA_TPS_H_
#define CAMERA_TPS_H_

#include "app/Actors/Actor/Actor.h"

//�O�l�̃J�����N���X
class CameraTPS : public Actor{
public:
	//�R���X�g���N�^
	CameraTPS(IWorld* world, const GSvector3& position, const GSvector3& at);
	//�X�V
	virtual void update(float delta_time) override;
	//�`��
	virtual void draw() const override;
};

#endif