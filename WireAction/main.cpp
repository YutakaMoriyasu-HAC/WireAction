#include <GSgame.h>

//マイゲームクラス
class MyGame : public gslib::Game {

};

//main関数
int main() {
	return MyGame().run();
}