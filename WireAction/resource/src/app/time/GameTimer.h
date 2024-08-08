#ifndef APP_TIME_GAMETIMER_H_
#define APP_TIME_GAMETIMER_H_

class GameTimer {
public:
	GameTimer();

	void update();

public:
	const float getRemainTime() const;
	const float getElapsedTime() const;

public:
	void reset(const float remainTime);

	// ÉRÉsÅ[ã÷é~
	GameTimer(const GameTimer& other) = delete;
	GameTimer& operator = (const GameTimer& other) = delete;

private:
	float remainTimer_;
	float elapsedTimer_;
};

#endif
