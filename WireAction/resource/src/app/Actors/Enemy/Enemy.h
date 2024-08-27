#ifndef ENEMY_H_
#define ENEMY_H_

#include "app/Actors/Actor/Actor.h"
#include "app/AnimatedMesh/AnimatedMesh.h"

class Enemy :public Actor
{
public:
	//コンストラクタ
	Enemy(IWorld* world, const GSvector3& position);

	virtual void update(float delta_time) override;
	//描画
	virtual void draw() const override;

	//衝突リアクション
	virtual void react(Actor& other) override;

private:
	//アニメーションメッシュ
	AnimatedMesh	mesh_;
	//モーション番号
	GSuint			motion_;

	//モーションのループ指定
	bool			motion_loop_;
	float timer_{ 0.0f };

};

#endif 

