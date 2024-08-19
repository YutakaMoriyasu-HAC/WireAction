#ifndef TWEEN_UNIT_H_
#define TWEEN_UNIT_H_

#include <string>
#include <functional>
#include <gslib.h>
#include <GSeasing.h>

// イージング種別
enum class EaseType {
    Linear,
    EaseInBack,
    EaseInCirc,
    EaseInCubic,
    EaseInExpo,
    EaseInQuad,
    EaseInQuart,
    EaseInQuint,
    EaseInSine,
    EaseOutBack,
    EaseOutBounce,
    EaseOutCirc,
    EaseOutCubic,
    EaseOutElastic,
    EaseOutExpo,
    EaseOutQuad,
    EaseOutQuart,
    EaseOutQuint,
    EaseOutSine,
    EaseInOutBack,
    EaseInOutCirc,
    EaseInOutCubic,
    EaseInOutExpo,
    EaseInOutQuad,
    EaseInOutQuart,
    EaseInOutQuint,
    EaseInOutSine,
};

// Tweenの動作1つを管理するクラスの基底クラス
class TweenUnit {
public:
    // コンストラクタ
    TweenUnit(float duration);
    // 仮想デストラクタ
    virtual ~TweenUnit() = default;
    // 更新処理
    void update(float delta_time);
    // 終了したかどうか
    bool is_finished() const;
    // イージング種別を設定する
    TweenUnit& ease(EaseType ease_type);
    // 名前を取得する
    const std::string& name() const;
    // 名前を設定する
    TweenUnit& name(const std::string& name);
    // コピー禁止
    TweenUnit(const TweenUnit& other) = delete;
    TweenUnit& operator = (const TweenUnit& other) = delete;

protected:
    // 派生クラスごとの更新処理
    virtual void on_update(float progress) {}

private:
    float duration_{ 0.0f };        // アニメーション時間（フレーム）
    float elapsed_{ 0.0f };         // 経過時間（フレーム）
    bool is_finished_{ false };     // 終了したか
    EaseType ease_type_{ EaseType::Linear };  // イージング種別
    std::string name_{};            // 名前（キャンセル処理に使う）

    // 値にイージングを適用する
    static float apply_ease(float value, EaseType ease_type);
};

#endif