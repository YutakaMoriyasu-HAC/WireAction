#include "FollowEffect.h"

#include <GSeffect.h>
#include "app/time/Time.h"
#include "app/Actors/Actor/Actor.h"

FollowEffect::FollowEffect(GSuint effectID, Actor* owner, const GSvector3& offset, float lifeTime, bool isDieTogether) :
    Effect(effectID, owner->transform().position(), lifeTime), owner_(owner), offset_(offset), isDieTogerther_(isDieTogether)
{
}

FollowEffect::FollowEffect(GSuint effectID, Actor* owner, const GSvector3& offset, float lifeTime, float exRate, bool isDieTogether) :
    Effect(effectID, owner->transform().position(), lifeTime, exRate), owner_(owner), offset_(offset), isDieTogerther_(isDieTogether)
{
}

FollowEffect::FollowEffect(GSuint effectID, Actor* owner, const GSvector3& offset, const GSquaternion& rotation, float lifeTime, bool isDieTogether) :
    Effect(effectID, owner->transform().position() , rotation, lifeTime), owner_(owner), offset_(offset), isDieTogerther_(isDieTogether)
{
}

FollowEffect::FollowEffect(GSuint effectID, Actor* owner, const GSvector3& offset, const GSquaternion& rotation, float lifeTime, float exRate, bool isDieTogether) :
    Effect(effectID, owner->transform().position(), rotation, lifeTime, exRate), owner_(owner), offset_(offset), isDieTogerther_(isDieTogether)
{
}

FollowEffect::FollowEffect(GSuint effectID, Actor* owner, const GSvector3& offset, const GSvector3& eulerAngles, float lifeTime, bool isDieTogether) :
    Effect(effectID, owner->transform().position(), lifeTime), owner_(owner), offset_(offset), isDieTogerther_(isDieTogether)
{
    if (owner_ != nullptr)
    {
        eulerAngles_ = eulerAngles;
        transform_.eulerAngles(owner_->transform().eulerAngles() += eulerAngles_);
    }
}

FollowEffect::FollowEffect(GSuint effectID, Actor* owner, const GSvector3& offset, const GSvector3& eulerAngles, float lifeTime, float exRate, bool isDieTogether) :
    Effect(effectID, owner->transform().position(), lifeTime, exRate), owner_(owner), offset_(offset), isDieTogerther_(isDieTogether)
{
    if (owner_ != nullptr)
    {
        eulerAngles_ = eulerAngles;
        transform_.eulerAngles(owner_->transform().eulerAngles() += eulerAngles_);
    }
}

FollowEffect::~FollowEffect()
{
}

void FollowEffect::update()
{
    if (timer_ < lifeTime_) {
        timer_ += app::Time::deltaTime;
    }
    else {
        Destroy();
    }

    if (!isDieTogerther_) return;
    if (owner_ == nullptr) {
        Destroy();
        return;
    }
    // 持ち主が死んだ場合は自分も死ぬ
    if (owner_->is_dead()) {
        Destroy();
        owner_ = nullptr;
    }
}

void FollowEffect::draw()
{
    if (owner_ == nullptr) return;
    // エフェクトに自身のワールド変換行列を設定
    GStransform transform = transform_;
    transform.position(owner_->transform().position() + offset_);
    transform_.eulerAngles(owner_->transform().eulerAngles() += eulerAngles_);
    GSmatrix4 world = transform.localToWorldMatrix();
    gsSetEffectMatrix(effectHandle_, &world); // ワールド変換行列を設定
    // 寿命によって、だんだん透明にしていく（アルファ値を変換させる）
    GScolor start_color{ 1.0f,1.0f, 1.0f, 1.0f };
    GScolor end_color{ 1.0f, 1.0f, 1.0f, 0.0f };
    GScolor color = GScolor::lerp(start_color, end_color, timer_ / lifeTime_);
    gsSetEffectColor(effectHandle_, &color); // カラーを設定  
}

void FollowEffect::end()
{
    gsStopEffect(effectHandle_);
}

bool FollowEffect::isDead() const
{
    return isDie_;
}

void FollowEffect::Destroy()
{
    isDie_ = true;
}
