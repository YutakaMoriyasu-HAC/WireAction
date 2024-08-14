#ifndef PLAYER_JUMP_STATE_H_
#define PLAYER_JUMP_STATE_H_

#include <memory>

#include "app/StateMachine/StateBase.h"
#include "app/Actors/Player/Player.h"

// プレイヤーの移動処理クラス
class PlayerJumpState : public app::StateBase
{
public:
	// コンストラクタ
	PlayerJumpState(Player* parent, IWorld* world, app::StateMachine* stateMachine);
	// デストラクタ
	~PlayerJumpState() = default;
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

	//移動速度
	float moveSpeed_{ 0.5f };

	//速度
	GSvector3 velocity_{ 0.0f,0.0f,0.0f };

	//このステートが始まった時のスピ―ド、通称SSS
	float stateStartSpeed{ 0.0f };
	//このステートが始まった時のベクトル
	GSvector3 stateStartVec{ 0.0f,0.0f,0.0f };

	//現在の移動方向から入力方向までのベクトル
	GSvector3 velocityToInputVector_{ 0.0f,0.0f,0.0f };

	int jumpNum_{ 1 }; //残りジャンプ回数

	//ジャンプボタンを離したらtrue
	bool buttonReleaseFlag_{ false };
	


	GSvector3 my_Input_Direction_{ 0,0,0 };

	//カメラの注視点(ジャンプで変わるため変数にしておく)
	GSvector3 cameraLookPoint_{ 0,0,0 };
};


#endif