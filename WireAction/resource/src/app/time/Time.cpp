#include "Time.h"

float app::Time::deltaTime = 0.0f;
float app::Time::deltaRate = 1.0f;

// デルタタイムを更新
void app::Time::UpdateDeltaTime(const float time)
{
	deltaTime = time;
}

void app::Time::setDeltaTimeRate(const float rate)
{
	deltaRate = rate;
}
