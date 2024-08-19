#include "TweenUnit.h"
#include <algorithm>

TweenUnit::TweenUnit(float duration) : duration_{ duration } { }

void TweenUnit::update(float delta_time) {
    // 経過時間を進める
    elapsed_ += delta_time;
    // 進捗率（0～1）を計算
    float progress = elapsed_ / duration_;
    // 進捗率が1を超えないようにする
    progress = std::min(progress, 1.0f);
    // 進捗率にイージングを適用する
    float eased = apply_ease(progress, ease_type_);
    // イージングを適用した進捗率を使い、派生クラスごとの更新処理を行う
    on_update(eased);
    // 終了したかを判定する
    is_finished_ = progress >= 1.0f;
}

bool TweenUnit::is_finished() const {
    return is_finished_;
}

TweenUnit& TweenUnit::ease(EaseType ease_type) {
    ease_type_ = ease_type;
    return *this;
}

float TweenUnit::apply_ease(float value, EaseType ease_type) {
    switch (ease_type) {
    case EaseType::Linear:         return value;
    case EaseType::EaseInBack:     return gsEasingInBack(value);
    case EaseType::EaseInCirc:     return gsEasingInCirc(value);
    case EaseType::EaseInCubic:    return gsEasingInCubic(value);
    case EaseType::EaseInExpo:     return gsEasingInExpo(value);
    case EaseType::EaseInQuad:     return gsEasingInQuad(value);
    case EaseType::EaseInQuart:    return gsEasingInQuart(value);
    case EaseType::EaseInQuint:    return gsEasingInQuint(value);
    case EaseType::EaseInSine:     return gsEasingInSine(value);
    case EaseType::EaseOutBack:    return gsEasingOutBack(value);
    case EaseType::EaseOutBounce:  return gsEasingOutBounce(value);
    case EaseType::EaseOutCirc:    return gsEasingOutCirc(value);
    case EaseType::EaseOutCubic:   return gsEasingOutCubic(value);
    case EaseType::EaseOutElastic: return gsEasingOutElastic(value);
    case EaseType::EaseOutExpo:    return gsEasingOutExpo(value);
    case EaseType::EaseOutQuad:    return gsEasingOutQuad(value);
    case EaseType::EaseOutQuart:   return gsEasingOutQuart(value);
    case EaseType::EaseOutQuint:   return gsEasingOutQuint(value);
    case EaseType::EaseOutSine:    return gsEasingOutSine(value);
    case EaseType::EaseInOutBack:  return gsEasingInOutBack(value);
    case EaseType::EaseInOutCirc:  return gsEasingInOutCirc(value);
    case EaseType::EaseInOutCubic: return gsEasingInOutCubic(value);
    case EaseType::EaseInOutExpo:  return gsEasingInOutExpo(value);;
    case EaseType::EaseInOutQuad:  return gsEasingInOutQuad(value);
    case EaseType::EaseInOutQuart: return gsEasingInOutQuart(value);
    case EaseType::EaseInOutQuint: return gsEasingInOutQuint(value);
    case EaseType::EaseInOutSine:  return gsEasingInOutSine(value);
    default: return value;
    }
}

const std::string& TweenUnit::name() const {
    return name_;
}

TweenUnit& TweenUnit::name(const std::string& name) {
    name_ = name;
    return *this;
}