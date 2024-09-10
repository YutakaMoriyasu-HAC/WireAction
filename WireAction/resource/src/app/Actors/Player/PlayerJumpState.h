#ifndef PLAYER_JUMP_STATE_H_
#define PLAYER_JUMP_STATE_H_

#include <memory>

#include "app/StateMachine/StateBase.h"
#include "app/Actors/Player/Player.h"

// �v���C���[�̈ړ������N���X
class PlayerJumpState : public app::StateBase
{
public:
	// �R���X�g���N�^
	PlayerJumpState(Player* parent, IWorld* world, app::StateMachine* stateMachine);
	// �f�X�g���N�^
	~PlayerJumpState() = default;
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

	//�ړ����x
	float moveSpeed_{ 0.5f };

	//���x
	GSvector3 velocity_{ 0.0f,0.0f,0.0f };

	//���̃X�e�[�g���n�܂������̃X�s�\�h�A�ʏ�SSS
	float stateStartSpeed{ 0.0f };
	//���̃X�e�[�g���n�܂������̃x�N�g��
	GSvector3 stateStartVec{ 0.0f,0.0f,0.0f };

	//���݂̈ړ�����������͕����܂ł̃x�N�g��
	GSvector3 velocityToInputVector_{ 0.0f,0.0f,0.0f };

	int jumpNum_{ 1 }; //�c��W�����v��

	//�W�����v�{�^���𗣂�����true
	bool buttonReleaseFlag_{ false };
	


	GSvector3 my_Input_Direction_{ 0,0,0 };

	//�J�����̒����_(�W�����v�ŕς�邽�ߕϐ��ɂ��Ă���)
	GSvector3 cameraLookPoint_{ 0,0,0 };
};


#endif