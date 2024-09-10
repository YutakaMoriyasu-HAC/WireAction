#ifndef COIN_H_
#define COIN_H_

#include "app/Actors/Actor/Actor.h"

class Coin :public Actor
{
public:
	//�R���X�g���N�^
	Coin(IWorld* world, const GSvector3& position);

	virtual void update(float delta_time) override;
	//�`��
	virtual void draw() const override;

	//�Փ˃��A�N�V����
	virtual void react(Actor& other) override;

private:

	float timer_{ 0.0f };
	int animPatarn_{ 0 };

};

#endif 

