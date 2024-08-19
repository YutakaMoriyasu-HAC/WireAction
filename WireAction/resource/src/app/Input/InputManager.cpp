#include "InputManager.h"

bool InputManager::isRightAxisReversX_{ true };
bool InputManager::isRightAxisReversY_{ false };
bool InputManager::canDashByRolling_{ false };
float InputManager::cameraSensitivity_{ 2.0f };

const int PAD_NUM = 0;

const GSvector2 InputManager::IsDirectionalInput()
{
    GSvector2 direct = GSvector2::zero();
    // 十字ボタン
    if (gsXBoxPadButtonState(PAD_NUM, GS_XBOX_PAD_UP)) direct.y = 1;
    else if (gsXBoxPadButtonState(PAD_NUM, GS_XBOX_PAD_DOWN)) direct.y = -1;
    if (gsXBoxPadButtonState(PAD_NUM, GS_XBOX_PAD_LEFT)) direct.x = -1;
    else if (gsXBoxPadButtonState(PAD_NUM, GS_XBOX_PAD_RIGHT)) direct.x = 1;
    // 十字ボタン入力があれば返却
    if (direct.length() != 0) return direct;
    // コントローラー
    gsXBoxPadGetLeftAxis(PAD_NUM, &direct);
    return direct;
}

const GSvector2 InputManager::Move()
{
    GSvector2 direct = GSvector2::zero();
    // コントローラー
    gsXBoxPadGetLeftAxis(PAD_NUM, &direct);
    // パッド入力が有れば返却
    if (direct.length() != 0) return direct;
    // キー入力
    if (gsGetKeyState(GKEY_W)) direct.y = 1;
    else if (gsGetKeyState(GKEY_S)) direct.y = -1;
    if (gsGetKeyState(GKEY_A)) direct.x = -1;
    else if (gsGetKeyState(GKEY_D)) direct.x = 1;
    return direct;
}

const GSvector2 InputManager::CameraRotation()
{
    GSvector2 direct = GSvector2::zero();
    // コントローラー
    gsXBoxPadGetRightAxis(PAD_NUM, &direct);
    direct.x *= isRightAxisReversX_ ? -1 : 1;
    if (ABS(direct.y) >= 0.9f) {
        direct.y = direct.y < 0 ? -1 : 1;
    }
    // パッドの入力があれば返却
    if (direct.length() != 0) return direct;
    // キー入力
    if (gsGetKeyTrigger(GKEY_UPARROW)) direct.y = 1;
    else if (gsGetKeyTrigger(GKEY_DOWNARROW)) direct.y = -1;
    if (gsGetKeyState(GKEY_LEFTARROW)) direct.x = -1;
    else if (gsGetKeyState(GKEY_RIGHTARROW)) direct.x = 1;
    return direct;
}

const bool InputManager::IsCameraReset()
{
    return gsXBoxPadButtonTrigger(PAD_NUM, GS_XBOX_PAD_LEFT_SHOULDER) ? true : gsGetKeyTrigger(GKEY_Q);
}

const bool InputManager::IsDecisionTrigger()
{
    return gsXBoxPadButtonTrigger(PAD_NUM, GS_XBOX_PAD_A) ? true : gsGetKeyTrigger(GKEY_SPACE);
}

const bool InputManager::IsPauseTrigger()
{
    return gsXBoxPadButtonTrigger(PAD_NUM, GS_XBOX_PAD_BACK) ? true : gsGetKeyTrigger(GKEY_SPACE);
}

const bool InputManager::IsProtectState()
{
    return gsXBoxPadGetRightTrigger(PAD_NUM) > 0.0f ? true : gsGetKeyState(GKEY_SPACE);
}

const bool InputManager::IsNormalAttackTrigger()
{
    return gsXBoxPadButtonTrigger(PAD_NUM, GS_XBOX_PAD_A) ? true : gsGetKeyTrigger(GKEY_O);
}

const bool InputManager::IsNormalAttackState()
{
    return gsXBoxPadButtonState(PAD_NUM, GS_XBOX_PAD_A) ? true : gsGetKeyState(GKEY_O);
}

const bool InputManager::IsDetachNormalAttackTrigger()
{
    return gsXBoxPadButtonDetach(PAD_NUM, GS_XBOX_PAD_A) ? true : gsGetKeyDetach(GKEY_O);
}

const bool InputManager::IsStrongAttackTrigger()
{
    return gsXBoxPadButtonTrigger(PAD_NUM, GS_XBOX_PAD_X) ? true : gsGetKeyTrigger(GKEY_P);
}

const bool InputManager::IsStrongAttackState()
{
    return  gsXBoxPadButtonState(PAD_NUM, GS_XBOX_PAD_X) ? true : gsGetKeyState(GKEY_P);
}

const bool InputManager::IsDetachStrongAttackTrigger()
{
    return gsXBoxPadButtonDetach(PAD_NUM, GS_XBOX_PAD_X) ? true : gsGetKeyDetach(GKEY_P);
}

const bool InputManager::IsRollingTrigger()
{
    return gsXBoxPadButtonTrigger(PAD_NUM, GS_XBOX_PAD_RIGHT_SHOULDER) ? true : gsGetKeyTrigger(GKEY_LSHIFT);
}

const bool InputManager::IsDashState()
{
    return gsXBoxPadButtonState(PAD_NUM, GS_XBOX_PAD_B) ? true : gsGetKeyState(GKEY_LSHIFT);
}

const bool InputManager::IsRollingDashState()
{
    return gsXBoxPadButtonState(PAD_NUM, GS_XBOX_PAD_RIGHT_SHOULDER) ? true : gsGetKeyState(GKEY_LSHIFT);
}

const bool InputManager::IsCameraResetTrigger()
{
    return gsXBoxPadButtonTrigger(PAD_NUM, GS_XBOX_PAD_LEFT_SHOULDER) ? true : gsGetKeyTrigger(GKEY_RSHIFT);
}

const bool InputManager::IsCameraResetState()
{
    return gsXBoxPadButtonState(PAD_NUM, GS_XBOX_PAD_LEFT_SHOULDER) ? true : gsGetKeyState(GKEY_RSHIFT);
}

const bool InputManager::IsTargetCameraTrigger()
{
    return gsXBoxPadButtonTrigger(PAD_NUM, GS_XBOX_PAD_RIGHT_THUMB) ? true : gsGetKeyTrigger(GKEY_J);
}

void InputManager::RightAxisReversX(const bool flag)
{
    isRightAxisReversX_ = flag;
}

void InputManager::RightAxisReversY(const bool flag)
{
    isRightAxisReversY_ = flag;
}

void InputManager::SetRollingDash(const bool flag)
{
    canDashByRolling_ = flag;
}

const bool InputManager::IsRightAxisReversX()
{
    return isRightAxisReversX_;
}

const bool InputManager::IsRightAxisReversY()
{
    return isRightAxisReversY_;
}

const bool InputManager::GetRollingDash() {
    return canDashByRolling_;
}


const bool InputManager::IsMenuTrigger()
{
    return gsXBoxPadButtonTrigger(PAD_NUM, GS_XBOX_PAD_START) || gsGetKeyTrigger(GKEY_TAB);
}

const bool InputManager::IsGameEndState()
{
    return (gsXBoxPadButtonState(PAD_NUM, GS_XBOX_PAD_START) && gsXBoxPadButtonState(PAD_NUM, GS_XBOX_PAD_BACK)) || gsGetKeyTrigger(GKEY_ESCAPE);
}

const bool InputManager::IsLeftButtonPressed()
{
    return gsXBoxPadButtonTrigger(PAD_NUM, GS_XBOX_PAD_LEFT) || gsGetKeyTrigger(GKEY_J);
}

const bool InputManager::IsRightButtonPressed()
{
    return gsXBoxPadButtonTrigger(PAD_NUM, GS_XBOX_PAD_RIGHT) || gsGetKeyTrigger(GKEY_K);
}

const bool InputManager::IsUpButtonPressed()
{
    return  gsXBoxPadButtonTrigger(PAD_NUM, GS_XBOX_PAD_UP) || gsGetKeyTrigger(GKEY_I);
}

const bool InputManager::IsBottomButtonPressed()
{
    return  gsXBoxPadButtonTrigger(PAD_NUM, GS_XBOX_PAD_DOWN) || gsGetKeyTrigger(GKEY_M);
}

const bool InputManager::IsLeftTriggerButtonPressed()
{
    return  gsXBoxPadGetLeftTrigger(PAD_NUM) > 0.0f;
}

const void InputManager::PadVibrationStart(float rightmotor, float leftmotor)
{
    return gsXBoxPadSetVibration(PAD_NUM, rightmotor, leftmotor);
}

const void InputManager::PadVibrationEnd()
{
    return gsXBoxPadSetVibration(PAD_NUM, 0.0f, 0.0f);
}

const bool InputManager::IsAButtonState()
{
    return gsXBoxPadButtonState(PAD_NUM, GS_XBOX_PAD_A) || gsGetKeyState(GKEY_F);
}

const bool InputManager::IsAButtonTrigger()
{
    return gsXBoxPadButtonTrigger(PAD_NUM, GS_XBOX_PAD_A) || gsGetKeyTrigger(GKEY_F);
}

const bool InputManager::IsBButtonState()
{
    return gsXBoxPadButtonState(PAD_NUM, GS_XBOX_PAD_B) || gsGetKeyState(GKEY_T);
}

const bool InputManager::IsBButtonTrigger()
{
    return gsXBoxPadButtonTrigger(PAD_NUM, GS_XBOX_PAD_B) || gsGetKeyTrigger(GKEY_T);
}

const bool InputManager::IsXButtonState()
{
    return gsXBoxPadButtonState(PAD_NUM, GS_XBOX_PAD_X) || gsGetKeyState(GKEY_B);
}

const bool InputManager::IsXButtonTrigger()
{
    return gsXBoxPadButtonTrigger(PAD_NUM, GS_XBOX_PAD_X) || gsGetKeyTrigger(GKEY_T);
}

const bool InputManager::IsYButtonState()
{
    return gsXBoxPadButtonState(PAD_NUM, GS_XBOX_PAD_Y) || gsGetKeyState(GKEY_R);
}

const bool InputManager::IsYButtonTrigger()
{
    return gsXBoxPadButtonTrigger(PAD_NUM, GS_XBOX_PAD_Y) || gsGetKeyTrigger(GKEY_R);
}

const bool InputManager::IsSheatheTrigger()
{
    return gsXBoxPadButtonTrigger(PAD_NUM, GS_XBOX_PAD_X) ? true : gsGetKeyTrigger(GKEY_B);
}

const void InputManager::GetJoyLeftAxis(int* x, int* y) {
    gsGetJoyLeftAxis(PAD_NUM, x,y);
}

const void InputManager::SetCameraSensitivity(float sensitiv) {
    cameraSensitivity_ = sensitiv;
}

const float InputManager::GetCameraSensitivity() {
    return cameraSensitivity_;
}