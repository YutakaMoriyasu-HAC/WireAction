#ifndef BALL_H_
#define BALL_H_

#include "app/Actors/Actor/Actor.h"
#include "app/AnimatedMesh/AnimatedMesh.h"

class Ball :public Actor
{
public:
	//コンストラクタ
	Ball(IWorld* world, const GSvector3& position);

	virtual void update(float delta_time) override;
	//描画
	virtual void draw() const override;

private:
	//アニメーションメッシュ
	AnimatedMesh	mesh_;

};

#endif 

