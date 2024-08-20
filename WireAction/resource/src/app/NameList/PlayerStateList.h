#pragma once

enum PlayerStateList {

	State_Walk,
	State_Jump,
	State_ThrowWire,
	State_Pendulum,
	State_QuickWire,

};

enum PlayerMotionList {
	Motion_Idle,
	Motion_Walk,
	Motion_Dash,
	Motion_Avoid,
	Motion_LittleDamage,
	Motion_BigDamage,
	Motion_Die,
	Motion_Attack1,
	Motion_Attack2,
	Motion_Attack3,
	Motion_Attack4,
	Motion_Charge,
	Motion_ChargeWalk,
	Motion_RotatingSlash1,
	Motion_RotatingSlash1End,
	Motion_Counter,
	Motion_Chaege2,
	Motion_RotatingSlash2,
	Motion_RotatingSlash2End,
	Motion_RotatingSlash1_0,
	Motion_Jump,
	Motion_JumpAttack,
	Motion_Run,
	Motion_Attack5,
	Motion_Attack6,
	Motion_BackJump
};

enum PlayerObjectList {
	Actor_WireBeam,
};