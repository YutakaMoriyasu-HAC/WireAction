#include "EffectManager.h"
#include <GSeffect.h>

EffectManager::EffectManager()
{
}

EffectManager::~EffectManager()
{
}

void EffectManager::update(float delta_time)
{
	for (const auto& effect : effectList_) {
		effect->update();
	}
	DeadCheck();
}

void EffectManager::draw() const
{
	for (const auto& effect : effectList_) {
		effect->draw();
	}
}

void EffectManager::clear()
{
	
	AllDead();
}

void EffectManager::add(std::shared_ptr<Effect> effect)
{
	effectList_.push_back(effect);
}

std::shared_ptr<Effect> EffectManager::findEffect(const std::string& name)
{
	for (auto& effect : effectList_) {
		if (effect->getName() == name) return effect;
	}
	return nullptr;
}

void EffectManager::DeadCheck()
{
	if (effectList_.empty()) return;

	for (auto itr = effectList_.begin(); itr != effectList_.end();) {
		bool flag = (*itr)->isDead();
		if (flag) {
			(*itr)->end();
			itr = effectList_.erase(itr);
			if (effectList_.empty()) break;
		}
		else {
			itr++;
		}
	}
}

void EffectManager::AllDead()
{
	if (effectList_.empty()) return;

	for (auto itr = effectList_.begin(); itr != effectList_.end();) {
		bool flag = (*itr)->isDead();
		if (!flag) {
			(*itr)->Destroy();
			itr++;
		}
		else {
			itr++;
		}
	}
}
