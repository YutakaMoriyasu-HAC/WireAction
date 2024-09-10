#include "PlayerQuickWireState.h"
#include "app/Namelist/PlayerStateList.h"
#include "app/Worlds/World.h"
#include "app/time/Time.h"
#include "app/Math/Math.h"
#include "app/Input/InputManager.h"
#include "app/Namelist/PlayerStateList.h"
#include "app/NameList/Assets.h"



// �R���X�g���N�^
PlayerQuickWireState::PlayerQuickWireState(Player* parent, IWorld* world, app::StateMachine* stateMachine) :
	StateBase(stateMachine), parent_(parent), world_(world)
{

}
// ������
void PlayerQuickWireState::init()
{
	wirePosition_ = parent_->getCenterPendulum();
	time_ = 10.0f;
	timer_ = time_;
}

// �I��
void PlayerQuickWireState::final()
{
}
// �X�V
void PlayerQuickWireState::update()
{
	parent_->Player::move_to(wirePosition_, time_).name("wire2");

	timer_ -= 1.0f;
	if (timer_ <= -10 || parent_->isWall()) {
		Tween::cancel("wire2");
		parent_->changeState(PlayerStateList::State_Jump);
	}

	//�����_�ݒ�
	parent_->setCameraLookPoint(parent_->transform().position());

}

void PlayerQuickWireState::lateUpdate()
{

}
// �`��
void PlayerQuickWireState::draw() const
{
}

void PlayerQuickWireState::lateDraw() const
{
}
