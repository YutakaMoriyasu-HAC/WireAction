#include "Ball.h"
#include "app/NameList/Assets.h"

Ball::Ball(IWorld* world, const GSvector3& position) :mesh_{ Mesh_Ball, 0, true } {
	//座標の初期化
	transform_.position(position);
	//メッシュの変換行列を初期化
	mesh_.transform(transform_.localToWorldMatrix());
	//タグ
	tag_ = "WireObjectTag";
	name_ = "Ball";
	//衝突判定球の設定
	collider_ = BoundingSphere{ 1.0f };
}

void Ball::update(float delta_time) {



}

void Ball::draw() const {
	mesh_.draw();
	collider().draw();
}