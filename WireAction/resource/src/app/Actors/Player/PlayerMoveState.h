#ifndef PLAYER_MOVE_STATE_H_
#define PLAYER_MOVE_STATE_H_

#include <memory>

#include "app/StateMachine/StateBase.h"
#include "app/Actors/Player/Player.h"

// �v���C���[�̈ړ������N���X
class PlayerMoveState : public app::StateBase
{
public:
	// �R���X�g���N�^
	PlayerMoveState(Player* parent, IWorld* world, app::StateMachine* stateMachine);
	// �f�X�g���N�^
	~PlayerMoveState() = default;
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
	bool isDash_;
private:
	Player* parent_;
	IWorld* world_;
	GSvector3 position_;

	//�����x
	float moveSpeed_{ 0.5f };

	//���x
	GSvector3 velocity_{0.0f,0.0f,0.0f};

	//���̃X�e�[�g���n�܂������̃X�s�\�h�A�ʏ�SSS
	float stateStartSpeed{ 0.0f };

	enum SpeedState {
		Stop,
		SpeedUp,
		SpeedDown
	};

	SpeedState SState_{ Stop };

	GSvector3 my_Input_Direction_{ 0,0,0 };

	//�J�����̒����_(�W�����v�ŕς�邽�ߕϐ��ɂ��Ă���)
	GSvector3 cameraLookPoint_{ 0,0,0 };
};


#endif