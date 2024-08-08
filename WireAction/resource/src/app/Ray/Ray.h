#ifndef RAY_RAY_H_
#define RAY_RAY_H_

#include <gslib.h>

class Ray {
public:
	Ray() = default;
	Ray(const GSvector3& position, const GSvector3& direction) :
		position{ position }, direction{ direction } {}

public:
	// ç¿ïW
	GSvector3 position{ 0.0f, 0.0f, 0.0f };
	// ï˚å¸
	GSvector3 direction{ 0.0f, 0.0f, 0.0f };
};

#endif
