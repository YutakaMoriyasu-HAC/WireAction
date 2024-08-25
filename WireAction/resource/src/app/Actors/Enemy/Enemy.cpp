#include "Enemy.h"
#include "app/NameList/Assets.h"
#include "EnemyMotionList.h"

Enemy::Enemy(IWorld* world, const GSvector3& position) :
	mesh_{ Mesh_EnemyBird, 0, true },
	motion_{ EnemyMotionList::Motion_Idle }, 
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
	collider_ = BoundingSphere{ 0.5f };
}

void Enemy::update(float delta_time) {
	//モーションを変更
	mesh_.change_motionS(motion_, motion_loop_,1.0f);
	mesh_.update(delta_time);
	//メッシュの変換行列を初期化
	mesh_.transform(transform_.localToWorldMatrix());

}

void Enemy::draw() const {
	mesh_.draw();
	collider().draw();
	
}

void Enemy::react(Actor& other) {
	if (other.name() == "Player") {
		transform_.localScale(GSvector3(2, 0.4, 2));
		
	}
}