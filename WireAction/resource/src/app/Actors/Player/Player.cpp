#include "Player.h"
#include "app/Worlds/IWorld.h"
#include "app/Field/Field.h"
#include "app/Ray/Line.h"
#include "app/NameList/Assets.h"

//モーション番号
enum {
	MotionIdle = 1,     // アイドル
	MotionForwardWalk = 2,     // 前進
	MotionBackwardWalk = 3,     // 後退
	MotionLeftWalk = 4,     // 左歩き
	MotionRightWalk = 5,     // 右歩き
	MotionFire = 11,    // 射撃
	MotionDamage = 14,    // ダメ―ジ
	MotionJump = 17     // ジャンプ
};

//移動速度
const float WalkSpeed{ 0.1f }; //0.025
//自分の高さ
const float PlayerHeight{ 1.0f };
//衝突判定用の半径
const float PlayerRadius{ 0.5f };
//足元のオフセット
const float FootOffset{ 0.1f };
//重力値
const float Gravity{ -0.016f };

//コンストラクタ
Player::Player(IWorld* world, const GSvector3& position) :
	mesh_{ Mesh_Player, MotionIdle, true },
	motion_{ MotionIdle },
	motion_loop_{ true },
	state_{ State::Move },
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
}

//更新
void Player::update(float delta_time) {
	//状態の更新
	update_state(delta_time);
	//重力値を更新
	velocity_.y += Gravity * delta_time;
	//重力を加える
	transform_.translate(0.0f, velocity_.y, 0.0f);
	//フィールドとの衝突判定
	collide_field();
	//モーションを変更
	mesh_.change_motion(motion_, motion_loop_);
	//メッシュのモーションを更新
	mesh_.update(delta_time);
	//ワールド変換行列を設定
	mesh_.transform(transform_.localToWorldMatrix());
}

//描画
void Player::draw() const {
	//メッシュを描画
	mesh_.draw();
	//衝突判定球のデバッグ表示
	collider().draw();
}

//衝突リアクション
void Player::react(Actor& other) {
	//ダメージ中の場合は何もしない
	if (state_ == State::Damage) return;
	//敵の攻撃判定と衝突したか？
	if (other.tag() == "EnemyAttackTag") {
		//ターゲット方向のベクトルを求める
		GSvector3 to_target = other.transform().position() - transform().position();
		//y成分は無効にする
		to_target.y = 0.0f;
		//ターゲット方向と逆方向にノックバックする移動量を求める
		velocity_ = -to_target.getNormalized() * 0.4f;
		//ダメージ状態に遷移する
		change_state(State::Damage, MotionDamage, false);
		return;
	}
	//敵と衝突したか？
	if (other.tag() == "EnemyTag") {
		//アクター同士が重ならないように補正する
		collide_actor(other);
	}

	
}

//状態の更新
void Player::update_state(float delta_time) {
	//状態遷移
	switch (state_) {
	case State::Move:	move(delta_time);	break;
	case State::Attack:	attack(delta_time);	break;
	case State::Damage:	damage(delta_time);	break;
	}
	//状態タイマの更新
	state_timer_ += delta_time;
}

//状態の変更
void Player::change_state(State state, GSuint motion, bool loop) {
	motion_ = motion;
	motion_loop_ = loop;
	state_ = state;
	state_timer_ = 0.0f;
}

//移動処理
void Player::move(float delta_time) {

	//スペースキーで弾を撃つ
	if (gsGetKeyState(GKEY_SPACE)) {
		//攻撃状態に遷移する
		change_state(State::Attack, MotionFire);
		
		return;
	}

	//カメラの前方向ベクトルを取得
	GSvector3 forward = world_->camera()->transform().forward();
	forward.y = 0.0f;
	//カメラの右方向ベクトルを取得
	GSvector3 right = world_->camera()->transform().right();
	right.y = 0.0f;
	//キーの入力値から移動ベクトルを計算
	GSvector3 velocity{ 0.0f,0.0f,0.0f };
	if (gsGetKeyState(GKEY_W)) velocity +=  forward;
	if (gsGetKeyState(GKEY_S)) velocity += -forward;
	if (gsGetKeyState(GKEY_A)) velocity += -right;
	if (gsGetKeyState(GKEY_D)) velocity +=  right;
	velocity = velocity.normalized() * WalkSpeed * delta_time;

	//移動してなければアイドル状態
	GSuint motion{ MotionIdle };
	//移動しているか？
	if (velocity.length() != 0.0f) {
		//向きの補間
		GSquaternion rotation =
			GSquaternion::rotateTowards(
				transform_.rotation(),
				GSquaternion::lookRotation(velocity), 12.0f * delta_time);
		transform_.rotation(rotation);

		//移動中のモーションにする
		motion = MotionForwardWalk;
	}
	
	//モーションの変更
	change_state(State::Move, motion);

	//移動量のxz成分だけ更新
	velocity_.x = velocity.x;
	velocity_.z = velocity.z;

	//平行移動する(ローカル座標系基準)
	transform_.translate(velocity_, GStransform::Space::World);
	
}

//攻撃中
void Player::attack(float delta_time) {
	//攻撃モーションの終了を待つ
	if (state_timer_ >= mesh_.motion_end_time()) {
		move(delta_time);
	}
}

//ダメージ中
void Player::damage(float delta_time) {
	//ノックバックする(ワールド座標系)
	transform_.translate(velocity_ * delta_time, GStransform::Space::World);
	//減速させる
	velocity_ -= GSvector3{ velocity_.x,0.0f,velocity_.z } * 0.5f * delta_time;

	//ダメージモーションの終了を待つ
	if (state_timer_ >= mesh_.motion_end_time()) {
		move(delta_time);
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
	}

	//地面との衝突判定(線分との交差判定)
	GSvector3 position = transform_.position();
	Line line;
	line.start	= position + collider_.center;
	line.end = position + GSvector3{ 0.0f,-FootOffset, 0.0f };
	GSvector3 intersect;	//地面との交点

	if (world_->field()->collide(line, &intersect)) {
		//交点の位置からy座標のみ補正する
		position.y = intersect.y;
		//座標を変更する
		transform_.position(position);
		//重力を初期化する
		velocity_.y = 0.0f;
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
	float length = collider_.radius + other.collider().radius;
	//衝突判定球の重なっている長さを求める
	float overlap = length - distance;
	//重なっている部分の半分の距離だけ離れる移動量を求める
	GSvector3 v = (position - target).getNormalized() * overlap * 0.5f;
	transform_.translate(v, GStransform::Space::World);
	//フィールドとの衝突判定
	collide_field();
}

