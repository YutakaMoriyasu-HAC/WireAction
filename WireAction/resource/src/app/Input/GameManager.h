#ifndef GAME_MANAGER_H_
#define GAME_MANAGER_H_

#include <gslib.h>

// 入力管理クラス
static class GameManager
{
public:
	const static int HowMuchCoins(); //コイン枚数を取得

	const static void CoinPlus(int num); //コインの増減


private:
	//カメラの左右反転
	static int coin_;
};

#endif