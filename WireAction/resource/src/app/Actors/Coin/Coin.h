#ifndef COIN_H_
#define COIN_H_

#include "app/Actors/Actor/Actor.h"
#include "app/AnimatedMesh/AnimatedMesh.h"

class Coin :public Actor
{
public:
	//コンストラクタ
	Coin(IWorld* world, const GSvector3& position);

	virtual void update(float delta_time) override;
	//描画
	virtual void draw() const override;

	//衝突リアクション
	virtual void react(Actor& other) override;

private:
	//アニメーションメッシュ
	AnimatedMesh	mesh_;

	float timer_{ 0.0f };
	int animPatarn_{ 0 };

	bool texture2D_{ false };

};

#endif 

