#include "PlayerMoveState.h"
#include "app/Namelist/PlayerStateList.h"
#include "app/Worlds/World.h"
#include "app/time/Time.h"
#include "app/Math/Math.h"
#include "app/Input/InputManager.h"


// 移動速度
const float MIN_SPEED{ 0.008f };
const float ACCELERATION{ 0.005f };
const float MAX_SPEED{ 0.2f };
const float ACCELERATION2{ 0.1f };
const float MAX_SPEED2{ 0.25f };

// << プレイヤーの移動 >>

// コンストラクタ
PlayerMoveState::PlayerMoveState(Player* parent, IWorld* world, app::StateMachine* stateMachine) :
	StateBase(stateMachine), parent_(parent), world_(world), isDash_(false)
{
	moveSpeed_ = 0;
}
// 初期化
void PlayerMoveState::init()
{
	
	my_Input_Direction_ = parent_->GetInput();

	//カメラ座標
	cameraLookPoint_ = parent_->getCameraLookPoint();

	//入力があれば加速、無ければ止まる
	if (parent_->input_ != GSvector3::zero()) {
		SState_ = SpeedUp;
	}
	else {
		SState_ = Stop;
	}

}
// 終了
void PlayerMoveState::final()
{
}
// 更新
void PlayerMoveState::update()
{
	//ジャンプボタンが押されたらステート変更
	if (InputManager::IsAButtonTrigger()) {
		parent_->changeState(PlayerStateList::State_Jump);
		parent_->velocity().y = 0.25f;
		parent_->setCameraLookPoint(cameraLookPoint_);
		return;
	}

	//地面から落ちてもステート変更
	if (!parent_->isGround()) {
		parent_->changeState(PlayerStateList::State_Jump);
		parent_->setCameraLookPoint(cameraLookPoint_);
		return;
	}

	//座標取得
	position_ = parent_->GetPosition();

	//向いてる方向
	my_Input_Direction_ = parent_->GetInput();

	// キーの入力から移動ベクトルを取得
	GSvector3 velocity = my_Input_Direction_ * moveSpeed_;
	
	//速度に応じてさらに状態分け
	switch (SState_) {
	case Stop:
		moveSpeed_ = 0;
		//スティックが倒された瞬間
		if (parent_->input2_ == GSvector3::zero() && parent_->input_ != GSvector3::zero()) {
			moveSpeed_ = MIN_SPEED;
			SState_ = SpeedUp;
			my_Input_Direction_ = parent_->GetInput();
		}
		break;

	case SpeedUp:
		// 移動しているか
		if (velocity.length() != 0.0f) {

			changeAngle();
		}

		//速度調整
		if (moveSpeed_ + ACCELERATION < MAX_SPEED) {
			moveSpeed_ += ACCELERATION;
		}
		else {
			moveSpeed_ = MAX_SPEED;
		}

		//スティックが離された瞬間
		if (parent_->input2_ != GSvector3::zero() && parent_->input_ == GSvector3::zero()) {
			//減速開始
			SState_ = SpeedDown;
			break;
		}

		if (parent_->input2_ != parent_->input_) {
			my_Input_Direction_ = parent_->GetInput();
		}

		break;

	case SpeedDown:
		if (moveSpeed_ - ACCELERATION*4 > 0) {
			moveSpeed_ -= ACCELERATION * 4;
		}
		else {
			moveSpeed_ = 0;
			SState_ = Stop;
		}
		//スティックが倒された瞬間
		if (parent_->input2_ == GSvector3::zero() && parent_->input_ != GSvector3::zero()) {
			moveSpeed_ = MIN_SPEED;
			SState_ = SpeedUp;
			my_Input_Direction_ = parent_->GetInput();
		}
		break;
	}
	//いろいろ変えちゃった回転方向を親に返す
	parent_->SetInputDirection(my_Input_Direction_);

	// モーションの変更
	// 移動量のxy成分だけ更新
	parent_->velocity().x = velocity.x;
	parent_->velocity().z = velocity.z;

	cameraLookPoint_ = parent_->GetPosition() + velocity;

	//移動
	parent_->SetPosition(parent_->GetPosition() + velocity);

	
	//注視点設定
	parent_->setCameraLookPoint(cameraLookPoint_);


}
void PlayerMoveState::lateUpdate()
{
}
// 描画
void PlayerMoveState::draw() const
{
}
void PlayerMoveState::lateDraw() const
{
}

void PlayerMoveState::changeAngle() {
	//向きの補間
	GSquaternion rotation =
		GSquaternion::rotateTowards(
			parent_->transform().rotation(),
			GSquaternion::lookRotation(my_Input_Direction_), 12.0f);
	parent_->transform().rotation(rotation);
}
