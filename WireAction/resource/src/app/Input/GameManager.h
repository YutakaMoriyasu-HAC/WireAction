#ifndef GAME_MANAGER_H_
#define GAME_MANAGER_H_

#include <gslib.h>

// ���͊Ǘ��N���X
static class GameManager
{
public:
	const static int HowMuchCoins(); //�R�C���������擾

	const static void CoinPlus(int num); //�R�C���̑���


private:
	//�J�����̍��E���]
	static int coin_;
};

#endif