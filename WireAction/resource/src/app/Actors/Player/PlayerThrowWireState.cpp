#include "PlayerThrowWireState.h"
#include "app/Namelist/PlayerStateList.h"
#include "app/Worlds/World.h"
#include "app/time/Time.h"
#include "app/Math/Math.h"
#include "app/Input/InputManager.h"
#include "app/Namelist/PlayerStateList.h"



// コンストラクタ
PlayerThrowWireState::PlayerThrowWireState(Player* parent, IWorld* world, app::StateMachine* stateMachine) :
	StateBase(stateMachine), parent_(parent), world_(world)
{
	
}
// 初期化
void PlayerThrowWireState::init()
{
	parent_->spone(Actor_WireBeam);
	

}
// 終了
void PlayerThrowWireState::final()
{
}
// 更新
void PlayerThrowWireState::update()
{
	parent_->changeState(PlayerStateList::State_Jump);
	return;

	


}
void PlayerThrowWireState::lateUpdate()
{
}
// 描画
void PlayerThrowWireState::draw() const
{
}
void PlayerThrowWireState::lateDraw() const
{
}

void PlayerThrowWireState::changeAngle() {
	//向きの補間
	GSquaternion rotation =
		GSquaternion::rotateTowards(
			parent_->transform().rotation(),
			GSquaternion::lookRotation(my_Input_Direction_), 12.0f);
	parent_->transform().rotation(rotation);
}
