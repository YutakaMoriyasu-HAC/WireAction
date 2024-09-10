#include "PlayerBodyAttackState.h"
#include "app/Namelist/PlayerStateList.h"
#include "app/Worlds/World.h"
#include "app/time/Time.h"
#include "app/Math/Math.h"
#include "app/Input/InputManager.h"
#include "app/Namelist/PlayerStateList.h"
#include "app/NameList/Assets.h"



// �R���X�g���N�^
PlayerBodyAttackState::PlayerBodyAttackState(Player* parent, IWorld* world, app::StateMachine* stateMachine) :
	StateBase(stateMachine), parent_(parent), world_(world)
{

}
// ������
void PlayerBodyAttackState::init()
{

	//���x�p��
	velocity_ = parent_->velocity();
	//�J�������W
	cameraLookPoint_ = parent_->getCameraLookPoint();
}

// �I��
void PlayerBodyAttackState::final()
{
}
// �X�V
void PlayerBodyAttackState::update()
{
	//�܂����R����
	parent_->gravityFall(1.0f);

	//���x�p������
	velocity_ = parent_->velocity();

	cameraLookPoint_.x = parent_->GetPosition().x + velocity_.x;
	cameraLookPoint_.z = parent_->GetPosition().z + velocity_.z;


	//�v���C���[���W�����v�O�̍��W�������ɗ����Ă��璍���_��������
	if (parent_->GetPosition().y < cameraLookPoint_.y) {
		cameraLookPoint_.y = parent_->GetPosition().y;
	}

	//�����_�ݒ�
	parent_->setCameraLookPoint(cameraLookPoint_);




	//���n������I��
	if (parent_->isGround()) {
		
		if (parent_->input_ != GSvector3::zero()) {
			parent_->ChangeMotionS(Motion_Rolling, false,3.0f,0.5f,10.0f); //���[�V�����ύX
		}
		else {
			parent_->ChangeMotionS(Motion_Idle, true); //���[�V�����ύX
		}
		parent_->changeState(PlayerStateList::State_Walk);

		return;
	}





}

void PlayerBodyAttackState::lateUpdate()
{

}
// �`��
void PlayerBodyAttackState::draw() const
{
	

}

void PlayerBodyAttackState::lateDraw() const
{
}
