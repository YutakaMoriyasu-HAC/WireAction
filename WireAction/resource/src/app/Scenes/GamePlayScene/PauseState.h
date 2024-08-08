#pragma once
#include "app/StateMachine/StateBase.h"

class PauseState : public app::StateBase
{
public:
	PauseState(app::StateMachine* stateMachine);
	~PauseState();
	// ‰Šú‰»
	void init() override;
	// I—¹
	void final() override;
	// XV
	void update() override;
	// ’x‰„XV
	void lateUpdate() override;
	// •`‰æ
	void draw() const override;
	// ’x‰„•`‰æ
	void lateDraw() const override;
};
