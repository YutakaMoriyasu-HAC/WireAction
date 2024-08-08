#include "Effect.h"

#include <GSeffect.h>
#include "app/time/Time.h"

Effect::Effect(GSuint effectID, const GSvector3& position, float lifeTime, float exRate, const std::string& name) :
    lifeTime_(lifeTime), timer_(0.0f), isDie_(false), name_(name)
{ 
    transform_.position(position);
    // エフェクトの生成
    effectHandle_ = gsPlayEffect(effectID, &position);
    // エフェクトのスケール
    GSvector3 scale{ exRate, exRate, exRate };
    gsSetEffectScale(effectHandle_, &scale);
    transform_.localScale(scale);
}

Effect::Effect(GSuint effectID, const GSvector3& position, const GSquaternion& rotation, float lifeTime, float exRate, const std::string& name) :
    lifeTime_(lifeTime), timer_(0.0f), isDie_(false), name_(name)
{
    transform_.position(position);
    transform_.rotation(rotation);
    // エフェクトの生成
    effectHandle_ = gsPlayEffect(effectID, &position);
    // エフェクトのスケール
    GSvector3 scale{ exRate, exRate, exRate };
    gsSetEffectScale(effectHandle_, &scale);
    transform_.localScale(scale);
}

Effect::Effect(GSuint effectID, const GStransform& transform, float lifeTime, float exRate, const std::string& name) :
    lifeTime_(lifeTime), timer_(0.0f), transform_(transform), isDie_(false), name_(name)
{
    // エフェクトの生成
    GSvector3 position = transform_.position();
    effectHandle_ = gsPlayEffect(effectID, &position);
    // エフェクトのスケール
    GSvector3 scale{ exRate, exRate, exRate };
    gsSetEffectScale(effectHandle_, &scale);
    transform_.localScale(scale);
}

Effect::~Effect()
{
}

void Effect::update()
{
    if (timer_ < lifeTime_) {
        timer_ += app::Time::deltaTime;
    }
    else {
        Destroy();
    }
}

void Effect::draw()
{
    // エフェクトに自身のワールド変換行列を設定
    GSmatrix4 world = transform_.localToWorldMatrix();
    gsSetEffectMatrix(effectHandle_, &world); // ワールド変換行列を設定
    // 寿命によって、だんだん透明にしていく（アルファ値を変換させる）
    GScolor start_color{ 1.0f, 1.0f, 1.0f, 1.0f };
    GScolor end_color{ 1.0f, 1.0f, 1.0f, 0.0f };
    GScolor color = GScolor::lerp(start_color, end_color, timer_ / lifeTime_);
    gsSetEffectColor(effectHandle_, &color); // カラーを設定
}

void Effect::end()
{
    gsStopEffect(effectHandle_);
}

void Effect::setName(const std::string& name)
{
    name_ = name;
}

const std::string& Effect::getName() const
{
    return name_;
}

bool Effect::isDead() const
{
    return isDie_;
}

void Effect::Destroy()
{
    isDie_ = true;
}

void Effect::setTimer(float time) {
    timer_ = time;
}