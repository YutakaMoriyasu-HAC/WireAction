#include "GameManager.h"

int GameManager::coin_{ 0 };

//コイン枚数を取得
const int GameManager::HowMuchCoins() {
	return coin_;
}

//コイン増減
const void GameManager::CoinPlus(int num) {
	coin_ += num;
	if (coin_ < 0) {
		coin_ = 0;
	}
}