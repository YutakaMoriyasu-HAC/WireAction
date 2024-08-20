#ifndef PLAYER_QUICKWIRE_STATE_H_
#define PLAYER_QUICKWIRE_STATE_H_

#include <memory>

#include "app/StateMachine/StateBase.h"
#include "app/Actors/Player/Player.h"

// プレイヤーの移動処理クラス
class PlayerQuickWireState : public app::StateBase
{
public:
	// コンストラクタ
	PlayerQuickWireState(Player* parent, IWorld* world, app::StateMachine* stateMachine);
	// デストラクタ
	~PlayerQuickWireState() = default;
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




private:
	Player* parent_;
	IWorld* world_;
	GSvector3 position_;
	GSvector3 my_Input_Direction_{ 0,0,0 };
	GSvector3 velocity_{ 0.0f,0.0f,0.0f }; //速度
	GSvector3 wirePosition_{ 0,0,0 }; //振り子の中心

	float timer_{ 0.0f };
	float time_{ 60.0f };

};

#endif