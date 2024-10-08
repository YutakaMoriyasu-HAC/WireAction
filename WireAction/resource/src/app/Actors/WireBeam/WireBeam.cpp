#include "WireBeam.h"
#include"app/NameList/Assets.h"
#include"app/Worlds/IWorld.h"
//エフェクト用インクルード
#include"app/Effect/FollowEffect.h"
#include "app/Actors/Player/Player.h"
#include "app/NameList/PlayerStateList.h"
#include "app/Math/Math.h"
#include "app/Input/InputManager.h"
#include "app/Field/Field.h"


WireBeam::WireBeam(IWorld* world, const GSvector3& position, const GSvector3& speed, const std::string& name, float lifeTime, float size, GSquaternion qua, Player* player) :
	Actor(),player_(player)
{
	//ワールドの設定
	world_ = world;
	name_ = name;
	tag_ = "AttackColliderTag";
	ownerTag_ = "PlayerTag";
	transform_.position(position);
	time_ = lifeTime;
	timer_ = lifeTime;
	speed_ = speed;
	speed_.y = 0;
	size_ = size;
	//衝突判定球の設定
	collider_ = BoundingSphere{ size_ };
	transform_.rotation(qua);
	//player_ = player;

	//距離10
	goalPoint_ = transform_.position() + (speed_*10);

	flagRotation = false;


}

void WireBeam::update(float delta_time)
{
	


	if (!flagRotation) {
		//transform_.position(transform_.position() + speed_ * (delta_time / 60) * size_ * 5 * 5.0f);
		collider_ = BoundingSphere{ size_ };

		GSvector3 posHead = player_->transform().position();
		posHead.y += 1.5f;
		if (!turnFlag_) {
			move_to(goalPoint_, time_).name("wire").ease(EaseType::EaseOutQuint);
		}
		else {
			move_to(posHead, time_ / 2).name("wire").ease(EaseType::EaseOutQuad);
		}

		timer_ -= delta_time;
		if (timer_ <= 0) {
			if (!turnFlag_) {
				Tween::cancel("wire");
				timer_ = time_;
				turnFlag_ = true;
				return;

			}
			else {
				Tween::cancel("wire");
				player_->setThrowing(false);
				die();
			}

		}

		if (!turnFlag_ && timer_ > time_ - 6)return;
		if (InputManager::IsXButtonTrigger()) {
			flagRotation = true;
			Tween::cancel("wire");
			timer_ = 3;
			rotateAngle_ = atan2f(goalPoint_.z - posHead.z, goalPoint_.x - posHead.x) * (180 / math::PI);
			rotateDistance_ = 0.0f;
		}
	}
	else {

		timer_ += 0.3f;

		collider_ = BoundingSphere{ size_*2 };

		if (timer_ > 10) {
			timer_ = 30;
		}

		




		//入力
		rotateAngle_ += 23;
		rotateDistance_ += 23;

		//rotateAngle_.y = 25;
		if (rotateAngle_ < 0) rotateAngle_ += 360;
		if (rotateAngle_ > 360) rotateAngle_ -= 360;
		rotateAngleE_ = rotateAngle_ * (math::PI / 180);

		GSvector3 position;
		position.x = player_->transform().position().x + cosf(rotateAngleE_) * 10;
		position.z = player_->transform().position().z + sinf(rotateAngleE_) * 10;
		position.y = transform_.position().y;
		transform_.position(position);

		if (rotateDistance_ >= 360) {
			flagRotation = false;
			turnFlag_ = true;
		}
		
	}

	//フィールドとの衝突判定
	collide_field();

}

void WireBeam::draw() const
{
	//メッシュを描画
	collider().draw();

	GSvector2 startPoint;
	GSvector2 endPoint;
	GSvector3 handPosition = player_->transform().position();
	handPosition.y += 0.5f; //手の位置のオフセット
	GSvector3 centerPosition = transform_.position();

	//それぞれの座標を画面座標に変換
	gsCalculateScreen(&startPoint, &handPosition);
	gsCalculateScreen(&endPoint, &centerPosition);

	//画面座標がマイナスにならないようにする
	if (startPoint.x > 10000) {
		startPoint.x -= 20000.0f;
	}
	/*if (startPoint.y< 0) {
		startPoint.y += 1080.0f;
	}*/


	//gsDrawText("s%f\n", startPoint.x);
	//gsDrawText("\ne%f", endPoint.x);

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
	//gsDrawSprite3D(Texture_Wire, &position_, &body, NULL, &color, &scale, 0.0f);
	
}

void WireBeam::react(Actor& other)
{

	if (other.name() == "Ball") {
		GSvector3 ballPosition = other.transform().position();
		GSvector3 pPosition = player_->transform().position();
		float distance = sqrtf(( ballPosition.x - pPosition.x) * (ballPosition.x - pPosition.x) +
								(ballPosition.y - pPosition.y) * (ballPosition.y - pPosition.y) +
								(ballPosition.z - pPosition.z) * (ballPosition.z - pPosition.z));
		if (distance > 10)return;

		Tween::cancel("wire");
		player_->setCenterPendulum(other.transform().position());
		player_->changeState(PlayerStateList::State_Pendulum);
		player_->setThrowing(false);
		die();
	}

	if (other.name() == "Player" && turnFlag_) {
		Tween::cancel("wire");
		player_->setThrowing(false);
		die();
	}
	
	if (other.tag() == "EnemyTag") {
		Tween::cancel("wire");
		player_->setCenterPendulum(other.transform().position());
		player_->changeState(PlayerStateList::State_QuickWire);
		player_->setThrowing(false);
		die();
	}



}

void WireBeam::collide_field() {
	//壁との衝突判定(球体との判定)
	GSvector3 center;	//衝突後の球体の中心座標
	if (world_->field()->collide(collider(), &center)) {
		
		GSvector3 ballPosition = center;
		GSvector3 pPosition = player_->transform().position();
		float distance = sqrtf((ballPosition.x - pPosition.x) * (ballPosition.x - pPosition.x) +
			(ballPosition.y - pPosition.y) * (ballPosition.y - pPosition.y) +
			(ballPosition.z - pPosition.z) * (ballPosition.z - pPosition.z));
		if (distance > 10)return;



		Tween::cancel("wire");
		player_->setCenterPendulum(center);
		player_->changeState(PlayerStateList::State_QuickWire);
		player_->setThrowing(false);
		die();
	}
}
