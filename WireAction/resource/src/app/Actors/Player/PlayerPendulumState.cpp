#include "PlayerPendulumState.h"
#include "app/Namelist/PlayerStateList.h"
#include "app/Worlds/World.h"
#include "app/time/Time.h"
#include "app/Math/Math.h"
#include "app/Input/InputManager.h"
#include "app/Namelist/PlayerStateList.h"



// �R���X�g���N�^
PlayerPendulumState::PlayerPendulumState(Player* parent, IWorld* world, app::StateMachine* stateMachine) :
	StateBase(stateMachine), parent_(parent), world_(world)
{

}
// ������
void PlayerPendulumState::init()
{
	


}
// �I��
void PlayerPendulumState::final()
{
}
// �X�V
void PlayerPendulumState::update()
{
	parent_->changeState(PlayerStateList::State_Jump);
	return;




}
void PlayerPendulumState::lateUpdate()
{
}
// �`��
void PlayerPendulumState::draw() const
{
}
void PlayerPendulumState::lateDraw() const
{
}

void PlayerPendulumState::changeAngle() {
	//�����̕��
	GSquaternion rotation =
		GSquaternion::rotateTowards(
			parent_->transform().rotation(),
			GSquaternion::lookRotation(my_Input_Direction_), 12.0f);
	parent_->transform().rotation(rotation);
}


