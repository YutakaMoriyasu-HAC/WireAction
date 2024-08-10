#ifndef LIGHT_H_
#define LIGHT_H_

#include "app/Actors/Actor/Actor.h"

//ライトクラス
class Light : public Actor {
public:
	//コンストラクタ
	Light(IWorld* world);
	//描画
	virtual void draw() const override;
};

#endif;