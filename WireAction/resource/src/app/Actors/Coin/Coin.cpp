#include "Coin.h"
#include "app/NameList/Assets.h"
#include "app/Input/GameManager.h"

Coin::Coin(IWorld* world, const GSvector3& position) :mesh_{ Mesh_Coin, 0, true } {
	//���W�̏�����
	transform_.position(position);
	//���b�V���̕ϊ��s���������
	mesh_.transform(transform_.localToWorldMatrix());

	//�^�O
	tag_ = "ItemTag";
	name_ = "Coin";
	//�Փ˔��苅�̐ݒ�
	collider_ = BoundingSphere{ 0.5f };
}

void Coin::update(float delta_time) {

	if (texture2D_) {

		timer_++;
		if (timer_ > 4) {
			animPatarn_++;
			timer_ = 0;
		}
		if (animPatarn_ > 5) {
			animPatarn_ = 0;
		}
	}
	else {
		transform_.rotate(0.0f, 8.0f * delta_time, 0.0f);
		mesh_.update(delta_time);
		//���b�V���̕ϊ��s���������
		mesh_.transform(transform_.localToWorldMatrix());
	}

	

}

void Coin::draw() const {

	//collider().draw();

	if (texture2D_) {
		static const GSrect body{ 0.3f, -0.3f, -0.3f, 0.3f };
		GScolor color = { 1.0f, 250.0f, 250.0f, 0.0f };
		static const GSvector2 scale{ 1.5f, 1.5f };

		float pixelScale = 1.0f;
		GSrect sourceRect{ 32 * animPatarn_ * pixelScale + 0.5f,
							32 * pixelScale,
							32 * (animPatarn_ + 1) * pixelScale - 0.5f,
							0 };

		GSvector3 position = transform_.position();

		gsDrawSprite3D(Texture_Coin, &position, &body, &sourceRect, &color, &scale, 0.0f);
	}
	else {

		

		mesh_.draw();
	}

}

void Coin::react(Actor& other) {

	if (other.name() == "Player") {

		GameManager::CoinPlus(1);
		die();

	}
}