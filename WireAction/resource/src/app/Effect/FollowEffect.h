#ifndef APP_EFFECT_FOLLOWEFFECT_H_
#define APP_EFFECT_FOLLOWEFFECT_H_

#include <GStransform.h>
#include "Effect.h"
class Actor;

class FollowEffect : public Effect {
public:
	FollowEffect(GSuint effectID, Actor* owner, const GSvector3& offset, float lifeTime, bool isDieTogether = true);
	FollowEffect(GSuint effectID, Actor* owner, const GSvector3& offset, bool isDieTogether = true);
	FollowEffect(GSuint effectID, Actor* owner, const GSvector3& offset, float lifeTime, float exRate, bool isDieTogether = true);
	FollowEffect(GSuint effectID, Actor* owner, const GSvector3& offset, const GSquaternion& rotation, float lifeTime, bool isDieTogether = true);
	FollowEffect(GSuint effectID, Actor* owner, const GSvector3& offset, const GSquaternion& rotation, float lifeTime, float exRate, bool isDieTogether = true);
	FollowEffect(GSuint effectID, Actor* owner, const GSvector3& offset, const GSvector3& eulerAngles, float lifeTime, bool isDieTogether = true);
	FollowEffect(GSuint effectID, Actor* owner, const GSvector3& offset, const GSvector3& eulerAngles, float lifeTime, float exRate, bool isDieTogether = true);
	~FollowEffect();

	void update() override;
	void draw() override;

	void end() override;

	bool isDead() const;
	void Destroy();

private:
	Actor* owner_{ nullptr };
	GSvector3 offset_;
	bool isDieTogerther_;
	GSvector3 eulerAngles_;
};

#endif
