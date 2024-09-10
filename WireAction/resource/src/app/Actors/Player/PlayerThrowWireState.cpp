#include "PlayerThrowWireState.h"
#include "app/Namelist/PlayerStateList.h"
#include "app/Worlds/World.h"
#include "app/time/Time.h"
#include "app/Math/Math.h"
#include "app/Input/InputManager.h"
#include "app/Namelist/PlayerStateList.h"



// �R���X�g���N�^
PlayerThrowWireState::PlayerThrowWireState(Player* parent, IWorld* world, app::StateMachine* stateMachine) :
	StateBase(stateMachine), parent_(parent), world_(world)
{
	
}
// ������
void PlayerThrowWireState::init()
{
	parent_->spone(Actor_WireBeam);
	

}
// �I��
void PlayerThrowWireState::final()
{
}
// �X�V
void PlayerThrowWireState::update()
{
	parent_->changeState(PlayerStateList::State_Jump);
	return;

	


}
void PlayerThrowWireState::lateUpdate()
{
}
// �`��
void PlayerThrowWireState::draw() const
{
}
void PlayerThrowWireState::lateDraw() const
{
}

void PlayerThrowWireState::changeAngle() {
	//�����̕��
	GSquaternion rotation =
		GSquaternion::rotateTowards(
			parent_->transform().rotation(),
			GSquaternion::lookRotation(my_Input_Direction_), 12.0f);
	parent_->transform().rotation(rotation);
}
