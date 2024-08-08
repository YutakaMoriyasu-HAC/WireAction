#pragma once

#include "app/StateMachine/StateBase.h"

class World;

class PlayingState : public app::StateBase
{
public:
	PlayingState(app::StateMachine* stateMachine, World* world);
	~PlayingState();
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
	// �A�N�e�B�u���̂ݕ`��
	void drawOnlyWhenActive()  const override;
private:
	World* world_;
};