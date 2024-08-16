#include "PlayerJumpState.h"
#include "app/Namelist/PlayerStateList.h"
#include "app/Worlds/World.h"
#include "app/time/Time.h"
#include "app/Math/Math.h"
#include "app/Input/InputManager.h"


// 移動速度
const float MIN_SPEED{ 0.008f };
const float ACCELERATION{ 0.001f };
const float MAX_SPEED{ 0.05f };

// << プレイヤーの移動 >>

// コンストラクタ
PlayerJumpState::PlayerJumpState(Player* parent, IWorld* world, app::StateMachine* stateMachine) :
	StateBase(stateMachine), parent_(parent), world_(world)
{
}
// 初期化
void PlayerJumpState::init()
{
	my_Input_Direction_ = parent_->GetInput();

	//カメラ座標
	cameraLookPoint_ = parent_->getCameraLookPoint();

	//速度継承
	velocity_ = parent_->velocity();

	//このステートが始まった時の速度をここで求めておく
	//通常の歩行よりも速いスピードのまま歩き状態になった時、その速度を維持するということ
	stateStartSpeed = sqrtf((velocity_.x * velocity_.x) + (velocity_.z * velocity_.z));
	stateStartVec = velocity_;

	//ジャンプ回数リセット
	jumpNum_ = 1;

	//ボタンを押している状態
	buttonReleaseFlag_ = false;


}
// 終了
void PlayerJumpState::final()
{
}
// 更新
void PlayerJumpState::update()
{
	//まず自然落下
	parent_->gravityFall(1.0f);

	//速度継承する
	velocity_ = parent_->velocity();
	moveSpeed_ = sqrtf((velocity_.x * velocity_.x) + (velocity_.z * velocity_.z));

	
	//parent_->setCanCollideField(true);

	//空中ジャンプ
	if (InputManager::IsAButtonTrigger() && jumpNum_ >= 1) {
		parent_->velocity().y = 0.15f;
		cameraLookPoint_.y = (cameraLookPoint_.y+parent_->GetPosition().y)/2;
		jumpNum_ -= 1;
		parent_->ChangeMotionS(Motion_Jump, false); //モーション変更
		buttonReleaseFlag_ = false;
	}

	//Xボタンを押したらワイヤー投げに移行
	if (InputManager::IsXButtonTrigger()) {
		changeAngle(600.0f);
		parent_->setBeamDirection(my_Input_Direction_);
		parent_->changeState(PlayerStateList::State_ThrowWire);
		parent_->ChangeMotionS(Motion_Jump, false); //モーション変更
		parent_->velocity().y = 0.15f;
		
		return;
	}

	//ボタンを離した瞬間
	if (!InputManager::IsAButtonState() && !buttonReleaseFlag_) {
		buttonReleaseFlag_ = true;
	}

	

	

	//上昇中かつボタン離していたら減衰
	//すぐ減衰すごく減衰
	if (buttonReleaseFlag_ && velocity_.y > 0) {
		velocity_.y -= ACCELERATION*100;
		if (velocity_.y < 0) {
			velocity_.y = 0;
		}
	}

	

	//座標取得
	position_ = parent_->GetPosition();

	//向いてる方向
	my_Input_Direction_ = parent_->GetInput();

	//スティック入力があった時、現在のベクトルからスティック入力方向までのベクトルを求める
	if (parent_->input_ != GSvector3::zero()) {

		//my_Input_Direction_の角度
		float inputAngle = atanf(my_Input_Direction_.z / my_Input_Direction_.x)*(180/math::PI);
		if (inputAngle > 360)inputAngle -= 360;
		if (inputAngle <0)inputAngle += 360;

		//ベクトル角度
		float velocityAngle = atanf(velocity_.z / velocity_.x) * (180 / math::PI);
		if (velocityAngle > 360)velocityAngle -= 360;
		if (velocityAngle < 0)velocityAngle += 360;



		//スティック角度が元の角度より60度以内だったら速度継承
		if ((ABS(velocityAngle - inputAngle) < 60 || ABS(velocityAngle - inputAngle) >300) && stateStartSpeed!=0) {
			velocityToInputVector_ = (my_Input_Direction_ * stateStartSpeed) - velocity_;
		}
		else {
			velocityToInputVector_ = (my_Input_Direction_ * MAX_SPEED) - velocity_;
		}

		parent_->setDebugFloat(0, velocityAngle);
		parent_->setDebugFloat(1, inputAngle);
		parent_->setDebugFloat(2, ABS(velocityAngle - inputAngle));


		velocity_.x += velocityToInputVector_.x / 15.0f;
		velocity_.z += velocityToInputVector_.z / 15.0f;
	}
	
	


	//いろいろ変えちゃった回転方向を親に返す
	parent_->SetInputDirection(my_Input_Direction_);

	// モーションの変更
	// 移動量のxy成分だけ更新
	parent_->velocity(velocity_);
	//parent_->velocity().z = velocity_.z;

	cameraLookPoint_.x = parent_->GetPosition().x + velocity_.x;
	cameraLookPoint_.z = parent_->GetPosition().z + velocity_.z;

	//移動
	//parent_->SetPosition(parent_->GetPosition() + velocity_);
	//自然落下


	//プレイヤーがジャンプ前の座標よりも下に落ちてたら注視点も下げる
	if (parent_->GetPosition().y < cameraLookPoint_.y) {
		cameraLookPoint_.y = parent_->GetPosition().y;
	}

	//注視点設定
	parent_->setCameraLookPoint(cameraLookPoint_);

	
	//着地したら終了
	if (parent_->isGround()) {
		parent_->changeState(PlayerStateList::State_Walk);
		parent_->ChangeMotionS(Motion_Idle, false); //モーション変更
		return;
	}


}
void PlayerJumpState::lateUpdate()
{
}
// 描画
void PlayerJumpState::draw() const
{
}
void PlayerJumpState::lateDraw() const
{
}

void PlayerJumpState::changeAngle(float speed) {
	//向きの補間
	GSquaternion rotation =
		GSquaternion::rotateTowards(
			parent_->transform().rotation(),
			GSquaternion::lookRotation(my_Input_Direction_), speed);
	parent_->transform().rotation(rotation);
}
