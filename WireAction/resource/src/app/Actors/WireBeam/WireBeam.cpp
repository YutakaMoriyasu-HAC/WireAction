#include "WireBeam.h"
#include"app/NameList/Assets.h"
#include"app/Worlds/IWorld.h"
//�G�t�F�N�g�p�C���N���[�h
#include"app/Effect/FollowEffect.h"
#include "app/Actors/Player/Player.h"
#include "app/NameList/PlayerStateList.h"
#include "app/Math/Math.h"


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
	timer_ = lifeTime;
	speed_ = speed;
	speed_.y = 0;
	size_ = size;
	//�Փ˔��苅�̐ݒ�
	collider_ = BoundingSphere{ size_ };
	transform_.rotation(qua);
	//player_ = player;

	goalPoint_ = transform_.position() + (speed_*10);


}

void WireBeam::update(float delta_time)
{
	//transform_.position(transform_.position() + speed_ * (delta_time / 60) * size_ * 5 * 5.0f);
	collider_ = BoundingSphere{ size_ };

	GSvector3 posHead = player_->transform().position();
	posHead.y += 1.5f;
	if (!turnFlag_) {
		move_to(goalPoint_, time_).name("wire").ease(EaseType::EaseOutQuint);
	}
	else {
		move_to(posHead, time_/2).name("wire").ease(EaseType::EaseOutQuad);
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
			die();
		}
		
	}

}

void WireBeam::draw() const
{
	//���b�V����`��
	collider().draw();

	GSvector2 startPoint;
	GSvector2 endPoint;
	GSvector3 handPosition = player_->transform().position();
	handPosition.y += 1.5f; //��̈ʒu�̃I�t�Z�b�g
	GSvector3 centerPosition = transform_.position();

	//���ꂼ��̍��W����ʍ��W�ɕϊ�
	gsCalculateScreen(&startPoint, &handPosition);
	gsCalculateScreen(&endPoint, &centerPosition);

	//��ʍ��W���}�C�i�X�ɂȂ�Ȃ��悤�ɂ���
	if (startPoint.x > 10000) {
		startPoint.x -= 20000.0f;
	}
	/*if (startPoint.y< 0) {
		startPoint.y += 1080.0f;
	}*/


	//gsDrawText("s%f\n", startPoint.x);
	//gsDrawText("\ne%f", endPoint.x);

	//�摜�̒��S����
	static const GSvector2 center{ 16.0f,0.0f };
	//�F
	GScolor color = { 1.0f, 1.0f, 1.0f, 1.0f };

	float nagasa = sqrtf((endPoint.y - startPoint.y) * (endPoint.y - startPoint.y) + (endPoint.x - startPoint.x) * (endPoint.x - startPoint.x));
	GSvector2 end_scale{ 0.5f, nagasa / 32.0f };

	GSvector2 position = GSvector2(endPoint);

	//�摜�̐؂蔲��
	static const GSrect Rect{ 0.0f, 0.0f, 32.0f,32.0f };

	//�p�x����
	float drawAngle = atan2f(endPoint.y - startPoint.y, endPoint.x - startPoint.x) * (180 / math::PI) + 90;

	gsDrawSprite2D(Texture_Wire, &position, &Rect, &center, &color, &end_scale, drawAngle);
	//gsDrawSprite3D(Texture_Wire, &position_, &body, NULL, &color, &scale, 0.0f);
	
}

void WireBeam::react(Actor& other)
{

	if (other.name() == "Ball") {
		Tween::cancel("wire");
		player_->setCenterPendulum(other.transform().position());
		player_->changeState(PlayerStateList::State_Pendulum);
		die();
	}

	if (other.name() == "Player" && turnFlag_) {
		Tween::cancel("wire");
		die();
	}
	


}
