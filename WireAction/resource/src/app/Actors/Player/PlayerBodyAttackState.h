#ifndef PLAYER_BODYATTACK_STATE_H_
#define PLAYER_BODYATTACK_STATE_H_

#include <memory>

#include "app/StateMachine/StateBase.h"
#include "app/Actors/Player/Player.h"

// �v���C���[�̈ړ������N���X
class PlayerBodyAttackState : public app::StateBase
{
public:
	// �R���X�g���N�^
	PlayerBodyAttackState(Player* parent, IWorld* world, app::StateMachine* stateMachine);
	// �f�X�g���N�^
	~PlayerBodyAttackState() = default;
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




private:
	Player* parent_;
	IWorld* world_;
	GSvector3 position_;
	GSvector3 my_Input_Direction_{ 0,0,0 };
	GSvector3 velocity_{ 0.0f,0.0f,0.0f }; //���x

	//�J�����̒����_(�W�����v�ŕς�邽�ߕϐ��ɂ��Ă���)
	GSvector3 cameraLookPoint_{ 0,0,0 };
	
};

#endif