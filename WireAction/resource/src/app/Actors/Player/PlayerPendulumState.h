#ifndef PLAYER_PENDULUM_STATE_H_
#define PLAYER_PENDULUM_STATE_H_

#include <memory>

#include "app/StateMachine/StateBase.h"
#include "app/Actors/Player/Player.h"

// プレイヤーの移動処理クラス
class PlayerPendulumState : public app::StateBase
{
public:
	// コンストラクタ
	PlayerPendulumState(Player* parent, IWorld* world, app::StateMachine* stateMachine);
	// デストラクタ
	~PlayerPendulumState() = default;
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
	GSvector3 velocity_{ 0.0f,0.0f,0.0f }; //速度
	GSvector3 wirePosition_{ 0,0,0 }; //振り子の中心
	GSvector3 pointH_{ 0,0,0 }; //ワイヤーの支点のY座標だけを主人公に合わせた座標
	GSvector3 thetaZeroPosition_{ 0,0,0 };//Θが0度になる位置
	GSvector3 previousPosition_{ 0,0,0 }; //振り子中の1フレーム前の座標

	//カメラの注視点(ジャンプで変わるため変数にしておく)
	GSvector3 cameraLookPoint_{ 0,0,0 };

	float w_{ 0.0f };//重力[N]
	const float G = 9.8f; //重力加速度
	const float M = 3.5f; //体重
	const float MaxAngleSpeed = 5.0f; //最高回転速度(振り子の回転の最高速度)
	const float ShakeLevel = 3.0f; //振りやすさ(振り子の速度に倍率がかかる)

	float wireX_{0.0f};
	float wireY_{0.0f};
	float wireZ_{0.0f};
	float angle_{ 0.0f };
	float phi_{ 0.0f };
	float wireRadius_{ 0.0f };
	float angleSpeed_{ 0.0f };
	float angleSpeedSpeed_{ 0.0f }; //加速度の加速度
	float angle90_{ 0.0f };
	float sinTheta_{ 0.0f };

	float previousPhi_{ 0.0f };

	bool isWire_{ true }; //ワイヤーが接続されているか

	float upDirection_{ 1.0f }; //上向きなら1、下向きなら-1

};


#endif