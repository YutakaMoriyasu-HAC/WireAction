#include "PlayerCrouchState.h"
#include "app/Namelist/PlayerStateList.h"
#include "app/Worlds/World.h"
#include "app/Input/InputManager.h"

const float ACCELERATION{ 0.01f };


// �R���X�g���N�^
PlayerCrouchState::PlayerCrouchState(Player* parent, IWorld* world, app::StateMachine* stateMachine) :
	StateBase(stateMachine), parent_(parent), world_(world)
{
	moveSpeed_ = 0;
}
// ������
void PlayerCrouchState::init()
{

	my_Input_Direction_ = parent_->GetInputDirection();
	parent_->ChangeMotionS(Motion_Crouch, true); //���[�V�����ύX

	//�J�������W
	cameraLookPoint_ = parent_->getCameraLookPoint();

	//���x�p��
	velocity_ = parent_->velocity();

	

}
// �I��
void PlayerCrouchState::final()
{
}
// �X�V
void PlayerCrouchState::update()
{
	//�܂����R����
	parent_->gravityFall(1.0f);

	//���x�p������
	velocity_ = parent_->velocity();
	moveSpeed_ = sqrtf((velocity_.x * velocity_.x) + (velocity_.z * velocity_.z));

	//���Ⴊ�݂������ꂽ�炨���
	if (!InputManager::IsCrouchState()) {
		parent_->changeState(PlayerStateList::State_Walk);
		parent_->ChangeMotionS(Motion_Idle, false); //���[�V�����ύX
		parent_->setCameraLookPoint(cameraLookPoint_);
		return;
	}

	//�n�ʂ��痎���Ă��X�e�[�g�ύX
	if (!parent_->isGround()) {
		parent_->changeState(PlayerStateList::State_Jump);
		parent_->setCameraLookPoint(cameraLookPoint_);
		return;
	}

	//���Ⴊ�݃W�����v
	if (InputManager::IsAButtonTrigger()) {
		cameraLookPoint_ = parent_->GetPosition() + velocity_;
		parent_->velocity(GSvector3(my_Input_Direction_.x * -0.05f, 0.40f, my_Input_Direction_.z * -0.05f));
		parent_->changeState(PlayerStateList::State_Jump);
		parent_->setCameraLookPoint(cameraLookPoint_);
		parent_->ChangeMotionS(Motion_BackRolling, false); //���[�V�����ύX
		return;
	}

	

	//���W�擾
	position_ = parent_->GetPosition();

	//�X�s�[�h
	if (moveSpeed_ - ACCELERATION > 0) {
		moveSpeed_ -= ACCELERATION;
	}
	else {
		moveSpeed_ = 0;
	}

	


	//�����Ă�����ɑ��x�������ĉ����x�ɂ���
	velocity_ = my_Input_Direction_ * moveSpeed_;


	//���낢��ς����������]������e�ɕԂ�
	parent_->SetInputDirection(my_Input_Direction_);

	// ���[�V�����̕ύX
	// �ړ��ʂ�xy���������X�V
	parent_->velocity().x = velocity_.x;
	parent_->velocity().z = velocity_.z;

	cameraLookPoint_ = parent_->GetPosition() + velocity_;


	//�����_�ݒ�
	parent_->setCameraLookPoint(cameraLookPoint_);

	parent_->setDebugMoveSpeed(moveSpeed_);
}
void PlayerCrouchState::lateUpdate()
{
}
// �`��
void PlayerCrouchState::draw() const
{
}
void PlayerCrouchState::lateDraw() const
{
}
