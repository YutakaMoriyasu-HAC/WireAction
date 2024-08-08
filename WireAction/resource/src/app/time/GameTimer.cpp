#include "GameTimer.h"

#include "app/time/Time.h"

GameTimer::GameTimer() :
    remainTimer_(0.0f), elapsedTimer_(0.0f)
{
}

void GameTimer::update()
{
    remainTimer_ -= app::Time::deltaTime;
    elapsedTimer_ += app::Time::deltaTime;
}

const float GameTimer::getRemainTime() const
{
    return remainTimer_;
}

const float GameTimer::getElapsedTime() const
{
    return elapsedTimer_;
}

void GameTimer::reset(const float remainTime)
{
    remainTimer_ = remainTime;
    elapsedTimer_ = 0.0f;
}
