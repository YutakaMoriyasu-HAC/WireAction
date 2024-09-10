#ifndef INPUT_INPUTMANAGER_H_
#define INPUT_INPUTMANAGER_H_

#include <gslib.h>

// ���͊Ǘ��N���X
static class InputManager
{
public:
	// L�X�e�B�b�Nor�\���{�^�������͂��ꂽ��
	const static GSvector2 IsDirectionalInput();
	// �ړ�
	const static GSvector2 Move();
	// �J�����ړ�		
	const static GSvector2 CameraRotation();
	//�J�������Z�b�g
	const static bool IsCameraReset();
	//���Ⴊ��
	const static bool IsCrouchState();
	// ����{�^������������
	const static bool IsDecisionTrigger();
	// �K�[�h�{�^������������
	const static bool IsProtectState();
	// �ʏ�U���{�^������������
	const static bool IsNormalAttackTrigger();
	// �ʏ�U���{�^�������������Ă��邩
	const static bool IsNormalAttackState();
	// �ʏ�U���{�^���𗣂�����
	const static bool IsDetachNormalAttackTrigger();
	//��]�a��{�^������������
	const static bool IsStrongAttackTrigger();
	//��]�a��{�^�����������Ă��Ă��邩
	const static bool IsStrongAttackState();
	//��]�a��{�^���𗣂������H
	const static bool IsDetachStrongAttackTrigger();
	//�J�������Z�b�g�{�^����������
	const static bool IsCameraResetTrigger();
	//�J�������Z�b�g�{�^��������������
	const static bool IsCameraResetState();
	// ����{�^������������
	const static bool IsRollingTrigger();
	// B�{�^���������Ń_�b�V��
	const static bool IsDashState();
	// ����{�^���������ł��_�b�V��
	const static bool IsRollingDashState();
	//�E�X�e�B�b�N��������
	const static bool IsTargetCameraTrigger();
	//�E�X�e�B�b�N��X�������̓��͋t�]�̐ݒ�
	static void RightAxisReversX(const bool flag);
	//�E�X�e�B�b�N��Y�������̓��͋t�]�̐ݒ�
	static void RightAxisReversY(const bool flag);
	//����Ń_�b�V�����邩�ݒ�
	static void SetRollingDash(const bool flag);
	//�J�������]���H
	const static bool IsRightAxisReversX();
	const static bool IsRightAxisReversY();
	//����Ń_�b�V���ł��邩�H
	const static bool GetRollingDash();

	//�K�[�h�{�^���������ꂽ���ǂ����H
	const static bool IsSheatheTrigger();
	// �|�[�Y�{�^���������ꂽ��
	const static bool IsPauseTrigger();
	// ���j���[�{�^���������ꂽ��
	const static bool IsMenuTrigger();
	// �Q�[���I���{�^���������ꂽ��
	const static bool IsGameEndState();
	// �R���g���[���[�̏\�����{�^���������ꂽ��
	const static bool IsLeftButtonPressed();
	// �R���g���[���[�̏\���E�{�^���������ꂽ��
	const static bool IsRightButtonPressed();
	//�R���g���[���[�̏\����{�^���������ꂽ��
	const static bool IsUpButtonPressed();
	//�R���g���[���[�̏\�����{�^���������ꂽ��
	const static bool IsBottomButtonPressed();

	// �R���g���[���[��LT�{�^���������ꂽ��
	const static bool IsLeftTriggerButtonPressed();
	//�R���g���[���[�̐U���J�n
	const static void PadVibrationStart(float rightmotor = 1.0f,float leftmotor=1.0f);
	//�R���g���[���[�̐U���I��
	const static void PadVibrationEnd();


	// A�{�^��
	const static bool IsAButtonState();
	const static bool IsAButtonTrigger();
	// B�{�^��
	const static bool IsBButtonState();
	const static bool IsBButtonTrigger();
	// X�{�^��
	const static bool IsXButtonState();
	const static bool IsXButtonTrigger();
	// Y�{�^��
	const static bool IsYButtonState();
	const static bool IsYButtonTrigger();

	//���X�e�B�b�N�̏��
	const static void GetJoyLeftAxis(int *x, int *y);

	//�J�������x�̐ݒ�
	const static void SetCameraSensitivity(float sensitiv);
	//�J�������x���擾
	const static float GetCameraSensitivity();


private:
	//�J�����̍��E���]
	static bool isRightAxisReversX_;
	//�J�����̏㉺���]
	static bool isRightAxisReversY_;
	//����Ń_�b�V���ł��邩
	static bool canDashByRolling_;
	//�J�������x
	static float cameraSensitivity_;
};

#endif