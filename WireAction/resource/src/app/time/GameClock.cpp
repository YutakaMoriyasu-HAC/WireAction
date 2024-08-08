#include "GameClock.h"

GameClock::GameClock()
{
    countReset();
}

void GameClock::update(float delta_time)
{
    count_ += delta_time;
}

const float GameClock::get()
{
    return count_;
}

void GameClock::countReset()
{
    count_ = 0.0f;
}

void GameClock::toggleCount()
{
    pause_ = !pause_;
}
