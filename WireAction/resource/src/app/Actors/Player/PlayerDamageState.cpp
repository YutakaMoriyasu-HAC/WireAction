#include "PlayerDamageState.h"
#include "app/Namelist/PlayerStateList.h"
#include "app/Worlds/World.h"
#include "app/Input/InputManager.h"

const float ACCELERATION{ 0.01f };


// �R���X�g���N�^
PlayerDamageState::PlayerDamageState(Player* parent, IWorld* world, app::StateMachine* stateMachine) :
	StateBase(stateMachine), parent_(parent), world_(world)
{
	moveSpeed_ = 0;
}
// ������
void PlayerDamageState::init()
{

	my_Input_Direction_ = parent_->GetInputDirection();
	parent_->ChangeMotionS(Motion_Damage, false); //���[�V�����ύX

	//�J�������W
	cameraLookPoint_ = parent_->getCameraLookPoint();

	//���x�p��
	velocity_ = my_Input_Direction_ * -0.1f;
	velocity_.y = 0;



}
// �I��
void PlayerDamageState::final()
{
}
// �X�V
void PlayerDamageState::update()
{
	//�܂����R����
	parent_->gravityFall(1.0f);

	
	

	




	


	

	// ���[�V�����̕ύX
	// �ړ��ʂ�xy���������X�V
	parent_->velocity().x = velocity_.x;
	parent_->velocity().z = velocity_.z;

	cameraLookPoint_ = parent_->GetPosition();


	//�����_�ݒ�
	parent_->setCameraLookPoint(cameraLookPoint_);

	//�A�j���I���ł����
	if (parent_->IsMotionEnd()) {
		parent_->changeState(PlayerStateList::State_Walk);
		parent_->ChangeMotionS(Motion_Idle, true); //���[�V�����ύX

		return;
	}

	
}
void PlayerDamageState::lateUpdate()
{
}
// �`��
void PlayerDamageState::draw() const
{
}
void PlayerDamageState::lateDraw() const
{
}
