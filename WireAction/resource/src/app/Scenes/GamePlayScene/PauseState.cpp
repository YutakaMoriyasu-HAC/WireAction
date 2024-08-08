#include "PauseState.h"
#include <gslib.h>
#include "app/NameList/Assets.h"
#include "app/StateMachine/StateMachine.h"
#include "app/Input/InputManager.h"

PauseState::PauseState(app::StateMachine* stateMachine):
	StateBase(stateMachine)
{
	isTransparency_ = true;
}

PauseState::~PauseState()
{
}

void PauseState::init()
{
}

void PauseState::final()
{
}

void PauseState::update()
{
	
}

void PauseState::lateUpdate()
{
}

void PauseState::draw() const
{
	//ポーズ中の画像表示
	
}

void PauseState::lateDraw() const
{
}
