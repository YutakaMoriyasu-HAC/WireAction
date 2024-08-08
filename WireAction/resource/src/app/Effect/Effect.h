#ifndef APP_EFFECT_EFFECT_H_
#define APP_EFFECT_EFFECT_H_

#include <GStransform.h>
#include <string>

class Effect {
public:
	Effect(GSuint effectID, const GSvector3& position, float lifeTime, float exRate = 1.0f, const std::string& name = "");
	Effect(GSuint effectID, const GSvector3& position, const GSquaternion& rotation, float lifeTime, float exRate = 1.0f, const std::string& name = "");
	Effect(GSuint effectID, const GStransform& transform, float lifeTime, float exRate = 1.0f, const std::string& name = "");
	~Effect();

	virtual void update();
	virtual void draw();

	virtual void end();

	void setName(const std::string& name);
	const std::string& getName() const;

	bool isDead() const;
	void Destroy();
	void setTimer(float time);

protected:
	GSuint effectHandle_;
	GStransform transform_;
	float lifeTime_;
	float timer_;
	bool isDie_;
	std::string name_;
};

#endif
