#ifndef GAME_CLOCK_H_
#define GAME_CLOCK_H_

class GameClock {
	GameClock();

	void update(float delta_time);

	const float get();

	void countReset();

	void toggleCount();

	// ÉRÉsÅ[ã÷é~
	GameClock(const GameClock& other) = delete;
	GameClock& operator = (const GameClock& other) = delete;

private:
	float	count_;
	bool	pause_{ false };
};

#endif