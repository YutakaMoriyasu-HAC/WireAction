#include "Enemy.h"
#include "app/NameList/Assets.h"
#include "EnemyMotionList.h"
#include "app/Worlds/IWorld.h"
#include "app/Math/Math.h"
#include "app/Field/Field.h"

Enemy::Enemy(IWorld* world, const GSvector3& position) :
	mesh_{ Mesh_EnemyBird, 0, true },
	motion_{ EnemyMotionList::Motion_Run }, 
	motion_loop_{true} {
	//座標の初期化
	transform_.position(position);
	transform_.localScale(GSvector3(2, 2, 2));
	//メッシュの変換行列を初期化
	mesh_.transform(transform_.localToWorldMatrix());
	//タグ
	tag_ = "EnemyTag";
	name_ = "EnemyBird";
	//衝突判定球の設定
	collider_ = BoundingSphere{ 0.4f , GSvector3{0.0f,0.5f,0.0f} };
	world_ = world;
	player_ = world_->find_actor("Player");
	velocity_ = GSvector3::zero();
}

void Enemy::update(float delta_time) {
	//モーションを変更
	mesh_.change_motionS(motion_, motion_loop_,1.0f);
	mesh_.update(delta_time);
	//メッシュの変換行列を初期化
	mesh_.transform(transform_.localToWorldMatrix());

	if (timer_ > 0) {
		timer_--;
		if (timer_ <= 0) {
			die();
		}
		return;
	}

	//フィールドとの衝突判定
	collide_field();
	//自然落下
	gravityFall(delta_time);
	
	targetPosition_ = player_->transform().position();

	//距離が離れたら止まる
	if (math::radiusFromVector2(targetPosition_ - transform_.position()) >= 15.0f) {

		if (motion_ == Motion_Run) {
			motion_=Motion_Idle;
			velocity_ = GSvector3::zero();
		}
	}
	else {
		//ここから走り出す
		if (motion_ == Motion_Idle) {
			motion_ = Motion_Run;
		}

		changeAngle(10.0f);
		GSvector3 directionToPlayer = targetPosition_ - transform_.position();
		direction_ = directionToPlayer * math::normal(directionToPlayer.x, directionToPlayer.z) * 0.1f;
		velocity_ = GSvector3(direction_.x, velocity_.y, direction_.z);
	}

	//移動
	transform_.position(transform_.position() + velocity_);

	//奈落に落ちたとき
	if (transform_.position().y < -30.0f) {
		die();
	}

	

}

void Enemy::draw() const {
	mesh_.draw();
	collider().draw();
	
}

//オブジェクト同士の衝突
void Enemy::react(Actor& other) {

	if (other.name() == "Player" && other.attackState()) {
		
		transform_.localScale(GSvector3(2, 0.4, 2));
		timer_ = 50.0f;
		
	}
	if (other.tag() == "EnemyTag" ) {
		collide_actor(other);
		collide_field();
	}
}

//回転
void Enemy::changeAngle(float time) {
	GSvector3 target = targetPosition_ - transform_.position();
	target.y = 0;
	//向きの補間
	GSquaternion rotation =
		GSquaternion::rotateTowards(
			transform().rotation(),
			GSquaternion::lookRotation(target), time);
	transform().rotation(rotation);
}

//フィールドとの衝突判定
void Enemy::collide_field() {
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
	line.start = position + collider_.center_;
	line.end = position + GSvector3{ 0.0f,-0.5f, 0.0f };;
	GSvector3 intersect;	//地面との交点

	if (world_->field()->collide(line, &intersect)) {
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