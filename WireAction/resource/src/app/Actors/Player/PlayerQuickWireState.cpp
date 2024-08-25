#include "PlayerQuickWireState.h"
#include "app/Namelist/PlayerStateList.h"
#include "app/Worlds/World.h"
#include "app/time/Time.h"
#include "app/Math/Math.h"
#include "app/Input/InputManager.h"
#include "app/Namelist/PlayerStateList.h"
#include "app/NameList/Assets.h"



// コンストラクタ
PlayerQuickWireState::PlayerQuickWireState(Player* parent, IWorld* world, app::StateMachine* stateMachine) :
	StateBase(stateMachine), parent_(parent), world_(world)
{

}
// 初期化
void PlayerQuickWireState::init()
{
	wirePosition_ = parent_->getCenterPendulum();
	time_ = 100.0f;
	timer_ = time_;
	previousPosition_ = parent_->transform().position();
	startDirection_ = wirePosition_ - previousPosition_; //移動するベクトル
}

// 終了
void PlayerQuickWireState::final()
{
}
// 更新
void PlayerQuickWireState::update()
{
	

	//parent_->Player::move_to(wirePosition_, time_).name("wire2");
	//ワイヤーを繋げた位置からプレイヤーまでのベクトルを正規化してその移動距離を速度にする
	velocity_ = startDirection_ * (1 / sqrtf((startDirection_.x * startDirection_.x) + (startDirection_.z * startDirection_.z)))*0.5f;

	GSvector3 direction = wirePosition_ - previousPosition_;
	if (direction.x * velocity_.x < 0 || direction.z * velocity_.z < 0) {
		parent_->velocity(GSvector3(0.0f, 0.0f, 0.0f));
		if (parent_->isCollide()) {
			parent_->velocity(GSvector3(0.0f, 0.35f, 0.0f));
			parent_->transform().position(wirePosition_);
		}
		parent_->changeState(PlayerStateList::State_Jump);
		parent_->ChangeMotionS(Motion_JumpSpin, false, 1.0f, 0.5f, 2.0f);
		return;
	}


	timer_ -= 1.0f;
	if (timer_ <= 0 || parent_->isWall()) {
		//注視点設定
		parent_->setCameraLookPoint(parent_->transform().position());
		parent_->velocity(GSvector3(0.0f, 0.01f, 0.0f));
		parent_->changeState(PlayerStateList::State_Jump);
		return;
	}

	if (InputManager::IsAButtonTrigger()) {
		Tween::cancel("wire2");
		parent_->changeState(PlayerStateList::State_Jump);
	}

	

	
	

	//注視点設定
	parent_->setCameraLookPoint(parent_->transform().position());

	parent_->velocity(velocity_);

	previousPosition_ = parent_->transform().position();

}

void PlayerQuickWireState::lateUpdate()
{

}
// 描画
void PlayerQuickWireState::draw() const
{
	GSvector2 startPoint = GSvector2::zero();
	GSvector2 endPoint = GSvector2::zero();
	GSvector3 handPosition = parent_->transform().position();
	handPosition.y += 0.5f; //手の位置のオフセット
	GSvector3 centerPosition = wirePosition_;

	//それぞれの座標を画面座標に変換
	gsCalculateScreen(&startPoint, &handPosition);
	gsCalculateScreen(&endPoint, &centerPosition);

	//画面座標がマイナスにならないようにする
	if (startPoint.x > 10000) {
		startPoint.x -= 20000.0f;
	}


	//画像の中心決定
	static const GSvector2 center{ 16.0f,0.0f };
	//色
	GScolor color = { 1.0f, 1.0f, 1.0f, 1.0f };

	float nagasa = sqrtf((endPoint.y - startPoint.y) * (endPoint.y - startPoint.y) + (endPoint.x - startPoint.x) * (endPoint.x - startPoint.x));
	GSvector2 end_scale{ 0.5f, nagasa / 32.0f };

	GSvector2 position = GSvector2(endPoint);

	//画像の切り抜き
	static const GSrect Rect{ 0.0f, 0.0f, 32.0f,32.0f };

	//角度決定
	float drawAngle = atan2f(endPoint.y - startPoint.y, endPoint.x - startPoint.x) * (180 / math::PI) + 90;

	gsDrawSprite2D(Texture_Wire, &position, &Rect, &center, &color, &end_scale, drawAngle);
	//(Texture_Wire, &position_, &Rect, NULL, &color, &end_scale, drawAngle);

}

void PlayerQuickWireState::lateDraw() const
{
}
