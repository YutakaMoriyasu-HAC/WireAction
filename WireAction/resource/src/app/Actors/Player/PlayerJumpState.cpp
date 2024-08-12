#include "PlayerJumpState.h"
#include "app/Namelist/PlayerStateList.h"
#include "app/Worlds/World.h"
#include "app/time/Time.h"
#include "app/Math/Math.h"
#include "app/Input/InputManager.h"


// 移動速度
const float MIN_SPEED{ 0.005f };
const float ACCELERATION{ 0.003f };
const float MAX_SPEED{ 0.15f };
const float ACCELERATION2{ 0.1f };
const float MAX_SPEED2{ 0.25f };

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

}
// 終了
void PlayerJumpState::final()
{
}
// 更新
void PlayerJumpState::update()
{
	if (parent_->isGround()) {
		parent_->changeState(PlayerStateList::State_Walk);
	}

	//座標取得
	position_ = parent_->GetPosition();

	//向いてる方向
	my_Input_Direction_ = parent_->GetInput();

	// キーの入力から移動ベクトルを取得
	GSvector3 velocity = parent_->velocity();

	
	//いろいろ変えちゃった回転方向を親に返す
	parent_->SetInputDirection(my_Input_Direction_);

	// モーションの変更
	// 移動量のxy成分だけ更新
	parent_->velocity().x = velocity.x;
	parent_->velocity().z = velocity.z;

	cameraLookPoint_.x = parent_->GetPosition().x + velocity.x;
	cameraLookPoint_.z = parent_->GetPosition().z + velocity.z;

	//移動
	parent_->SetPosition(parent_->GetPosition() + velocity);

	//プレイヤーがジャンプ前の座標よりも下に落ちてたら注視点も下げる
	if (parent_->GetPosition().y < cameraLookPoint_.y) {
		cameraLookPoint_.y = parent_->GetPosition().y;
	}

	//注視点設定
	parent_->setCameraLookPoint(cameraLookPoint_);


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

void PlayerJumpState::changeAngle() {
	//向きの補間
	GSquaternion rotation =
		GSquaternion::rotateTowards(
			parent_->transform().rotation(),
			GSquaternion::lookRotation(my_Input_Direction_), 12.0f);
	parent_->transform().rotation(rotation);
}
