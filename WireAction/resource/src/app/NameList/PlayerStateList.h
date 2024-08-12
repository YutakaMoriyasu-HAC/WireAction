#pragma once

enum PlayerStateList {

	State_Walk,
	State_Jump,
	

};

enum PlayerMotionList {
	MotionIdle = 1,     // アイドル
	MotionForwardWalk = 2,     // 前進
	MotionBackwardWalk = 3,     // 後退
	MotionLeftWalk = 4,     // 左歩き
	MotionRightWalk = 5,     // 右歩き
	MotionFire = 11,    // 射撃
	MotionDamage = 14,    // ダメ―ジ
	MotionJump = 17     // ジャンプ
};

enum PlayerObjectList {
	
};