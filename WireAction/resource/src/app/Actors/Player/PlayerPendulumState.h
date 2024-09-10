#ifndef PLAYER_PENDULUM_STATE_H_
#define PLAYER_PENDULUM_STATE_H_

#include <memory>

#include "app/StateMachine/StateBase.h"
#include "app/Actors/Player/Player.h"

// �v���C���[�̈ړ������N���X
class PlayerPendulumState : public app::StateBase
{
public:
	// �R���X�g���N�^
	PlayerPendulumState(Player* parent, IWorld* world, app::StateMachine* stateMachine);
	// �f�X�g���N�^
	~PlayerPendulumState() = default;
	// ������
	void init() override;
	// �I��
	void final() override;
	// �X�V
	void update() override;
	void lateUpdate()override;
	// �`��
	void draw() const override;
	void lateDraw()const override;



private:
	void changeAngle();



private:
	Player* parent_;
	IWorld* world_;
	GSvector3 position_;





	GSvector3 my_Input_Direction_{ 0,0,0 };


};


#endif