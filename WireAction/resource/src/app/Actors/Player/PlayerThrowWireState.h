#ifndef PLAYER_THROW_WIRE_STATE_H_
#define PLAYER_THROW_WIRE_STATE_H_

#include <memory>

#include "app/StateMachine/StateBase.h"
#include "app/Actors/Player/Player.h"

// �v���C���[�̈ړ������N���X
class PlayerThrowWireState : public app::StateBase
{
public:
	// �R���X�g���N�^
	PlayerThrowWireState(Player* parent, IWorld* world, app::StateMachine* stateMachine);
	// �f�X�g���N�^
	~PlayerThrowWireState() = default;
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


	//�J�����̒����_(�W�����v�ŕς�邽�ߕϐ��ɂ��Ă���)
	GSvector3 cameraLookPoint_{ 0,0,0 };


	GSvector3 my_Input_Direction_{ 0,0,0 };

	//���x
	GSvector3 velocity_{ 0.0f,0.0f,0.0f };


};


#endif