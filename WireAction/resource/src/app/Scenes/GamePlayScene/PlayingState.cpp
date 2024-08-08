#include "PlayingState.h"
#include <gslib.h>
#include "app/NameList/Assets.h"
#include "PlaySceneState.h"
#include "app/StateMachine/StateMachine.h"
#include "app/Worlds/World.h"
#include "app/time/Time.h"
#include "app/Input/InputManager.h"

PlayingState::PlayingState(app::StateMachine* stateMachine, World* world):
	StateBase(stateMachine), world_(world)
{
}

PlayingState::~PlayingState()
{
}

void PlayingState::init()
{

}

void PlayingState::final()
{
}

void PlayingState::update()
{
	if (InputManager::IsPauseTrigger())
	{
		stateMachine_->pushState(stateTag::PauseState);
	}
	
	world_->update(app::Time::deltaTime);





}

void PlayingState::lateUpdate()
{
}

void PlayingState::draw() const
{
	world_->draw();
}

void PlayingState::lateDraw() const
{
}

void PlayingState::drawOnlyWhenActive() const
{
}
