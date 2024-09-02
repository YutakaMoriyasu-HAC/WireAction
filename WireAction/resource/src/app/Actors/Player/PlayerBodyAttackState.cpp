#include "PlayerBodyAttackState.h"
#include "app/Namelist/PlayerStateList.h"
#include "app/Worlds/World.h"
#include "app/time/Time.h"
#include "app/Math/Math.h"
#include "app/Input/InputManager.h"
#include "app/Namelist/PlayerStateList.h"
#include "app/NameList/Assets.h"



// コンストラクタ
PlayerBodyAttackState::PlayerBodyAttackState(Player* parent, IWorld* world, app::StateMachine* stateMachine) :
	StateBase(stateMachine), parent_(parent), world_(world)
{

}
// 初期化
void PlayerBodyAttackState::init()
{

	//速度継承
	velocity_ = parent_->velocity();
	//カメラ座標
	cameraLookPoint_ = parent_->getCameraLookPoint();
}

// 終了
void PlayerBodyAttackState::final()
{
}
// 更新
void PlayerBodyAttackState::update()
{
	//まず自然落下
	parent_->gravityFall(1.0f);

	//速度継承する
	velocity_ = parent_->velocity();

	cameraLookPoint_.x = parent_->GetPosition().x + velocity_.x;
	cameraLookPoint_.z = parent_->GetPosition().z + velocity_.z;


	//プレイヤーがジャンプ前の座標よりも下に落ちてたら注視点も下げる
	if (parent_->GetPosition().y < cameraLookPoint_.y) {
		cameraLookPoint_.y = parent_->GetPosition().y;
	}

	//注視点設定
	parent_->setCameraLookPoint(cameraLookPoint_);




	//着地したら終了
	if (parent_->isGround()) {
		
		if (parent_->input_ != GSvector3::zero()) {
			parent_->ChangeMotionS(Motion_Rolling, false,3.0f,0.5f,10.0f); //モーション変更
		}
		else {
			parent_->ChangeMotionS(Motion_Idle, true); //モーション変更
		}
		parent_->changeState(PlayerStateList::State_Walk);

		return;
	}





}

void PlayerBodyAttackState::lateUpdate()
{

}
// 描画
void PlayerBodyAttackState::draw() const
{
	

}

void PlayerBodyAttackState::lateDraw() const
{
}
