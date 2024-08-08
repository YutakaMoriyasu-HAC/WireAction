#ifndef APP_MATH_RANDOM_H_
#define APP_MATH_RANDOM_H_

#include <random>

namespace math {
    class Random {
    public:
        // �Q�[�����ŗB��̃C���X�^���X
        static std::random_device rand;// �񌈒�I�ȗ���������
        static std::mt19937 random; // �����Z���k�E�c�C�X�^��32�r�b�g��

        // �������i�V�[�h�w�薳���j
        static unsigned int init()
        {
            unsigned int seed = rand();
            random.seed(seed); // �������V�[�h�ɂ��邱�ƂŖ��񃉃��_���ɂ���
            return seed;
        }

        // �������i�V�[�h���w��j
        static void init(int seed)
        {
            random.seed(seed);
        }

        template <class T>
        // �w�肵���͈̗͂������擾����(max�͊܂܂�Ȃ�)
        const static T range(const T min, const T max) {
            std::uniform_real_distribution<> randRange(min, max);        // [0, 99] �͈͂̈�l����
            return (T)randRange(random);
        }
    };
}

#endif
