#include "Coin.h"
#include "app/NameList/Assets.h"

Coin::Coin(IWorld* world, const GSvector3& position){
	//À•W‚Ì‰Šú‰»
	transform_.position(position);

	//ƒ^ƒO
	tag_ = "ItemTag";
	name_ = "Coin";
	//Õ“Ë”»’è‹…‚ÌÝ’è
	collider_ = BoundingSphere{ 0.5f };
}

void Coin::update(float delta_time) {

	timer_++;
	if (timer_ > 4) {
		animPatarn_++;
		timer_ = 0;
	}
	if (animPatarn_ > 5) {
		animPatarn_ = 0;
	}

}

void Coin::draw() const {

	//collider().draw();

    static const GSrect body{ 0.3f, -0.3f, -0.3f, 0.3f };
    GScolor color = { 1.0f, 250.0f, 250.0f, 0.0f };
    static const GSvector2 scale{ 1.5f, 1.5f };

	float pixelScale = 1.0f;
	GSrect sourceRect{	32 * animPatarn_ * pixelScale+0.5f,
						32 * pixelScale,
						32 * (animPatarn_ + 1) * pixelScale-0.5f,
						0 };

    GSvector3 position = transform_.position();

	gsDrawSprite3D(Texture_Coin, &position, &body, &sourceRect, &color, &scale, 0.0f);

}

void Coin::react(Actor& other) {

	if (other.name() == "Player") {

		die();

	}
}