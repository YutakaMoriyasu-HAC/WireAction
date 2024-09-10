#ifndef ENEMY_H_
#define ENEMY_H_

#include "app/Actors/Actor/Actor.h"
#include "app/AnimatedMesh/AnimatedMesh.h"

class Enemy :public Actor
{
public:
	//�R���X�g���N�^
	Enemy(IWorld* world, const GSvector3& position);

	virtual void update(float delta_time) override;
	//�`��
	virtual void draw() const override;

	//�Փ˃��A�N�V����
	virtual void react(Actor& other) override;

	void changeAngle(float time);
	void collide_field();

private:
	//�A�j���[�V�������b�V��
	AnimatedMesh	mesh_;
	//���[�V�����ԍ�
	GSuint			motion_;

	//���[�V�����̃��[�v�w��
	bool			motion_loop_;
	float timer_{ 0.0f };

	//�v���C���[�̏ꏊ
	GSvector3 targetPosition_{ 0,0,0 };

	//Actor player_;

	std::shared_ptr<Actor> player_;

	//�����Ă����
	GSvector3 direction_{ 0,0,0 };

	bool isWall_{ false };
	bool isGround_{ false };

};

#endif 

