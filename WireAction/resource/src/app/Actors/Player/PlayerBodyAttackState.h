#ifndef PLAYER_BODYATTACK_STATE_H_
#define PLAYER_BODYATTACK_STATE_H_

#include <memory>

#include "app/StateMachine/StateBase.h"
#include "app/Actors/Player/Player.h"

// プレイヤーの移動処理クラス
class PlayerBodyAttackState : public app::StateBase
{
public:
	// コンストラクタ
	PlayerBodyAttackState(Player* parent, IWorld* world, app::StateMachine* stateMachine);
	// デストラクタ
	~PlayerBodyAttackState() = default;
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

	//カメラの注視点(ジャンプで変わるため変数にしておく)
	GSvector3 cameraLookPoint_{ 0,0,0 };
	
};

#endif