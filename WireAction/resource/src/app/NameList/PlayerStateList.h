#pragma once

enum PlayerStateList {

	State_Walk,
	State_Jump,
	State_ThrowWire,
	State_Pendulum,
	State_QuickWire,
	State_Crouch,
	State_Damage,
	State_BodyAttack,

};

enum PlayerMotionList {
	Motion_BackRolling,
	Motion_Crouch,
	Motion_Damage,
	Motion_Rolling,
	Motion_BodyAttack,
	Motion_Brake,
	Motion_Idle,
	Motion_JumpIn,
	Motion_JumpNow,
	Motion_JumpOut,
	Motion_JumpSpin,
	Motion_Kick,
	Motion_RollingBrake,
	Motion_Run,
	Motion_RunL,
	Motion_RunR,
	Motion_Walk,
};

enum PlayerObjectList {
	Actor_WireBeam,
};