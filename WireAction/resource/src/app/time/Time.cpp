#include "Time.h"

float app::Time::deltaTime = 0.0f;
float app::Time::deltaRate = 1.0f;

// �f���^�^�C�����X�V
void app::Time::UpdateDeltaTime(const float time)
{
	deltaTime = time;
}

void app::Time::setDeltaTimeRate(const float rate)
{
	deltaRate = rate;
}
