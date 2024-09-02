#include "app/Actors/Actor/Actor.h"

//更新
void Actor::update(float) {}

//遅延更新
void Actor::lateUpdate(float) {}

//描画
void Actor::draw() const {}

//半透明オブジェクトの描画
void Actor::draw_transparent() const {}

//GUIの描画
void Actor::draw_gui() const {}

//衝突リアクション
void Actor::react(Actor&) {}

//メッセージ処理
void Actor::handle_message(const std::string& message, void*param) {}

//衝突判定
void Actor::collide(Actor& other) {
	//どちらのアクターも衝突判定が有効か？
	if (enable_collider_ && other.enable_collider_) {
		//衝突判定をする
		if (is_collide(other)) {
			//衝突した場合は、お互いに衝突リアクションをとる
			react(other);
			other.react(*this);
		}
	}
}

//線分との衝突判定
bool Actor::collide(const Line& line, GSvector3* intersect) const {
	return collider().intersects(line, intersect);
}

//アクターとの衝突処理
void Actor::collide_actor(Actor& other) {
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
	transform_.translate(-v, GStransform::Space::World);
	//フィールドとの衝突判定
	//collide_field();
}

//死亡する
void Actor::die() {
	dead_ = true;
}

//衝突しているか？
bool Actor::is_collide(const Actor& other) const {
	return collider().intersects(other.collider());
}

//死亡しているか？
bool Actor::is_dead() const {
	return dead_;
}

//名前を取得
const std::string& Actor::name() const {
	return name_;
}

//タグ名を取得
const std::string& Actor::tag() const {
	return tag_;
}

//攻撃状態を取得
const bool& Actor::attackState() const {
	return attackState_;
}

//トランスフォームを取得(const版)
const GStransform& Actor::transform() const {
	return transform_;
}

//トランスフォームを取得
GStransform& Actor::transform() {
	return transform_;
}

// 移動量の取得
const GSvector3& Actor::velocity() const {
	return velocity_;
}

GSvector3& Actor::velocity()
{
	return velocity_;
}

void Actor::velocity(const GSvector3& vec)
{
	velocity_ = vec;
}

//衝突判定データを取得
BoundingSphere Actor::collider() const {
	return collider_.transform(transform_.localToWorldMatrix());
}

TweenUnit& Actor::move_to(const GSvector3& to, float duration) {
	// 現在の場所から指定された場所まで、Tweenで移動する
	return Tween::vector3(transform_.position(), to, duration,
		[=](GSvector3 pos) {transform_.position(pos); });
}

void Actor::gravityFall(float delta_time) {
	//重力値を更新
	velocity_.y += Gravity * delta_time;
}