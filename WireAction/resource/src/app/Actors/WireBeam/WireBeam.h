#ifndef WIRE_BEAM_H_
#define WIRE_BEAM_H_

#include "app/Actors/Actor/Actor.h"
#include <string>
#include"app/Actors/CameraTps/CameraTps.h"
#include <GStransform.h>
#include <memory>

class Player;

class WireBeam : public Actor
{
public:
	WireBeam(IWorld* world, const GSvector3& position, const GSvector3& speed, const std::string& name, float lifeTime, float size, GSquaternion qua = GSquaternion::identity(), Player* player = nullptr);
	~WireBeam() = default;
	//更新
	virtual void update(float delta_time)override;
	//描画
	virtual void draw()const override;
	//衝突判定
	virtual void react(Actor& other)override;
private:
	//デバックダメージ処理
	bool isTestDamage_{ false };

	float time_;

	GSvector3 speed_{ 0,0,0 };
	float size_{ 0.2f };

	Player* player_;

};

#endif