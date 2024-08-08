#include "Random.h"

std::random_device math::Random::rand;// 非決定的な乱数生成器
std::mt19937 math::Random::random; // メルセンヌ・ツイスタの32ビット版