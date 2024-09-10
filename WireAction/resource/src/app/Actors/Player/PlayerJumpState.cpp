#include "PlayerJumpState.h"
#include "app/Namelist/PlayerStateList.h"
#include "app/Worlds/World.h"
#include "app/time/Time.h"
#include "app/Math/Math.h"
#include "app/Input/InputManager.h"


// �ړ����x
const float MIN_SPEED{ 0.008f };
const float ACCELERATION{ 0.001f };
const float MAX_SPEED{ 0.05f };

// << �v���C���[�̈ړ� >>

// �R���X�g���N�^
PlayerJumpState::PlayerJumpState(Player* parent, IWorld* world, app::StateMachine* stateMachine) :
	StateBase(stateMachine), parent_(parent), world_(world)
{
}
// ������
void PlayerJumpState::init()
{
	my_Input_Direction_ = parent_->GetInput();

	//�J�������W
	cameraLookPoint_ = parent_->getCameraLookPoint();

	//���x�p��
	velocity_ = parent_->velocity();

	//���̃X�e�[�g���n�܂������̑��x�������ŋ��߂Ă���
	//�ʏ�̕��s���������X�s�[�h�̂܂ܕ�����ԂɂȂ������A���̑��x���ێ�����Ƃ�������
	stateStartSpeed = sqrtf((velocity_.x * velocity_.x) + (velocity_.z * velocity_.z));
	stateStartVec = velocity_;

	//�W�����v�񐔃��Z�b�g
	jumpNum_ = 1;


}
// �I��
void PlayerJumpState::final()
{
}
// �X�V
void PlayerJumpState::update()
{
	//�󒆃W�����v
	if (InputManager::IsAButtonTrigger() && jumpNum_ >= 1) {
		parent_->velocity().y = 0.18f;
		cameraLookPoint_.y = (cameraLookPoint_.y+parent_->GetPosition().y)/2;
		jumpNum_ -= 1;
	}

	//���x�p������
	velocity_ = parent_->velocity();
	moveSpeed_ = sqrtf((velocity_.x * velocity_.x) + (velocity_.z * velocity_.z));

	//���n������I��
	if (parent_->isGround()) {
		parent_->changeState(PlayerStateList::State_Walk);
		return;
	}

	

	//���W�擾
	position_ = parent_->GetPosition();

	//�����Ă����
	my_Input_Direction_ = parent_->GetInput();

	//�X�e�B�b�N���͂����������A���݂̃x�N�g������X�e�B�b�N���͕����܂ł̃x�N�g�������߂�
	if (parent_->input_ != GSvector3::zero()) {

		//my_Input_Direction_�̊p�x
		float inputAngle = atanf(my_Input_Direction_.z / my_Input_Direction_.x)*(180/math::PI);
		if (inputAngle > 360)inputAngle -= 360;
		if (inputAngle <0)inputAngle += 360;

		//�x�N�g���p�x
		float velocityAngle = atanf(velocity_.z / velocity_.x) * (180 / math::PI);
		if (velocityAngle > 360)velocityAngle -= 360;
		if (velocityAngle < 0)velocityAngle += 360;



		//�X�e�B�b�N�p�x�����̊p�x���60�x�ȓ��������瑬�x�p��
		if ((ABS(velocityAngle - inputAngle) < 60 || ABS(velocityAngle - inputAngle) >300) && stateStartSpeed!=0) {
			velocityToInputVector_ = (my_Input_Direction_ * stateStartSpeed) - velocity_;
		}
		else {
			velocityToInputVector_ = (my_Input_Direction_ * MAX_SPEED) - velocity_;
		}

		velocity_.x += velocityToInputVector_.x / 15.0f;
		velocity_.z += velocityToInputVector_.z / 15.0f;
	}
	
	


	//���낢��ς����������]������e�ɕԂ�
	parent_->SetInputDirection(my_Input_Direction_);

	// ���[�V�����̕ύX
	// �ړ��ʂ�xy���������X�V
	parent_->velocity().x = velocity_.x;
	parent_->velocity().z = velocity_.z;

	cameraLookPoint_.x = parent_->GetPosition().x + velocity_.x;
	cameraLookPoint_.z = parent_->GetPosition().z + velocity_.z;

	//�ړ�
	parent_->SetPosition(parent_->GetPosition() + velocity_);

	//�v���C���[���W�����v�O�̍��W�������ɗ����Ă��璍���_��������
	if (parent_->GetPosition().y < cameraLookPoint_.y) {
		cameraLookPoint_.y = parent_->GetPosition().y;
	}

	//�����_�ݒ�
	parent_->setCameraLookPoint(cameraLookPoint_);


}
void PlayerJumpState::lateUpdate()
{
}
// �`��
void PlayerJumpState::draw() const
{
}
void PlayerJumpState::lateDraw() const
{
}

void PlayerJumpState::changeAngle() {
	//�����̕��
	GSquaternion rotation =
		GSquaternion::rotateTowards(
			parent_->transform().rotation(),
			GSquaternion::lookRotation(my_Input_Direction_), 12.0f);
	parent_->transform().rotation(rotation);
}
