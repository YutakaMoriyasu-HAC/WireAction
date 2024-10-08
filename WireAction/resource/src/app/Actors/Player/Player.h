#ifndef PLAYER_H_
#define PLAYER_H_

#include "app/Actors/Actor/Actor.h"
#include "app/AnimatedMesh/AnimatedMesh.h"
#include "app/StateMachine/StateMachine.h"
#include"app/Effect/FollowEffect.h"

//プレイヤークラス
class Player : public Actor {


public:
	//コンストラクタ
	Player(IWorld* world = nullptr, const GSvector3& position = GSvector3{ 0.0f,0.0f,0.0f });
	//更新
	virtual void update(float delta_time) override;
	virtual void lateUpdate(float delta_time)override;
	//描画
	virtual void draw() const override;
	//衝突リアクション
	virtual void react(Actor& other) override;
	// モーションが終了しているか
	const bool IsMotionEnd() const;
	// モーションを変更
	void ChangeMotionS(GSuint motion, bool loopFlag = true, float speed = 1, float lerp = 0.5f, float endTime = 0.0f, float startTime = 0.0f);
	// モーションの終了時間を取得
	const float GetMotionEndTime() const;
	//モーションがループするか
	const bool IsMotionRoop() const;
	//モーション時間をリセット
	void motionTimeReset();

	//プレイヤーの座標を他のクラスに受け渡すときの関数
	const GSvector3 GetPosition() const;
	//他のクラスから受け取った座標を変数にする関数
	void SetPosition(GSvector3 pos);

	//現在のモーション番号を返す
	const int GetMotionState() const;

	//最後に向いていた方向を返す
	const GSvector3 GetInputDirection() const;

	//最後に向いていた方向をセット
	void SetInputDirection(GSvector3 direction);

	//input_を返す
	const GSvector3 GetInput()const;
	//ステート変更
	void changeState(const int state);

	//地面についてる？
	bool isGround();
	bool isWall();

	//注視点取得
	GSvector3 getCameraLookPoint();

	//注視点セット
	void setCameraLookPoint(GSvector3 point);

	void setDebugFloat(int num, float value);

	//プレイヤークラスから召喚
	void spone(int actorListNum);

	//ビームを出す方向を決定
	void setBeamDirection(GSvector3 direction);

	


	//振り子の中心をセット
	void setCenterPendulum(GSvector3 center);
	//振り子の中心を取得
	GSvector3 getCenterPendulum();

	void setDebugMoveSpeed(float speed);

	void setThrowing(bool throwing);
	bool getThrowing();

	void resetStateTimer();

	//他のコライダーとぶつかっているか取得
	bool isCollide();

	//デバッグ用
	void SetDebugFloat(float a = 0, float b = 0, float c = 0, float d = 0, float e = 0);

	//
	void changeMesh(GSuint model);

	//敵を踏みつぶしたか
	bool isTrampled(int trampleSwitch = -1);

	//壁キック可能なら壁の座標を返す
	bool canWallKick();

	//最後にいた足場をセット
	void setLastPosition(GSvector3 position);

	//無敵時間さん
	void startInvincibilityTime();

	//空中ジャンプ回数-1
	bool canAirJump();



private:


	//ダメージ中
	void damage(float delta_time);

	//フィールドとの衝突処理
	void collide_field();
	

	// ステートの初期化
	void InitState();


	//入力情報更新
	void ControllerUpdate();

	// 移動入力
	GSvector3 MoveInput();



public:
	GSvector3 input_ = { 0,0,0 };	//入力1
	GSvector3 input2_ = { 0,0,0 };	//入力2(1フレーム前の入力)

private:
	//アニメーションメッシュ
	AnimatedMesh	mesh_;
	//モーション番号we
	GSuint			motion_;
	//状態タイマ
	float			state_timer_;

	// 状態
	app::StateMachine stateMachine_;
	//モーションの開始時間
	float startTime_{ 0.0f };
	//モーションのループ指定
	bool motionLoop_;
	//アニメ再生速度
	float animSpeed_{ 1.0f };
	//ラープ倍率
	float lerpSize_{ 0.5f };
	//いつもよりもさらに速くモーションが終了しますよ
	float motionEndTimePlus_{ 0.0f };
	//最後に向いていた方向(ここの値は最初に向いている方向)
	GSvector3 input_Direction_{ 0,0,1 };

	bool isGround_{ false };
	bool isWall_{ false };

	//カメラの注視点(ジャンプで変わるため変数にしておく)
	GSvector3 cameraLookPoint_{ 0,0,0 };

	float debugFloat_[7]{ 0.0f,0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };

	GSvector3 beamDirection_{ GSvector3::zero() };

	//地面と衝突判定するか
	bool canCollideField_{ true };

	float footOffset_{ -1.0f };

	GSvector3 centerPendulum_{ GSvector3::zero() };

	float debugMoveSpeed_{ 0.0f };

	//ワイヤーを投げているか
	bool isThrowing_{ false };

	//他のオブジェクトとぶつかっているか
	bool isCollide_{ false };

	//踏みつぶしたか
	bool trampled_{ false };

	//壁キック可能
	bool canWallKick_{ false };
	//壁キック座標
	GSvector3 wallKickPosition_{0, 0, 0};

	//最後にいた座標
	GSvector3 lastPosition_{ 0,0,0 };

	//無敵かどうか
	bool isInvincible_{ false };
	float invincibleTimer_{ 0.0f };

	//法線p
	GSvector3 p{ 0,0,0 };

	//空中ジャンプ回数
	int airJumpNum_{ 1 };

	//エフェクト
	Effect* effectGetCoin_;
};

#endif