#pragma once

#include "app/StateMachine/StateBase.h"

class World;

class PlayingState : public app::StateBase
{
public:
	PlayingState(app::StateMachine* stateMachine, World* world);
	~PlayingState();
	// 初期化
	void init() override;
	// 終了
	void final() override;
	// 更新
	void update() override;
	// 遅延更新
	void lateUpdate() override;
	// 描画
	void draw() const override;
	// 遅延描画
	void lateDraw() const override;
	// アクティブ時のみ描画
	void drawOnlyWhenActive()  const override;
private:
	World* world_;
};