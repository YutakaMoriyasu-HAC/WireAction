#ifndef CAMERA_TPS_H_
#define CAMERA_TPS_H_

#include "app/Actors/Actor/Actor.h"

class CameraTps :public Actor
{
public:
	//�R���X�g���N�^
	CameraTps(IWorld* world, const GSvector3& position, const GSvector3& at);
	//�X�V
	virtual void update(float delta_time)override;
	//�`��
	virtual void draw()const override;

private:
	GSvector3 playerOffset_{ 0,3,8 };

	GSvector2 cameraAngle_{ 90,30 };
	GSvector2 cameraAngleE_{ 0,0 };

	bool startFlag_{ false };

	int cameraLevel_{ 0 };

	//�O�̃t���[���̒����_
	GSvector3 prevPlayerLookPos_{ 0,3,5 };

	//�␳���������_
	GSvector3 smoothPlayerLookPos_{ 0,0,0 };

	
};

#endif 

