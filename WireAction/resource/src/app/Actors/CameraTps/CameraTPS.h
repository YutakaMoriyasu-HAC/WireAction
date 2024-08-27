#ifndef CAMERA_TPS_H_
#define CAMERA_TPS_H_

#include "app/Actors/Actor/Actor.h"

class CameraTps :public Actor
{
public:
	//コンストラクタ
	CameraTps(IWorld* world, const GSvector3& position, const GSvector3& at);
	//更新
	virtual void update(float delta_time)override;
	//描画
	virtual void draw()const override;

private:
	GSvector3 playerOffset_{ 0,3,8 };

	GSvector2 cameraAngle_{ 90,30 };
	GSvector2 cameraAngleE_{ 0,0 };

	bool startFlag_{ false };

	int cameraLevel_{ 0 };

	//前のフレームの注視点
	GSvector3 prevPlayerLookPos_{ 0,3,5 };

	//補正した注視点
	GSvector3 smoothPlayerLookPos_{ 0,0,0 };

	
};

#endif 

