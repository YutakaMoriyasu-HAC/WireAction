#ifndef PLAYER_THROW_WIRE_STATE_H_
#define PLAYER_THROW_WIRE_STATE_H_

#include <memory>

#include "app/StateMachine/StateBase.h"
#include "app/Actors/Player/Player.h"

// プレイヤーの移動処理クラス
class PlayerThrowWireState : public app::StateBase
{
public:
	// コンストラクタ
	PlayerThrowWireState(Player* parent, IWorld* world, app::StateMachine* stateMachine);
	// デストラクタ
	~PlayerThrowWireState() = default;
	// 初期化
	void init() override;
	// 終了
	void final() override;
	// 更新
	void update() override;
	void lateUpdate()override;
	// 描画
	void draw() const override;
	void lateDraw()const override;



private:
	void changeAngle();



private:
	Player* parent_;
	IWorld* world_;
	GSvector3 position_;





	GSvector3 my_Input_Direction_{ 0,0,0 };


};


#endif