#include "GameManager.h"

int GameManager::coin_{ 0 };

//�R�C���������擾
const int GameManager::HowMuchCoins() {
	return coin_;
}

//�R�C������
const void GameManager::CoinPlus(int num) {
	coin_ += num;
	if (coin_ < 0) {
		coin_ = 0;
	}
}