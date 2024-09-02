#ifndef PLAYER_MOVE_STATE_H_
#define PLAYER_MOVE_STATE_H_

#include <memory>

#include "app/StateMachine/StateBase.h"
#include "app/Actors/Player/Player.h"

// プレイヤーの移動処理クラス
class PlayerMoveState : public app::StateBase
{
public:
	// コンストラクタ
	PlayerMoveState(Player* parent, IWorld* world, app::StateMachine* stateMachine);
	// デストラクタ
	~PlayerMoveState() = default;
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
	void changeAngle(float time=12.0f);



private:
	bool isDash_;
private:
	Player* parent_;
	IWorld* world_;
	GSvector3 position_;

	//加速度
	float moveSpeed_{ 0.5f };

	//速度
	GSvector3 velocity_{0.0f,0.0f,0.0f};

	//このステートが始まった時のスピ―ド、通称SSS
	float stateStartSpeed{ 0.0f };

	enum SpeedState {
		Stop,
		SpeedUp,
		SpeedDown,
		brake
	};

	SpeedState SState_{ Stop };

	GSvector3 my_Input_Direction_{ 0,0,0 };

	//カメラの注視点(ジャンプで変わるため変数にしておく)
	GSvector3 cameraLookPoint_{ 0,0,0 };

	//前のフレームの向き
	float previousAngle_{ 0.0f };

	//前のフレームの場所
	GSvector3 previousPosition_{ 0,0,0 };

	//低速ころころ
	bool rowSpeedKoroKoro_{ false };
};


#endif