#include "PlayerMoveState.h"
#include "app/Namelist/PlayerStateList.h"
#include "app/Worlds/World.h"
#include "app/time/Time.h"
#include "app/Math/Math.h"
#include "app/Input/InputManager.h"


// �ړ����x
const float MIN_SPEED{ 0.008f };
const float ACCELERATION{ 0.005f };
const float MAX_SPEED{ 0.2f };
const float ACCELERATION2{ 0.1f };
const float MAX_SPEED2{ 0.25f };

// << �v���C���[�̈ړ� >>

// �R���X�g���N�^
PlayerMoveState::PlayerMoveState(Player* parent, IWorld* world, app::StateMachine* stateMachine) :
	StateBase(stateMachine), parent_(parent), world_(world), isDash_(false)
{
	moveSpeed_ = 0;
}
// ������
void PlayerMoveState::init()
{
	
	my_Input_Direction_ = parent_->GetInput();

	//�J�������W
	cameraLookPoint_ = parent_->getCameraLookPoint();

	//���͂�����Ή����A������Ύ~�܂�
	if (parent_->input_ != GSvector3::zero()) {
		SState_ = SpeedUp;
	}
	else {
		SState_ = Stop;
	}

}
// �I��
void PlayerMoveState::final()
{
}
// �X�V
void PlayerMoveState::update()
{
	//�W�����v�{�^���������ꂽ��X�e�[�g�ύX
	if (InputManager::IsAButtonTrigger()) {
		parent_->changeState(PlayerStateList::State_Jump);
		parent_->velocity().y = 0.25f;
		parent_->setCameraLookPoint(cameraLookPoint_);
		return;
	}

	//�n�ʂ��痎���Ă��X�e�[�g�ύX
	if (!parent_->isGround()) {
		parent_->changeState(PlayerStateList::State_Jump);
		parent_->setCameraLookPoint(cameraLookPoint_);
		return;
	}

	//���W�擾
	position_ = parent_->GetPosition();

	//�����Ă����
	my_Input_Direction_ = parent_->GetInput();

	// �L�[�̓��͂���ړ��x�N�g�����擾
	GSvector3 velocity = my_Input_Direction_ * moveSpeed_;
	
	//���x�ɉ����Ă���ɏ�ԕ���
	switch (SState_) {
	case Stop:
		moveSpeed_ = 0;
		//�X�e�B�b�N���|���ꂽ�u��
		if (parent_->input2_ == GSvector3::zero() && parent_->input_ != GSvector3::zero()) {
			moveSpeed_ = MIN_SPEED;
			SState_ = SpeedUp;
			my_Input_Direction_ = parent_->GetInput();
		}
		break;

	case SpeedUp:
		// �ړ����Ă��邩
		if (velocity.length() != 0.0f) {

			changeAngle();
		}

		//���x����
		if (moveSpeed_ + ACCELERATION < MAX_SPEED) {
			moveSpeed_ += ACCELERATION;
		}
		else {
			moveSpeed_ = MAX_SPEED;
		}

		//�X�e�B�b�N�������ꂽ�u��
		if (parent_->input2_ != GSvector3::zero() && parent_->input_ == GSvector3::zero()) {
			//�����J�n
			SState_ = SpeedDown;
			break;
		}

		if (parent_->input2_ != parent_->input_) {
			my_Input_Direction_ = parent_->GetInput();
		}

		break;

	case SpeedDown:
		if (moveSpeed_ - ACCELERATION*4 > 0) {
			moveSpeed_ -= ACCELERATION * 4;
		}
		else {
			moveSpeed_ = 0;
			SState_ = Stop;
		}
		//�X�e�B�b�N���|���ꂽ�u��
		if (parent_->input2_ == GSvector3::zero() && parent_->input_ != GSvector3::zero()) {
			moveSpeed_ = MIN_SPEED;
			SState_ = SpeedUp;
			my_Input_Direction_ = parent_->GetInput();
		}
		break;
	}
	//���낢��ς����������]������e�ɕԂ�
	parent_->SetInputDirection(my_Input_Direction_);

	// ���[�V�����̕ύX
	// �ړ��ʂ�xy���������X�V
	parent_->velocity().x = velocity.x;
	parent_->velocity().z = velocity.z;

	cameraLookPoint_ = parent_->GetPosition() + velocity;

	//�ړ�
	parent_->SetPosition(parent_->GetPosition() + velocity);

	
	//�����_�ݒ�
	parent_->setCameraLookPoint(cameraLookPoint_);


}
void PlayerMoveState::lateUpdate()
{
}
// �`��
void PlayerMoveState::draw() const
{
}
void PlayerMoveState::lateDraw() const
{
}

void PlayerMoveState::changeAngle() {
	//�����̕��
	GSquaternion rotation =
		GSquaternion::rotateTowards(
			parent_->transform().rotation(),
			GSquaternion::lookRotation(my_Input_Direction_), 12.0f);
	parent_->transform().rotation(rotation);
}
