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
#include "app/Input/InputManager.h"
#include "app/Actors/WireBeam/WireBeam.h"
#include <imgui/imgui.h>


//移動速度
const float WalkSpeed{ 0.1f }; //0.025
//自分の高さ
const float PlayerHeight{ 1.0f };
//衝突判定用の半径
const float PlayerRadius{ 0.5f };
//足元のオフセット
//const float FootOffset{ 0.1f };
//重力値
const float Gravity{ -0.016f }; //-0.016 //-0.008


//コンストラクタ
Player::Player(IWorld* world, const GSvector3& position) :
	mesh_{ Mesh_Player, 0, true },
	motion_{ Motion_Idle },
	motion_loop_{ true },
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

	
}

//更新
void Player::update(float delta_time) {
	//入力処理
	ControllerUpdate();

	//フィールドとの衝突判定
	collide_field();
	
	
	//モーションを変更
	mesh_.change_motionS(motion_, motion_loop_,animSpeed_,lerpSize_, startTime_);
	//メッシュのモーションを更新
	mesh_.update(delta_time);
	//ワールド変換行列を設定
	mesh_.transform(transform_.localToWorldMatrix());
	//ステートマシンを動かす
	stateMachine_.update();

	footOffset_ = velocity_.y;

	//移動
	SetPosition(transform_.position() + velocity_);
	
	if (stateMachine_.getNowStateID() == State_QuickWire) {
		attackState_ = true;
	}
	else {
		attackState_ = false;
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
	
	//敵と衝突したか？
	if (other.tag() == "EnemyTag") {

		isCollide_ = true;

		//地面との衝突判定(線分との交差判定)
		GSvector3 position = transform_.position();
		

		if (other.transform().position().y<position.y && footOffset_ < 0 && !isGround_) {
			attackState_ = true;
			trampled_ = true;
			return;
		}
		else {
			//踏んづけてない！
			attackState_ = false;
			//ダメージ
			if (stateMachine_.getNowStateID() != State_QuickWire) {
				changeState(PlayerStateList::State_Damage);
				isCollide_ = false;
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
	return stateMachine_.getNowStateTimer() >= GetMotionEndTime();
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
	}
	else {
		isWall_ = false;
	}

	//地面との衝突判定(線分との交差判定)
	GSvector3 position = transform_.position();
	Line line;
	line.start	= position + collider_.center_;
	line.end = position + GSvector3{ 0.0f,footOffset_, 0.0f };
	GSvector3 intersect;	//地面との交点

	if (world_->field()->collide(line, &intersect) && footOffset_<0) {
		//交点の位置からy座標のみ補正する
		position.y = intersect.y;
		//座標を変更する
		transform_.position(position);
		//重力を初期化する
		velocity_.y = 0.0f;
		isGround_ = true;
	}
	else {
		isGround_ = false;
	}

}

//アクターとの衝突処理
void Player::collide_actor(Actor& other) {
	//y座標を除く座標を求める
	GSvector3 position = transform_.position();
	position.y = 0.0f;
	GSvector3 target = other.transform().position();
	target.y = 0.0f;
	//相手との距離
	float distance = GSvector3::distance(position, target);
	//衝突判定球の半径同士を加えた長さを求める
	float length = collider_.radius_ + other.collider().radius_;
	//衝突判定球の重なっている長さを求める
	float overlap = length - distance;
	//重なっている部分の半分の距離だけ離れる移動量を求める
	GSvector3 v = (position - target).getNormalized() * overlap * 0.5f;
	transform_.translate(v, GStransform::Space::World);
	//フィールドとの衝突判定
	collide_field();
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

void Player::gravityFall(float delta_time) {
	//重力値を更新
	velocity_.y += Gravity * delta_time;
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