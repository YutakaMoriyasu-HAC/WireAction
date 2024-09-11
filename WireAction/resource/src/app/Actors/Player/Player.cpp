#include "Player.h"
#include "app/Worlds/IWorld.h"
#include "app/Field/Field.h"
#include "app/Ray/Line.h"
#include "app/NameList/Assets.h"
#include "app/Namelist/PlayerStateList.h"
#include "PlayerMoveState.h"
#include "PlayerJumpState.h"
#include "PlayerThrowWireState.h"
#include "PlayerPendulumState.h"
#include "PlayerQuickWireState.h"
#include "PlayerCrouchState.h"
#include "PlayerDamageState.h"
#include "PlayerBodyAttackState.h"
#include "app/Input/InputManager.h"
#include "app/Actors/WireBeam/WireBeam.h"
#include "app/Input/GameManager.h"
#include <imgui/imgui.h>


//移動速度
const float WalkSpeed{ 0.1f }; //0.025
//自分の高さ
const float PlayerHeight{ 1.0f };
//衝突判定用の半径
const float PlayerRadius{ 0.5f };
//足元のオフセット
//const float FootOffset{ 0.1f };


//ミスとなるy座標
const float MissPosition_{ -30.0f };
//空中ジャンプの最大数
const int MaxAirJumpNum_{ 1 };


//コンストラクタ
Player::Player(IWorld* world, const GSvector3& position) :
	mesh_{ Mesh_Player, 0, true },
	motion_{ Motion_Idle },
	motionLoop_{ true },
	state_timer_{ 0.0f } {
	//ワールドを設定
	world_ = world;
	//タグ名の設定
	tag_ = "PlayerTag";
	//名前の設定
	name_ = "Player";
	//衝突判定球の設定
	collider_ = BoundingSphere{ PlayerRadius, GSvector3{0.0f,PlayerHeight,0.0f} };
	//座標の初期化
	transform_.position(position);
	//メッシュの変換行列を初期化
	mesh_.transform(transform_.localToWorldMatrix());

	InitState();
	lerpSize_ = mesh_.DefaultLerpTime;
	lastPosition_ = position;

	//空中ジャンプ回数リセット
	airJumpNum_ = MaxAirJumpNum_;

	
}

//更新
void Player::update(float delta_time) {
	//入力処理
	ControllerUpdate();

	//フィールドとの衝突判定
	collide_field();
	
	
	//モーションを変更
	mesh_.change_motionS(motion_, motionLoop_,animSpeed_,lerpSize_, startTime_, motionEndTimePlus_);
	//メッシュのモーションを更新
	mesh_.update(delta_time);
	//ワールド変換行列を設定
	mesh_.transform(transform_.localToWorldMatrix());
	//ステートマシンを動かす
	stateMachine_.update();

	footOffset_ = velocity_.y;

	//移動
	SetPosition(transform_.position() + velocity_);
	
	if (stateMachine_.getNowStateID() == State_QuickWire||
		stateMachine_.getNowStateID() == State_BodyAttack ||
		GetMotionState()==Motion_Rolling) {
		attackState_ = true;
	}
	else {
		attackState_ = false;
	}

	//奈落に落ちたとき
	if (transform_.position().y < MissPosition_) {
		velocity_ = GSvector3::zero();
		transform_.position(lastPosition_);
		GameManager::CoinPlus(-5); //コイン-5枚失う
	}

	//無敵時間
	if (invincibleTimer_ > 0) {
		invincibleTimer_ -= delta_time;
		if (invincibleTimer_ <= 0) {
			invincibleTimer_ = 0;
			isInvincible_ = false;
		}
	}

}

void Player::lateUpdate(float delta_time) {



	//フレームの最後に今のフレームの入力状態を記録
	input2_ = input_;

	static float velocityAngle{ debugFloat_[0]};
	static float inputAngle{ debugFloat_[1] };
	static float hikiAngle{ debugFloat_[2] };

	ImGui::Begin("Debug");

	ImGui::Text("motionEnd:%s", IsMotionEnd() ? "true" : "false");
	ImGui::Text("velocityY:%f", velocity_.y);
	ImGui::Text("centerPendulumX:%f", centerPendulum_.x);
	ImGui::Text("playerz:%f", transform_.position().z);
	ImGui::Text("direction(deg):%f", debugFloat_[0]);
	ImGui::Text("direction(now):%f", debugFloat_[1]);
	ImGui::Text("movespeed:%f", debugFloat_[2]);
	ImGui::Text("%f", abs(180 - abs(debugFloat_[0] - debugFloat_[1])));
	ImGui::Text("isWall:%d", isWall_);
	ImGui::Text("trampled:%d", trampled_);
	ImGui::Text("motionTime:%f", mesh_.motion_time());
	ImGui::Text("motioin:%d", motion_);
	ImGui::Text("motioinEndTime:%f", mesh_.motionEndTime());
	ImGui::Text("motionEnd:%s", canWallKick_ ? "true" : "false");
	ImGui::Text("coin:%d",GameManager::HowMuchCoins());
	ImGui::Text("px:%f", p.x);
	ImGui::Text("py:%f", p.y);
	ImGui::Text("pz:%f", p.z);


	ImGui::End();
	
}

//描画
void Player::draw() const {
	
	//衝突判定球のデバッグ表示
	//collider().draw();
	stateMachine_.draw();
	
	//メッシュを描画
	mesh_.draw();
	
}

//衝突リアクション
void Player::react(Actor& other) {
	
	if (isInvincible_)return;

	//敵と衝突したか？
	if (other.tag() == "EnemyTag") {

		isCollide_ = true;

		//地面との衝突判定(線分との交差判定)
		GSvector3 position = transform_.position();
		

		if (other.transform().position().y<position.y+1.5f && !isGround_) {
			attackState_ = true;
			trampled_ = true;
			return;
		}
		else {
			//ダメージ
			if (!attackState_) {
				changeState(PlayerStateList::State_Damage);
				isCollide_ = false;
				if (stateMachine_.getNowStateID() == PlayerStateList::State_Walk) {
					lastPosition_ = transform_.position();
				}
				//無敵になる
				startInvincibilityTime();
				
				return;
			}
		}
	}
	else {
		isCollide_ = false;
	}

	
}

// モーションが終了しているか
const bool Player::IsMotionEnd() const
{
	//return stateMachine_.getNowStateTimer() >= GetMotionEndTime();
	return mesh_.is_end_motion();
}

//モーション変更、速度も変更
void Player::ChangeMotionS(GSuint motion, bool loopFlag, float speed, float lerp, float endTime, float startTime)
{
	motion_ = motion;
	motionLoop_ = loopFlag;
	state_timer_ = 0.0f;
	
	startTime_ = startTime;
	animSpeed_ = speed;
	lerpSize_ = lerp;
	motionEndTimePlus_ = endTime;

}

// モーションの終了時間を取得
const float Player::GetMotionEndTime() const
{
	return mesh_.motionEndTime() - motionEndTimePlus_;
}

//モーションがループするか
const bool Player::IsMotionRoop() const
{
	return motionLoop_;
}

//モーション時間だけリセット
void Player::motionTimeReset() {
	mesh_.motion_time(0.0f);
}

//座標を返す
const GSvector3 Player::GetPosition() const
{
	return transform_.position();
}

//座標にワープ
void Player::SetPosition(GSvector3 pos)
{
	transform_.position(pos);
}

//再生中のアニメを返す
const int Player::GetMotionState() const {
	return motion_;
}



//ダメージ中
void Player::damage(float delta_time) {
	//ノックバックする(ワールド座標系)
	transform_.translate(velocity_ * delta_time, GStransform::Space::World);
	//減速させる
	velocity_ -= GSvector3{ velocity_.x,0.0f,velocity_.z } * 0.5f * delta_time;

	//ダメージモーションの終了を待つ
	if (state_timer_ >= mesh_.motionEndTime()) {
		//move(delta_time);
	}
}

//フィールドとの衝突判定
void Player::collide_field() {
	//壁との衝突判定(球体との判定)
	GSvector3 center;	//衝突後の球体の中心座標
	if (world_->field()->collide(collider(), &center)) {
		//y座標は変更しない
		center.y = transform_.position().y;
		//補正後の座標に変更する
		transform_.position(center);
		isWall_ = true;

		GSvector3 position = transform_.position();
		Line wallLine;
		wallLine.start = position + collider_.center_;
		wallLine.end = position + collider_.center_ + GetInput();
		GSvector3 intersect;	//地面との交点
		
		if (world_->field()->collide(wallLine, &intersect)) {
			canWallKick_ = true;
			wallKickPosition_ = intersect;
		}
		else {
			canWallKick_ = false;
		}
		

	}
	else {
		isWall_ = false;
		canWallKick_ = false;
	}

	//地面との衝突判定(線分との交差判定)
	GSvector3 position = transform_.position();
	Line line;
	line.start	= position + collider_.center_;
	line.end = position + GSvector3{ 0.0f,footOffset_, 0.0f };
	GSvector3 intersect;	//地面との交点

	GSplane plane; //法線の情報
	if (world_->field()->collide(line, &intersect, &plane) && footOffset_<0) {
		//交点の位置からy座標のみ補正する
		position.y = intersect.y;
		//座標を変更する
		transform_.position(position);
		//重力を初期化する
		velocity_.y = 0.0f;
		isGround_ = true;
		p = plane.normal;

		//空中ジャンプ回数リセット
		airJumpNum_ = MaxAirJumpNum_;
	}
	else {
		isGround_ = false;
	}
	

}



//ステート管理
void Player::InitState() {
	stateMachine_.addState(PlayerStateList::State_Walk, std::make_shared<PlayerMoveState>(this, world_, &stateMachine_));
	stateMachine_.addState(PlayerStateList::State_Jump, std::make_shared<PlayerJumpState>(this, world_, &stateMachine_));
	stateMachine_.addState(PlayerStateList::State_ThrowWire, std::make_shared<PlayerThrowWireState>(this, world_, &stateMachine_));
	stateMachine_.addState(PlayerStateList::State_Pendulum, std::make_shared<PlayerPendulumState>(this, world_, &stateMachine_));
	stateMachine_.addState(PlayerStateList::State_QuickWire, std::make_shared<PlayerQuickWireState>(this, world_, &stateMachine_));
	stateMachine_.addState(PlayerStateList::State_Crouch, std::make_shared<PlayerCrouchState>(this, world_, &stateMachine_));
	stateMachine_.addState(PlayerStateList::State_Damage, std::make_shared<PlayerDamageState>(this, world_, &stateMachine_));
	stateMachine_.addState(PlayerStateList::State_BodyAttack, std::make_shared<PlayerBodyAttackState>(this, world_, &stateMachine_));


	// ステートマシンの初期化
	stateMachine_.reset(PlayerStateList::State_Walk);
}

//ステート変更
void Player::changeState(const int state) {
	stateMachine_.chengeState(state);
}

//入力処理
void Player::ControllerUpdate() {
	// 入力
	input_ = MoveInput().normalized();
}

//最後に向いていた方向を返す
const GSvector3 Player::GetInputDirection() const {
	return input_Direction_;
}

//最後に向いていた方向をセット
void Player::SetInputDirection(GSvector3 direction) {
	input_Direction_ = direction;
}

//入力があれば入力方向を、なければ向いてる方向を返す
const GSvector3 Player::GetInput() const {
	if (input_ != GSvector3::zero()) {
		return input_;
	}
	else {
		return GetInputDirection();
	}
}

// 入力
GSvector3 Player::MoveInput()
{
	GSvector3 velocity{ 0.0f,0.0f,0.0f };
	// カメラの方向ベクトルを取得
	GSvector3 forward = world_->camera()->transform().forward();
	//GSvector3 forward = GSvector3(0, 0, -1);
	forward.y = 0.0f;
	// カメラの右方ベクトルを取得
	GSvector3 right = world_->camera()->transform().right();
	right.y = 0.0f;
	GSvector2 vec2 = InputManager::Move();
	velocity += vec2.y * forward;
	velocity += vec2.x * right;
	return velocity;
}

bool Player::isGround() {
	return isGround_;
}

bool Player::isWall() {
	return isWall_;
}

//注視点取得
GSvector3 Player::getCameraLookPoint() {
	return cameraLookPoint_;
}

//注視点セット
void Player::setCameraLookPoint(GSvector3 point) {
	cameraLookPoint_ = point;
}

void Player::setDebugFloat(int num, float value) {
	debugFloat_[num] = value;
}

//召喚
void Player::spone(int actorListNum) {

	

	switch (actorListNum) {
	case Actor_WireBeam:
		if (isThrowing_)break;
		world_->add_actor(std::make_shared<WireBeam>(world_, beamDirection_, beamDirection_ - transform_.position(), "Beam", 30.0f /*伸ばす時間 */ , 1.0f, transform_.rotation(),this));
		isThrowing_ = true;
		break;
	}
}

void Player::setBeamDirection(GSvector3 direction) {
	beamDirection_ = transform_.position() + direction;
	beamDirection_.y = transform_.position().y + 1.5f;
}



void Player::setCenterPendulum(GSvector3 center) {
	centerPendulum_ = center;
}

GSvector3 Player::getCenterPendulum() {
	return centerPendulum_;
}

void Player::setDebugMoveSpeed(float speed) {
	debugMoveSpeed_ = speed;
}

//投げてる
void Player::setThrowing(bool throwing) {
	isThrowing_ = throwing;
}

//投げてる
bool Player::getThrowing() {
	return isThrowing_;
}

//アニメーションタイマリセット
void Player::resetStateTimer() {
	stateMachine_.resetTimer();
}

//敵に触れてるか
bool Player::isCollide() {
	return isCollide_;
}

void Player::changeMesh(GSuint model) {
	mesh_.changeModel(model);
	
}

void Player::SetDebugFloat(float a, float b, float c, float d, float e) {
	debugFloat_[0] = a;
	debugFloat_[1] = b;
	debugFloat_[2] = c;
	debugFloat_[3] = d;
	debugFloat_[4] = e;
}

bool Player::isTrampled(int trampleSwitch) {
	bool re = trampled_;
	if(trampleSwitch>=0)trampled_ = trampleSwitch;


	return re;
}

//壁キック時に壁の座標を返す
bool Player::canWallKick() {
	//kickPosition = &wallKickPosition_;
	return canWallKick_;
}

void Player::setLastPosition(GSvector3 position) {
	lastPosition_ = position;
}

//無敵時間開始
void Player::startInvincibilityTime() {
	invincibleTimer_ = 120.0f;
	isInvincible_ = true;
}

//空中ジャンプ可能か判断して回数-1
bool Player::canAirJump() {
	if (airJumpNum_ > 0) {
		airJumpNum_ -= 1;
		return true;
	}
	else {
		return false;
	}
}