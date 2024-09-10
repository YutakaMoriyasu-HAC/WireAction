#ifndef APP_MATH_MATH_H_
#define APP_MATH_MATH_H_

#include <cmath>
#include <gslib.h>

// �}�X�N���X
namespace math {
	// PI
	const static float PI = 3.1415926535897932384626433832795f;	// �~����
	const float PI_2 = PI * 2.0f;	// �~������2�{
	const float RAD_2_DEG = 180.0f / PI;
	const float DEG_2_RAD = PI / 180.0f;
	// ����
	const static float HALF = 0.5f;
	// �����ɂ���
	const static float makeHalf(const float value) {
		return value * HALF;
	}
	// ���W�A����x��
	const static float radToDeg(const float rad) {
		return rad * RAD_2_DEG;
	}
	// �x�����W�A����
	const static float degToRad(const float deg) {
		return deg * DEG_2_RAD;
	}
	const static int wrap(const int value, const int low, const int high) {
		int n = (value - low) % (high - low);
		return n >= 0 ? (n + low) : (n + high);
	}

	const static float wrap(const float value, const float low, const float high) {
		float n = std::fmod(value - low, high - low);
		return n >= 0.0f ? (n + low) : (n + high);
	}
	// �N�����v
	const static int clamp(const int value, const int low, const int high) {
		if (value < low) return low;
		if (value > high) return high;
		return value;
	}
	// �N�����v
	const static float clamp(const float value, const float low, const float high) {
		if (value < low) return low;
		if (value > high) return high;
		return value;
	}
	// 0~1�ŃN�����v
	const static int clamp01(const int a) {
		return clamp(a, 0, 1);
	}
	// 0~1�ŃN�����v
	const static float clamp01(const float a) {
		return clamp(a, 0.0f, 1.0f);
	}
	const static int lerp(const int a, const int b, const float t) {
		return a + (b - a) * t;
	}

	const static int lerpCramped(const int a, const int b, const float t) {
		return a + (b - a) * clamp01(t);
	}
	const static float lerp(const float a, const float b, const float t) {
		return a + (b - a) * t;
	}

	const static float lerpCramped(const float a, const float b, const float t) {
		return a + (b - a) * clamp01(t);
	}
	// �덷�ȓ��Ɋ܂܂�Ă��邩(�덷�� margin ������)
	const static bool comparison(const float value1, const float value2, const float margin) {
		return std::abs(value1 - value2) < margin;
	}
	// Vector3�̌덷�L���r
	const static bool comparison(const GSvector3& vec1, const GSvector3& vec2, const GSvector3& margin) {
		return comparison(vec1.x, vec2.x, margin.x) && comparison(vec1.y, vec2.y, margin.y) && comparison(vec1.z, vec2.z, margin.z);
	}
	// ������ԋp
	const static int sign(const int value) {
		return value < 0 ? -1 : 1;
	}
	// �����̐؂�グ
	const static int roundingUp(const float f) {
		int val = f;
		return f - val == 0.0f ? val : val + sign(val);
	}
	// �����̐؂�グ
	const static int roundingUpAbs(const float f) {
		return std::abs(roundingUp(f));
	}

	// �t���[����b�ɕϊ�
	const static float frameToSecond(const float frame) {
		return frame / 60.0f;
	}
	// �b�𕪂ɕϊ�
	const static float secondToMinutes(const float second) {
		return second / 60.0f;
	}

	/// <summary>
	/// �����_�ȉ��̌������w�肵�Đ؂�̂�
	/// </summary>
	/// <param name="value">���lparam>
	/// <param name="decimalPlace">�����_�ȉ��̌�>
	/// <returns></returns>
	const static float TruncateDecimal(const float value, const int decimalPlace) {
		int multiplier = std::pow(10.0f, decimalPlace);
		return std::floor(value * multiplier) / multiplier;
	}


	//x��z�̒l����΂߂̒��������߂�
	const static float radiusFromVector2(float x, float z) {
		return sqrtf((x * x) + (z * z));
	}

	//x��y��z�̒l����΂߂̒��������߂�
	const static float radiusFromVector3(GSvector3 vector) {
		return sqrtf((vector.x * vector.x) + (vector.y * vector.y)+ (vector.z * vector.z));
	}

	//�x�N�g�����烉�W�A�������߂�
	const static float angleFromVector2Rad(float x, float z) {
		return atan2f(z, x);
	}

	//�x�N�g������x�����߂�
	const static float angleFromVector2Deg(float x, float z) {
		float answer = atan2f(z, x);
		return radToDeg(answer);
	}
}

#endif
