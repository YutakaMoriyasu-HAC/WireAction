#include "PlayerDamageState.h"
#include "app/Namelist/PlayerStateList.h"
#include "app/Worlds/World.h"
#include "app/Input/InputManager.h"

const float ACCELERATION{ 0.01f };


// コンストラクタ
PlayerDamageState::PlayerDamageState(Player* parent, IWorld* world, app::StateMachine* stateMachine) :
	StateBase(stateMachine), parent_(parent), world_(world)
{
	moveSpeed_ = 0;
}
// 初期化
void PlayerDamageState::init()
{

	my_Input_Direction_ = parent_->GetInputDirection();
	parent_->ChangeMotionS(Motion_Damage, false); //モーション変更

	//カメラ座標
	cameraLookPoint_ = parent_->getCameraLookPoint();

	//速度継承
	velocity_ = my_Input_Direction_ * -0.1f;
	velocity_.y = 0;



}
// 終了
void PlayerDamageState::final()
{
}
// 更新
void PlayerDamageState::update()
{
	//まず自然落下
	parent_->gravityFall(1.0f);

	
	

	




	


	

	// モーションの変更
	// 移動量のxy成分だけ更新
	parent_->velocity().x = velocity_.x;
	parent_->velocity().z = velocity_.z;

	cameraLookPoint_ = parent_->GetPosition();


	//注視点設定
	parent_->setCameraLookPoint(cameraLookPoint_);

	//アニメ終了でおわり
	if (parent_->IsMotionEnd()) {
		parent_->changeState(PlayerStateList::State_Walk);
		parent_->ChangeMotionS(Motion_Idle, true); //モーション変更

		return;
	}

	
}
void PlayerDamageState::lateUpdate()
{
}
// 描画
void PlayerDamageState::draw() const
{
}
void PlayerDamageState::lateDraw() const
{
}
