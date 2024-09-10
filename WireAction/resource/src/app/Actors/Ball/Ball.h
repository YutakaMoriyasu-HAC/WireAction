#ifndef BALL_H_
#define BALL_H_

#include "app/Actors/Actor/Actor.h"
#include "app/AnimatedMesh/AnimatedMesh.h"

class Ball :public Actor
{
public:
	//�R���X�g���N�^
	Ball(IWorld* world, const GSvector3& position);

	virtual void update(float delta_time) override;
	//�`��
	virtual void draw() const override;

private:
	//�A�j���[�V�������b�V��
	AnimatedMesh	mesh_;

};

#endif 

