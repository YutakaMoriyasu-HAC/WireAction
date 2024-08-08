#pragma once
#include "app/StateMachine/StateBase.h"

class PauseState : public app::StateBase
{
public:
	PauseState(app::StateMachine* stateMachine);
	~PauseState();
	// ������
	void init() override;
	// �I��
	void final() override;
	// �X�V
	void update() override;
	// �x���X�V
	void lateUpdate() override;
	// �`��
	void draw() const override;
	// �x���`��
	void lateDraw() const override;
};
