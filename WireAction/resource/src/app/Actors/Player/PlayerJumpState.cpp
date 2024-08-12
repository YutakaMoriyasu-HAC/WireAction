#include "PlayerJumpState.h"
#include "app/Namelist/PlayerStateList.h"
#include "app/Worlds/World.h"
#include "app/time/Time.h"
#include "app/Math/Math.h"
#include "app/Input/InputManager.h"


// �ړ����x
const float MIN_SPEED{ 0.005f };
const float ACCELERATION{ 0.003f };
const float MAX_SPEED{ 0.15f };
const float ACCELERATION2{ 0.1f };
const float MAX_SPEED2{ 0.25f };

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

}
// �I��
void PlayerJumpState::final()
{
}
// �X�V
void PlayerJumpState::update()
{
	if (parent_->isGround()) {
		parent_->changeState(PlayerStateList::State_Walk);
	}

	//���W�擾
	position_ = parent_->GetPosition();

	//�����Ă����
	my_Input_Direction_ = parent_->GetInput();

	// �L�[�̓��͂���ړ��x�N�g�����擾
	GSvector3 velocity = parent_->velocity();

	
	//���낢��ς����������]������e�ɕԂ�
	parent_->SetInputDirection(my_Input_Direction_);

	// ���[�V�����̕ύX
	// �ړ��ʂ�xy���������X�V
	parent_->velocity().x = velocity.x;
	parent_->velocity().z = velocity.z;

	cameraLookPoint_.x = parent_->GetPosition().x + velocity.x;
	cameraLookPoint_.z = parent_->GetPosition().z + velocity.z;

	//�ړ�
	parent_->SetPosition(parent_->GetPosition() + velocity);

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
