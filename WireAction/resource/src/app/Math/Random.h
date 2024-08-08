#ifndef APP_MATH_RANDOM_H_
#define APP_MATH_RANDOM_H_

#include <random>

namespace math {
    class Random {
    public:
        // ゲーム中で唯一のインスタンス
        static std::random_device rand;// 非決定的な乱数生成器
        static std::mt19937 random; // メルセンヌ・ツイスタの32ビット版

        // 初期化（シード指定無し）
        static unsigned int init()
        {
            unsigned int seed = rand();
            random.seed(seed); // 乱数をシードにすることで毎回ランダムにする
            return seed;
        }

        // 初期化（シードを指定）
        static void init(int seed)
        {
            random.seed(seed);
        }

        template <class T>
        // 指定した範囲の乱数を取得する(maxは含まれない)
        const static T range(const T min, const T max) {
            std::uniform_real_distribution<> randRange(min, max);        // [0, 99] 範囲の一様乱数
            return (T)randRange(random);
        }
    };
}

#endif
