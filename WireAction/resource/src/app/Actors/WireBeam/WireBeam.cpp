#include "WireBeam.h"
#include"app/NameList/Assets.h"
#include"app/Worlds/IWorld.h"
//�G�t�F�N�g�p�C���N���[�h
#include"app/Effect/FollowEffect.h"
#include "app/Actors/Player/Player.h"
#include "app/NameList/PlayerStateList.h"


WireBeam::WireBeam(IWorld* world, const GSvector3& position, const GSvector3& speed, const std::string& name, float lifeTime, float size, GSquaternion qua, Player* player) :
	Actor(),player_(player)
{
	//���[���h�̐ݒ�
	world_ = world;
	name_ = name;
	tag_ = "AttackColliderTag";
	ownerTag_ = "PlayerTag";
	transform_.position(position);
	time_ = lifeTime;
	speed_ = speed;
	speed_.y = 0;
	size_ = size;
	//�Փ˔��苅�̐ݒ�
	collider_ = BoundingSphere{ size_ };
	transform_.rotation(qua);
	//player_ = player;


}

void WireBeam::update(float delta_time)
{
	transform_.position(transform_.position() + speed_ * (delta_time / 60) * size_ * 5 * 5.0f);
	collider_ = BoundingSphere{ size_ };



	//�������Ԃ��؂ꂽ�玀
	time_ -= delta_time / 60;
	if (time_ <= 0) {

		die();
	}

}

void WireBeam::draw() const
{
	//���b�V����`��
	collider().draw();

	
}

void WireBeam::react(Actor& other)
{

	if (other.name() == "Ball") {
		player_->changeState(PlayerStateList::State_Pendulum);
		die();
	}
	


}
