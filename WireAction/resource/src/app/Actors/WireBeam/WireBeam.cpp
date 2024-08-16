#include "WireBeam.h"
#include"app/NameList/Assets.h"
#include"app/Worlds/IWorld.h"
//エフェクト用インクルード
#include"app/Effect/FollowEffect.h"
#include "app/Actors/Player/Player.h"
#include "app/NameList/PlayerStateList.h"


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
	speed_ = speed;
	speed_.y = 0;
	size_ = size;
	//衝突判定球の設定
	collider_ = BoundingSphere{ size_ };
	transform_.rotation(qua);
	//player_ = player;


}

void WireBeam::update(float delta_time)
{
	transform_.position(transform_.position() + speed_ * (delta_time / 60) * size_ * 5 * 5.0f);
	collider_ = BoundingSphere{ size_ };



	//生存時間が切れたら死
	time_ -= delta_time / 60;
	if (time_ <= 0) {

		die();
	}

}

void WireBeam::draw() const
{
	//メッシュを描画
	collider().draw();

	
}

void WireBeam::react(Actor& other)
{

	if (other.name() == "Ball") {
		player_->changeState(PlayerStateList::State_Pendulum);
		die();
	}
	


}
