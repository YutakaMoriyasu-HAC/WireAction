#ifndef APP_TIME_TIME_H_
#define APP_TIME_TIME_H_

namespace app {
	// ���ԃN���X
	class Time
	{
	public:
		static float deltaTime; // �f���^�^�C��

		static float deltaRate;	//�f���^�^�C�����[�g

	public:
		// �f���^�^�C�����X�V
		static void UpdateDeltaTime(const float time);
		// �f���^�^�C���{����ϓ�
		static void setDeltaTimeRate(const float rate);
	};
}

#endif