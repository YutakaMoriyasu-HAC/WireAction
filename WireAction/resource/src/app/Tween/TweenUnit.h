#ifndef TWEEN_UNIT_H_
#define TWEEN_UNIT_H_

#include <string>
#include <functional>
#include <gslib.h>
#include <GSeasing.h>

// �C�[�W���O���
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

// Tween�̓���1���Ǘ�����N���X�̊��N���X
class TweenUnit {
public:
    // �R���X�g���N�^
    TweenUnit(float duration);
    // ���z�f�X�g���N�^
    virtual ~TweenUnit() = default;
    // �X�V����
    void update(float delta_time);
    // �I���������ǂ���
    bool is_finished() const;
    // �C�[�W���O��ʂ�ݒ肷��
    TweenUnit& ease(EaseType ease_type);
    // ���O���擾����
    const std::string& name() const;
    // ���O��ݒ肷��
    TweenUnit& name(const std::string& name);
    // �R�s�[�֎~
    TweenUnit(const TweenUnit& other) = delete;
    TweenUnit& operator = (const TweenUnit& other) = delete;

protected:
    // �h���N���X���Ƃ̍X�V����
    virtual void on_update(float progress) {}

private:
    float duration_{ 0.0f };        // �A�j���[�V�������ԁi�t���[���j
    float elapsed_{ 0.0f };         // �o�ߎ��ԁi�t���[���j
    bool is_finished_{ false };     // �I��������
    EaseType ease_type_{ EaseType::Linear };  // �C�[�W���O���
    std::string name_{};            // ���O�i�L�����Z�������Ɏg���j

    // �l�ɃC�[�W���O��K�p����
    static float apply_ease(float value, EaseType ease_type);
};

#endif