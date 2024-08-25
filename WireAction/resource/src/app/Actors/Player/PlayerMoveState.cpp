#include "PlayerMoveState.h"
#include "app/Namelist/PlayerStateList.h"
#include "app/Worlds/World.h"
#include "app/time/Time.h"
#include "app/Math/Math.h"
#include "app/Input/InputManager.h"
#include "app/NameList/Assets.h"

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

	//前のフレームの向いてる角度
	previousAngle_ = math::angleFromVector2Deg(my_Input_Direction_.x, my_Input_Direction_.z);

}
// 終了
void PlayerMoveState::final()
{
}
// 更新
void PlayerMoveState::update()
{
	//まず自然落下
	parent_->gravityFall(1.0f);

	//速度継承する
	velocity_ = parent_->velocity();
	moveSpeed_ = sqrtf((velocity_.x * velocity_.x) + (velocity_.z * velocity_.z));

	//ジャンプボタンが押されたらステート変更
	if (InputManager::IsAButtonTrigger() && SState_!=brake) {
		cameraLookPoint_ = parent_->GetPosition() + velocity_;
		parent_->changeState(PlayerStateList::State_Jump);
		parent_->velocity().y = 0.32f; //0.18
		parent_->setCameraLookPoint(cameraLookPoint_);
		parent_->ChangeMotionS(Motion_JumpIn, false); //モーション変更
		return;
	}

	//地面から落ちてもステート変更
	if (!parent_->isGround()) {
		parent_->changeState(PlayerStateList::State_Jump);
		parent_->setCameraLookPoint(cameraLookPoint_);
		return;
	}

	//Xボタンを押したらワイヤー投げに移行
	if (InputManager::IsXButtonTrigger()) {
		if (parent_->getThrowing())return;

		changeAngle(600.0f);
		parent_->setBeamDirection(my_Input_Direction_);
		parent_->changeState(PlayerStateList::State_ThrowWire);
		parent_->ChangeMotionS(Motion_JumpNow, false); //モーション変更


		cameraLookPoint_.x = parent_->GetPosition().x + velocity_.x;
		cameraLookPoint_.z = parent_->GetPosition().z + velocity_.z;

		//注視点設定
		parent_->setCameraLookPoint(cameraLookPoint_);
		return;
	}

	//しゃがみ
	if (InputManager::IsCrouchState()) {
		parent_->setCameraLookPoint(cameraLookPoint_);
		parent_->changeState(PlayerStateList::State_Crouch);
		parent_->ChangeMotionS(Motion_Crouch, true); //モーション変更
		return;
	}

	//座標取得
	position_ = parent_->GetPosition();
	//向いてる方向取得
	my_Input_Direction_ = parent_->GetInput();

	float nowAngle = 0;

	//移動速度の計算
	//速度に応じてさらに状態分けする
	switch (SState_) {
	case Stop:
		moveSpeed_ = 0;
		
		//スティックが倒された瞬間
		if (parent_->input2_ == GSvector3::zero() && parent_->input_ != GSvector3::zero()) {
			moveSpeed_ = MIN_SPEED;
			SState_ = SpeedUp;
			parent_->ChangeMotionS(Motion_Run, true, 1.1f); //モーション変更
			my_Input_Direction_ = parent_->GetInput();
		}

		//向いてる方向に速度をかけて加速度にする
		velocity_ = my_Input_Direction_ * moveSpeed_;

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

		if (moveSpeed_ > MAX_SPEED) {
			//転がり中は減衰
			parent_->ChangeMotionS(Motion_Rolling, true, moveSpeed_*10.0f,0.5f,8); //モーション変更
			moveSpeed_ -= ACCELERATION / 2;
			stateStartSpeed = moveSpeed_;
			

		}else if (parent_->GetMotionState() != Motion_Run) {
			//もしアニメが変わってなかったら変更
			parent_->ChangeMotionS(Motion_Run, true, 1.1f); //モーション変更
		}

		//スティックが離された瞬間
		if (parent_->input2_ != GSvector3::zero() && parent_->input_ == GSvector3::zero()) {
			//減速開始
			SState_ = SpeedDown;
			parent_->ChangeMotionS(Motion_Walk, true); //モーション変更
		}
		else {
			if (parent_->input2_ != parent_->input_) {
				my_Input_Direction_ = parent_->GetInput();
			}
		}

		nowAngle = math::angleFromVector2Deg(my_Input_Direction_.x, my_Input_Direction_.z);
		if (nowAngle < 0)nowAngle += 360;
		//急に逆方向にスティック
		if (abs(180-abs(previousAngle_ - nowAngle)) <30
			&& nowAngle!=0
			&& moveSpeed_ > MIN_SPEED/2) {
			SState_ = brake;
			parent_->ChangeMotionS(Motion_Brake, true); //Brake
		}

		
		//向いてる方向に速度をかけて加速度にする
		velocity_ = my_Input_Direction_ * moveSpeed_;

		break;

	case SpeedDown:
		if (moveSpeed_ - ACCELERATION*2 > 0) {
			moveSpeed_ -= ACCELERATION * 2;
			//速度調整
			if (stateStartSpeed > moveSpeed_) {
				stateStartSpeed = moveSpeed_;
			}
		}
		else {
			moveSpeed_ = 0;
			SState_ = Stop;
			parent_->ChangeMotionS(Motion_Idle, true); //モーション変更
		}

		//後ろ向きだったら速度も後ろ向き
		if (velocity_.x * my_Input_Direction_.x <= 0 && velocity_.z * my_Input_Direction_.z <= 0) {
			moveSpeed_ *= -1;
		}

		//スティックが倒された瞬間
		if (parent_->input2_ == GSvector3::zero() && parent_->input_ != GSvector3::zero()) {
			my_Input_Direction_ = parent_->GetInput();
			nowAngle = math::angleFromVector2Deg(my_Input_Direction_.x, my_Input_Direction_.z);
			if (nowAngle < 0)nowAngle += 360;
			//急に逆方向にスティック
			if (abs(180 - abs(previousAngle_ - nowAngle)) < 30
				&& nowAngle != 0
				) {
				SState_ = brake;
				parent_->ChangeMotionS(Motion_Brake, true); //Brake
			}
			else {
				moveSpeed_ = MIN_SPEED;
				SState_ = SpeedUp;
				parent_->ChangeMotionS(Motion_Run, true, 1.1f); //モーション変更
			}
		}

		//向いてる方向に速度をかけて加速度にする
		velocity_ = my_Input_Direction_ * moveSpeed_;

		break;

	case brake:
		if (moveSpeed_ - ACCELERATION > 0) {
			moveSpeed_ -= ACCELERATION;
			//速度調整
			if (stateStartSpeed > moveSpeed_) {
				stateStartSpeed = moveSpeed_;
			}
		}
		else {
			moveSpeed_ = 0;
			
			parent_->ChangeMotionS(Motion_Idle, true); //モーション変更
			if (parent_->input_ != GSvector3::zero()) {
				SState_ = SpeedUp;
			}
			else {
				SState_ = Stop;
			}
		}

		//向いてる方向に速度をかけて加速度にする
		velocity_ = my_Input_Direction_ * -moveSpeed_;
		
		//ジャンプボタンが押されたらステート変更
		if (InputManager::IsAButtonTrigger()) {
			cameraLookPoint_ = parent_->GetPosition() + velocity_;
			parent_->changeState(PlayerStateList::State_Jump);
			parent_->velocity(GSvector3(my_Input_Direction_.x * 0.1f, 0.40f, my_Input_Direction_.z * 0.1f));
			parent_->setCameraLookPoint(cameraLookPoint_);
			parent_->ChangeMotionS(Motion_BackRolling, false); //モーション変更
			return;
		}

	}

	
	

	//いろいろ変えちゃった回転方向を親に返す
	parent_->SetInputDirection(my_Input_Direction_);

	// モーションの変更
	// 移動量のxy成分だけ更新
	parent_->velocity().x = velocity_.x;
	parent_->velocity().z = velocity_.z;

	cameraLookPoint_ = parent_->GetPosition() + velocity_;

	//移動
	//parent_->SetPosition(parent_->GetPosition() + velocity_);

	
	//注視点設定
	parent_->setCameraLookPoint(cameraLookPoint_);

	//向き
	//parent_->setDebugMoveSpeed(nowAngle);
	parent_->SetDebugFloat(previousAngle_, nowAngle,moveSpeed_);

	//最後に前のフレームの値をとっておく
	if (nowAngle != 0.0f) {
		previousAngle_ = nowAngle;
	}
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

void PlayerMoveState::changeAngle(float time) {
	//向きの補間
	GSquaternion rotation =
		GSquaternion::rotateTowards(
			parent_->transform().rotation(),
			GSquaternion::lookRotation(my_Input_Direction_), time);
	parent_->transform().rotation(rotation);
}
