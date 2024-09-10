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
	parent_->ChangeMotionS(Motion_Idle, true); //���[�V�����ύX

	//�J�������W
	cameraLookPoint_ = parent_->getCameraLookPoint();

	//���x�p��
	velocity_= parent_->velocity();

	//���̃X�e�[�g���n�܂������̑��x�������ŋ��߂Ă���
	//�ʏ�̕��s���������X�s�[�h�̂܂ܕ�����ԂɂȂ������A���̑��x���ێ�����Ƃ�������
	stateStartSpeed = sqrtf((velocity_.x * velocity_.x) + (velocity_.z * velocity_.z));

	//���͂�����Ή����A������Ύ~�܂�
	if (parent_->input_ != GSvector3::zero()) {
		SState_ = SpeedUp;
	}
	else {
		SState_ = SpeedDown;
	}

}
// �I��
void PlayerMoveState::final()
{
}
// �X�V
void PlayerMoveState::update()
{
	//���x�p������
	velocity_ = parent_->velocity();
	moveSpeed_ = sqrtf((velocity_.x * velocity_.x) + (velocity_.z * velocity_.z));

	//�W�����v�{�^���������ꂽ��X�e�[�g�ύX
	if (InputManager::IsAButtonTrigger()) {
		parent_->changeState(PlayerStateList::State_Jump);
		parent_->velocity().y = 0.18f;
		parent_->setCameraLookPoint(cameraLookPoint_);
		parent_->ChangeMotionS(Motion_Jump, false); //���[�V�����ύX
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
	//�����Ă�����擾
	my_Input_Direction_ = parent_->GetInput();

	//�ړ����x�̌v�Z
	//���x�ɉ����Ă���ɏ�ԕ�������
	switch (SState_) {
	case Stop:
		moveSpeed_ = 0;
		
		//�X�e�B�b�N���|���ꂽ�u��
		if (parent_->input2_ == GSvector3::zero() && parent_->input_ != GSvector3::zero()) {
			moveSpeed_ = MIN_SPEED;
			SState_ = SpeedUp;
			parent_->ChangeMotionS(Motion_Dash, true, 1.1f); //���[�V�����ύX
			my_Input_Direction_ = parent_->GetInput();
		}
		break;

	case SpeedUp:
		// �ړ����Ă��邩
		if (velocity_.length() != 0.0f) {

			changeAngle();
		}

		//���x����
		if (stateStartSpeed > MAX_SPEED) {
			moveSpeed_ = stateStartSpeed;
		}
		else if (moveSpeed_ + ACCELERATION < MAX_SPEED) {
			moveSpeed_ += ACCELERATION;
		}
		else {
			moveSpeed_ = MAX_SPEED;
		}
		//�����A�j�����ς���ĂȂ�������ύX
		if (parent_->GetMotionState() != Motion_Dash) {
			parent_->ChangeMotionS(Motion_Dash, true, 1.1f); //���[�V�����ύX
		}

		//�X�e�B�b�N�������ꂽ�u��
		if (parent_->input2_ != GSvector3::zero() && parent_->input_ == GSvector3::zero()) {
			//�����J�n
			SState_ = SpeedDown;
			parent_->ChangeMotionS(Motion_Walk, true); //���[�V�����ύX
			break;
		}

		if (parent_->input2_ != parent_->input_) {
			my_Input_Direction_ = parent_->GetInput();
		}

		break;

	case SpeedDown:
		if (moveSpeed_ - ACCELERATION*4 > 0) {
			moveSpeed_ -= ACCELERATION * 2;
		}
		else {
			moveSpeed_ = 0;
			SState_ = Stop;
			parent_->ChangeMotionS(Motion_Idle, true); //���[�V�����ύX
		}
		//�X�e�B�b�N���|���ꂽ�u��
		if (parent_->input2_ == GSvector3::zero() && parent_->input_ != GSvector3::zero()) {
			moveSpeed_ = MIN_SPEED;
			SState_ = SpeedUp;
			my_Input_Direction_ = parent_->GetInput();
			parent_->ChangeMotionS(Motion_Dash, true, 1.1f); //���[�V�����ύX
		}
		break;
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

	//�ړ�
	parent_->SetPosition(parent_->GetPosition() + velocity_);

	
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
