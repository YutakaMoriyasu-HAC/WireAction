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
	parent_->ChangeMotionS(Motion_Idle, true); //モーション変更

	//カメラ座標
	cameraLookPoint_ = parent_->getCameraLookPoint();

	//速度継承
	velocity_= parent_->velocity();

	//このステートが始まった時の速度をここで求めておく
	//通常の歩行よりも速いスピードのまま歩き状態になった時、その速度を維持するということ
	stateStartSpeed = sqrtf((velocity_.x * velocity_.x) + (velocity_.z * velocity_.z));

	//入力があれば加速、無ければ止まる
	if (parent_->input_ != GSvector3::zero()) {
		SState_ = SpeedUp;
	}
	else {
		SState_ = SpeedDown;
	}

}
// 終了
void PlayerMoveState::final()
{
}
// 更新
void PlayerMoveState::update()
{
	//速度継承する
	velocity_ = parent_->velocity();
	moveSpeed_ = sqrtf((velocity_.x * velocity_.x) + (velocity_.z * velocity_.z));

	//ジャンプボタンが押されたらステート変更
	if (InputManager::IsAButtonTrigger()) {
		parent_->changeState(PlayerStateList::State_Jump);
		parent_->velocity().y = 0.18f;
		parent_->setCameraLookPoint(cameraLookPoint_);
		parent_->ChangeMotionS(Motion_Jump, false); //モーション変更
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
	//向いてる方向取得
	my_Input_Direction_ = parent_->GetInput();

	//移動速度の計算
	//速度に応じてさらに状態分けする
	switch (SState_) {
	case Stop:
		moveSpeed_ = 0;
		
		//スティックが倒された瞬間
		if (parent_->input2_ == GSvector3::zero() && parent_->input_ != GSvector3::zero()) {
			moveSpeed_ = MIN_SPEED;
			SState_ = SpeedUp;
			parent_->ChangeMotionS(Motion_Dash, true, 1.1f); //モーション変更
			my_Input_Direction_ = parent_->GetInput();
		}
		break;

	case SpeedUp:
		// 移動しているか
		if (velocity_.length() != 0.0f) {

			changeAngle();
		}

		//速度調整
		if (stateStartSpeed > MAX_SPEED) {
			moveSpeed_ = stateStartSpeed;
		}
		else if (moveSpeed_ + ACCELERATION < MAX_SPEED) {
			moveSpeed_ += ACCELERATION;
		}
		else {
			moveSpeed_ = MAX_SPEED;
		}
		//もしアニメが変わってなかったら変更
		if (parent_->GetMotionState() != Motion_Dash) {
			parent_->ChangeMotionS(Motion_Dash, true, 1.1f); //モーション変更
		}

		//スティックが離された瞬間
		if (parent_->input2_ != GSvector3::zero() && parent_->input_ == GSvector3::zero()) {
			//減速開始
			SState_ = SpeedDown;
			parent_->ChangeMotionS(Motion_Walk, true); //モーション変更
			break;
		}

		if (parent_->input2_ != parent_->input_) {
			my_Input_Direction_ = parent_->GetInput();
		}

		break;

	case SpeedDown:
		if (moveSpeed_ - ACCELERATION*4 > 0) {
			moveSpeed_ -= ACCELERATION * 2;
		}
		else {
			moveSpeed_ = 0;
			SState_ = Stop;
			parent_->ChangeMotionS(Motion_Idle, true); //モーション変更
		}
		//スティックが倒された瞬間
		if (parent_->input2_ == GSvector3::zero() && parent_->input_ != GSvector3::zero()) {
			moveSpeed_ = MIN_SPEED;
			SState_ = SpeedUp;
			my_Input_Direction_ = parent_->GetInput();
			parent_->ChangeMotionS(Motion_Dash, true, 1.1f); //モーション変更
		}
		break;
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

	//移動
	parent_->SetPosition(parent_->GetPosition() + velocity_);

	
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
