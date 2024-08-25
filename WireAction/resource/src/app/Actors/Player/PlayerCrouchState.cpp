#include "PlayerCrouchState.h"
#include "app/Namelist/PlayerStateList.h"
#include "app/Worlds/World.h"
#include "app/Input/InputManager.h"

const float ACCELERATION{ 0.01f };


// コンストラクタ
PlayerCrouchState::PlayerCrouchState(Player* parent, IWorld* world, app::StateMachine* stateMachine) :
	StateBase(stateMachine), parent_(parent), world_(world)
{
	moveSpeed_ = 0;
}
// 初期化
void PlayerCrouchState::init()
{

	my_Input_Direction_ = parent_->GetInputDirection();
	parent_->ChangeMotionS(Motion_Crouch, true); //モーション変更

	//カメラ座標
	cameraLookPoint_ = parent_->getCameraLookPoint();

	//速度継承
	velocity_ = parent_->velocity();

	

}
// 終了
void PlayerCrouchState::final()
{
}
// 更新
void PlayerCrouchState::update()
{
	//まず自然落下
	parent_->gravityFall(1.0f);

	//速度継承する
	velocity_ = parent_->velocity();
	moveSpeed_ = sqrtf((velocity_.x * velocity_.x) + (velocity_.z * velocity_.z));

	//しゃがみが離されたらおわり
	if (!InputManager::IsCrouchState()) {
		parent_->changeState(PlayerStateList::State_Walk);
		parent_->ChangeMotionS(Motion_Idle, false); //モーション変更
		parent_->setCameraLookPoint(cameraLookPoint_);
		return;
	}

	//地面から落ちてもステート変更
	if (!parent_->isGround()) {
		parent_->changeState(PlayerStateList::State_Jump);
		parent_->setCameraLookPoint(cameraLookPoint_);
		return;
	}

	//しゃがみジャンプ
	if (InputManager::IsAButtonTrigger()) {
		cameraLookPoint_ = parent_->GetPosition() + velocity_;
		parent_->velocity(GSvector3(my_Input_Direction_.x * -0.05f, 0.40f, my_Input_Direction_.z * -0.05f));
		parent_->changeState(PlayerStateList::State_Jump);
		parent_->setCameraLookPoint(cameraLookPoint_);
		parent_->ChangeMotionS(Motion_BackRolling, false); //モーション変更
		return;
	}

	

	//座標取得
	position_ = parent_->GetPosition();

	//スピード
	if (moveSpeed_ - ACCELERATION > 0) {
		moveSpeed_ -= ACCELERATION;
	}
	else {
		moveSpeed_ = 0;
	}

	


	//向いてる方向に速度をかけて加速度にする
	velocity_ = my_Input_Direction_ * moveSpeed_;


	//いろいろ変えちゃった回転方向を親に返す
	parent_->SetInputDirection(my_Input_Direction_);

	// モーションの変更
	// 移動量のxy成分だけ更新
	parent_->velocity().x = velocity_.x;
	parent_->velocity().z = velocity_.z;

	cameraLookPoint_ = parent_->GetPosition() + velocity_;


	//注視点設定
	parent_->setCameraLookPoint(cameraLookPoint_);

	parent_->setDebugMoveSpeed(moveSpeed_);
}
void PlayerCrouchState::lateUpdate()
{
}
// 描画
void PlayerCrouchState::draw() const
{
}
void PlayerCrouchState::lateDraw() const
{
}
