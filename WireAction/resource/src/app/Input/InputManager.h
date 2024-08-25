#ifndef INPUT_INPUTMANAGER_H_
#define INPUT_INPUTMANAGER_H_

#include <gslib.h>

// 入力管理クラス
static class InputManager
{
public:
	// Lスティックor十字ボタンが入力されたか
	const static GSvector2 IsDirectionalInput();
	// 移動
	const static GSvector2 Move();
	// カメラ移動		
	const static GSvector2 CameraRotation();
	//カメラリセット
	const static bool IsCameraReset();
	//しゃがみ
	const static bool IsCrouchState();
	// 決定ボタンを押したか
	const static bool IsDecisionTrigger();
	// ガードボタンを押したか
	const static bool IsProtectState();
	// 通常攻撃ボタンを押したか
	const static bool IsNormalAttackTrigger();
	// 通常攻撃ボタンを押し続けているか
	const static bool IsNormalAttackState();
	// 通常攻撃ボタンを離したか
	const static bool IsDetachNormalAttackTrigger();
	//回転斬りボタンを押したか
	const static bool IsStrongAttackTrigger();
	//回転斬りボタンを押し続てけているか
	const static bool IsStrongAttackState();
	//回転斬りボタンを離したか？
	const static bool IsDetachStrongAttackTrigger();
	//カメラリセットボタン押したか
	const static bool IsCameraResetTrigger();
	//カメラリセットボタン押し続けたか
	const static bool IsCameraResetState();
	// 回避ボタンを押したか
	const static bool IsRollingTrigger();
	// Bボタン長押しでダッシュ
	const static bool IsDashState();
	// 回避ボタン長押しでもダッシュ
	const static bool IsRollingDashState();
	//右スティック押し込み
	const static bool IsTargetCameraTrigger();
	//右スティックのX軸方向の入力逆転の設定
	static void RightAxisReversX(const bool flag);
	//右スティックのY軸方向の入力逆転の設定
	static void RightAxisReversY(const bool flag);
	//回避でダッシュするか設定
	static void SetRollingDash(const bool flag);
	//カメラ反転か？
	const static bool IsRightAxisReversX();
	const static bool IsRightAxisReversY();
	//回避でダッシュできるか？
	const static bool GetRollingDash();

	//ガードボタンが押されたかどうか？
	const static bool IsSheatheTrigger();
	// ポーズボタンが押されたか
	const static bool IsPauseTrigger();
	// メニューボタンが押されたか
	const static bool IsMenuTrigger();
	// ゲーム終了ボタンが押されたか
	const static bool IsGameEndState();
	// コントローラーの十時左ボタンが押されたか
	const static bool IsLeftButtonPressed();
	// コントローラーの十時右ボタンが押されたか
	const static bool IsRightButtonPressed();
	//コントローラーの十字上ボタンが押されたか
	const static bool IsUpButtonPressed();
	//コントローラーの十字下ボタンが押されたか
	const static bool IsBottomButtonPressed();

	// コントローラーのLTボタンが押されたか
	const static bool IsLeftTriggerButtonPressed();
	//コントローラーの振動開始
	const static void PadVibrationStart(float rightmotor = 1.0f,float leftmotor=1.0f);
	//コントローラーの振動終了
	const static void PadVibrationEnd();


	// Aボタン
	const static bool IsAButtonState();
	const static bool IsAButtonTrigger();
	// Bボタン
	const static bool IsBButtonState();
	const static bool IsBButtonTrigger();
	// Xボタン
	const static bool IsXButtonState();
	const static bool IsXButtonTrigger();
	// Yボタン
	const static bool IsYButtonState();
	const static bool IsYButtonTrigger();

	//左スティックの状態
	const static void GetJoyLeftAxis(int *x, int *y);

	//カメラ感度の設定
	const static void SetCameraSensitivity(float sensitiv);
	//カメラ感度を取得
	const static float GetCameraSensitivity();


private:
	//カメラの左右反転
	static bool isRightAxisReversX_;
	//カメラの上下反転
	static bool isRightAxisReversY_;
	//回避でダッシュできるか
	static bool canDashByRolling_;
	//カメラ感度
	static float cameraSensitivity_;
};

#endif