#ifndef APP_EFFECT_EFFECTMANAGER_H_
#define APP_EFFECT_EFFECTMANAGER_H_

#include <list>
#include <memory>
#include "app/Effect/Effect.h"

class EffectManager {
public:
	EffectManager();
	~EffectManager();

	void update(float delta_time);
	void draw() const;
	// 消去
	void clear();

public:
	// GUIの追加
	template <class... Args>
	void add(Args&&... args) {
		effectList_.push_back(std::make_shared<Effect>(args...));
	}
	void add(std::shared_ptr<Effect> effect);

public:
	std::shared_ptr<Effect> findEffect(const std::string& name);

public:
	// コピー禁止
	EffectManager(const EffectManager& other) = delete;
	EffectManager& operator=(const EffectManager& other) = delete;

private:
	// 死亡確認
	void DeadCheck();

	//生きてたやつは殺す
	void AllDead();

private:
	std::list<std::shared_ptr<Effect>> effectList_;
};

#endif
