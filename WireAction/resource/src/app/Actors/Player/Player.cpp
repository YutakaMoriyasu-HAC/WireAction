#include "Player.h"
#include "app/Worlds/IWorld.h"
#include "app/Field/Field.h"
#include "app/Ray/Line.h"
#include "app/NameList/Assets.h"
#include "app/Namelist/PlayerStateList.h"
#include "PlayerMoveState.h"
#include "PlayerJumpState.h"
#include "PlayerThrowWireState.h"
#include "PlayerPendulumState.h"
#include "PlayerQuickWireState.h"
#include "PlayerCrouchState.h"
#include "PlayerDamageState.h"
#include "PlayerBodyAttackState.h"
#include "app/Input/InputManager.h"
#include "app/Actors/WireBeam/WireBeam.h"
#include "app/Input/GameManager.h"
#include <imgui/imgui.h>


//�ړ����x
const float WalkSpeed{ 0.1f }; //0.025
//�����̍���
const float PlayerHeight{ 1.0f };
//�Փ˔���p�̔��a
const float PlayerRadius{ 0.5f };
//�����̃I�t�Z�b�g
//const float FootOffset{ 0.1f };


//�~�X�ƂȂ�y���W
const float MissPosition_{ -30.0f };
//�󒆃W�����v�̍ő吔
const int MaxAirJumpNum_{ 1 };


//�R���X�g���N�^
Player::Player(IWorld* world, const GSvector3& position) :
	mesh_{ Mesh_Player, 0, true },
	motion_{ Motion_Idle },
	motionLoop_{ true },
	state_timer_{ 0.0f } {
	//���[���h��ݒ�
	world_ = world;
	//�^�O���̐ݒ�
	tag_ = "PlayerTag";
	//���O�̐ݒ�
	name_ = "Player";
	//�Փ˔��苅�̐ݒ�
	collider_ = BoundingSphere{ PlayerRadius, GSvector3{0.0f,PlayerHeight,0.0f} };
	//���W�̏�����
	transform_.position(position);
	//���b�V���̕ϊ��s���������
	mesh_.transform(transform_.localToWorldMatrix());

	InitState();
	lerpSize_ = mesh_.DefaultLerpTime;
	lastPosition_ = position;

	//�󒆃W�����v�񐔃��Z�b�g
	airJumpNum_ = MaxAirJumpNum_;

	
}

//�X�V
void Player::update(float delta_time) {
	//���͏���
	ControllerUpdate();

	//�t�B�[���h�Ƃ̏Փ˔���
	collide_field();
	
	
	//���[�V������ύX
	mesh_.change_motionS(motion_, motionLoop_,animSpeed_,lerpSize_, startTime_, motionEndTimePlus_);
	//���b�V���̃��[�V�������X�V
	mesh_.update(delta_time);
	//���[���h�ϊ��s���ݒ�
	mesh_.transform(transform_.localToWorldMatrix());
	//�X�e�[�g�}�V���𓮂���
	stateMachine_.update();

	footOffset_ = velocity_.y;

	//�ړ�
	SetPosition(transform_.position() + velocity_);
	
	if (stateMachine_.getNowStateID() == State_QuickWire||
		stateMachine_.getNowStateID() == State_BodyAttack ||
		GetMotionState()==Motion_Rolling) {
		attackState_ = true;
	}
	else {
		attackState_ = false;
	}

	//�ޗ��ɗ������Ƃ�
	if (transform_.position().y < MissPosition_) {
		velocity_ = GSvector3::zero();
		transform_.position(lastPosition_);
		GameManager::CoinPlus(-5); //�R�C��-5������
	}

	//���G����
	if (invincibleTimer_ > 0) {
		invincibleTimer_ -= delta_time;
		if (invincibleTimer_ <= 0) {
			invincibleTimer_ = 0;
			isInvincible_ = false;
		}
	}

}

void Player::lateUpdate(float delta_time) {



	//�t���[���̍Ō�ɍ��̃t���[���̓��͏�Ԃ��L�^
	input2_ = input_;

	static float velocityAngle{ debugFloat_[0]};
	static float inputAngle{ debugFloat_[1] };
	static float hikiAngle{ debugFloat_[2] };

	ImGui::Begin("Debug");

	ImGui::Text("motionEnd:%s", IsMotionEnd() ? "true" : "false");
	ImGui::Text("velocityY:%f", velocity_.y);
	ImGui::Text("centerPendulumX:%f", centerPendulum_.x);
	ImGui::Text("playerz:%f", transform_.position().z);
	ImGui::Text("direction(deg):%f", debugFloat_[0]);
	ImGui::Text("direction(now):%f", debugFloat_[1]);
	ImGui::Text("movespeed:%f", debugFloat_[2]);
	ImGui::Text("%f", abs(180 - abs(debugFloat_[0] - debugFloat_[1])));
	ImGui::Text("isWall:%d", isWall_);
	ImGui::Text("trampled:%d", trampled_);
	ImGui::Text("motionTime:%f", mesh_.motion_time());
	ImGui::Text("motioin:%d", motion_);
	ImGui::Text("motioinEndTime:%f", mesh_.motionEndTime());
	ImGui::Text("motionEnd:%s", canWallKick_ ? "true" : "false");
	ImGui::Text("coin:%d",GameManager::HowMuchCoins());
	ImGui::Text("px:%f", p.x);
	ImGui::Text("py:%f", p.y);
	ImGui::Text("pz:%f", p.z);


	ImGui::End();
	
}

//�`��
void Player::draw() const {
	
	//�Փ˔��苅�̃f�o�b�O�\��
	//collider().draw();
	stateMachine_.draw();
	
	//���b�V����`��
	mesh_.draw();
	
}

//�Փ˃��A�N�V����
void Player::react(Actor& other) {
	
	if (isInvincible_)return;

	//�G�ƏՓ˂������H
	if (other.tag() == "EnemyTag") {

		isCollide_ = true;

		//�n�ʂƂ̏Փ˔���(�����Ƃ̌�������)
		GSvector3 position = transform_.position();
		

		if (other.transform().position().y<position.y+1.5f && !isGround_) {
			attackState_ = true;
			trampled_ = true;
			return;
		}
		else {
			//�_���[�W
			if (!attackState_) {
				changeState(PlayerStateList::State_Damage);
				isCollide_ = false;
				if (stateMachine_.getNowStateID() == PlayerStateList::State_Walk) {
					lastPosition_ = transform_.position();
				}
				//���G�ɂȂ�
				startInvincibilityTime();
				
				return;
			}
		}
	}
	else {
		isCollide_ = false;
	}

	
}

// ���[�V�������I�����Ă��邩
const bool Player::IsMotionEnd() const
{
	//return stateMachine_.getNowStateTimer() >= GetMotionEndTime();
	return mesh_.is_end_motion();
}

//���[�V�����ύX�A���x���ύX
void Player::ChangeMotionS(GSuint motion, bool loopFlag, float speed, float lerp, float endTime, float startTime)
{
	motion_ = motion;
	motionLoop_ = loopFlag;
	state_timer_ = 0.0f;
	
	startTime_ = startTime;
	animSpeed_ = speed;
	lerpSize_ = lerp;
	motionEndTimePlus_ = endTime;

}

// ���[�V�����̏I�����Ԃ��擾
const float Player::GetMotionEndTime() const
{
	return mesh_.motionEndTime() - motionEndTimePlus_;
}

//���[�V���������[�v���邩
const bool Player::IsMotionRoop() const
{
	return motionLoop_;
}

//���[�V�������Ԃ������Z�b�g
void Player::motionTimeReset() {
	mesh_.motion_time(0.0f);
}

//���W��Ԃ�
const GSvector3 Player::GetPosition() const
{
	return transform_.position();
}

//���W�Ƀ��[�v
void Player::SetPosition(GSvector3 pos)
{
	transform_.position(pos);
}

//�Đ����̃A�j����Ԃ�
const int Player::GetMotionState() const {
	return motion_;
}



//�_���[�W��
void Player::damage(float delta_time) {
	//�m�b�N�o�b�N����(���[���h���W�n)
	transform_.translate(velocity_ * delta_time, GStransform::Space::World);
	//����������
	velocity_ -= GSvector3{ velocity_.x,0.0f,velocity_.z } * 0.5f * delta_time;

	//�_���[�W���[�V�����̏I����҂�
	if (state_timer_ >= mesh_.motionEndTime()) {
		//move(delta_time);
	}
}

//�t�B�[���h�Ƃ̏Փ˔���
void Player::collide_field() {
	//�ǂƂ̏Փ˔���(���̂Ƃ̔���)
	GSvector3 center;	//�Փˌ�̋��̂̒��S���W
	if (world_->field()->collide(collider(), &center)) {
		//y���W�͕ύX���Ȃ�
		center.y = transform_.position().y;
		//�␳��̍��W�ɕύX����
		transform_.position(center);
		isWall_ = true;

		GSvector3 position = transform_.position();
		Line wallLine;
		wallLine.start = position + collider_.center_;
		wallLine.end = position + collider_.center_ + GetInput();
		GSvector3 intersect;	//�n�ʂƂ̌�_
		
		if (world_->field()->collide(wallLine, &intersect)) {
			canWallKick_ = true;
			wallKickPosition_ = intersect;
		}
		else {
			canWallKick_ = false;
		}
		

	}
	else {
		isWall_ = false;
		canWallKick_ = false;
	}

	//�n�ʂƂ̏Փ˔���(�����Ƃ̌�������)
	GSvector3 position = transform_.position();
	Line line;
	line.start	= position + collider_.center_;
	line.end = position + GSvector3{ 0.0f,footOffset_, 0.0f };
	GSvector3 intersect;	//�n�ʂƂ̌�_

	GSplane plane; //�@���̏��
	if (world_->field()->collide(line, &intersect, &plane) && footOffset_<0) {
		//��_�̈ʒu����y���W�̂ݕ␳����
		position.y = intersect.y;
		//���W��ύX����
		transform_.position(position);
		//�d�͂�����������
		velocity_.y = 0.0f;
		isGround_ = true;
		p = plane.normal;

		//�󒆃W�����v�񐔃��Z�b�g
		airJumpNum_ = MaxAirJumpNum_;
	}
	else {
		isGround_ = false;
	}
	

}



//�X�e�[�g�Ǘ�
void Player::InitState() {
	stateMachine_.addState(PlayerStateList::State_Walk, std::make_shared<PlayerMoveState>(this, world_, &stateMachine_));
	stateMachine_.addState(PlayerStateList::State_Jump, std::make_shared<PlayerJumpState>(this, world_, &stateMachine_));
	stateMachine_.addState(PlayerStateList::State_ThrowWire, std::make_shared<PlayerThrowWireState>(this, world_, &stateMachine_));
	stateMachine_.addState(PlayerStateList::State_Pendulum, std::make_shared<PlayerPendulumState>(this, world_, &stateMachine_));
	stateMachine_.addState(PlayerStateList::State_QuickWire, std::make_shared<PlayerQuickWireState>(this, world_, &stateMachine_));
	stateMachine_.addState(PlayerStateList::State_Crouch, std::make_shared<PlayerCrouchState>(this, world_, &stateMachine_));
	stateMachine_.addState(PlayerStateList::State_Damage, std::make_shared<PlayerDamageState>(this, world_, &stateMachine_));
	stateMachine_.addState(PlayerStateList::State_BodyAttack, std::make_shared<PlayerBodyAttackState>(this, world_, &stateMachine_));


	// �X�e�[�g�}�V���̏�����
	stateMachine_.reset(PlayerStateList::State_Walk);
}

//�X�e�[�g�ύX
void Player::changeState(const int state) {
	stateMachine_.chengeState(state);
}

//���͏���
void Player::ControllerUpdate() {
	// ����
	input_ = MoveInput().normalized();
}

//�Ō�Ɍ����Ă���������Ԃ�
const GSvector3 Player::GetInputDirection() const {
	return input_Direction_;
}

//�Ō�Ɍ����Ă����������Z�b�g
void Player::SetInputDirection(GSvector3 direction) {
	input_Direction_ = direction;
}

//���͂�����Γ��͕������A�Ȃ���Ό����Ă������Ԃ�
const GSvector3 Player::GetInput() const {
	if (input_ != GSvector3::zero()) {
		return input_;
	}
	else {
		return GetInputDirection();
	}
}

// ����
GSvector3 Player::MoveInput()
{
	GSvector3 velocity{ 0.0f,0.0f,0.0f };
	// �J�����̕����x�N�g�����擾
	GSvector3 forward = world_->camera()->transform().forward();
	//GSvector3 forward = GSvector3(0, 0, -1);
	forward.y = 0.0f;
	// �J�����̉E���x�N�g�����擾
	GSvector3 right = world_->camera()->transform().right();
	right.y = 0.0f;
	GSvector2 vec2 = InputManager::Move();
	velocity += vec2.y * forward;
	velocity += vec2.x * right;
	return velocity;
}

bool Player::isGround() {
	return isGround_;
}

bool Player::isWall() {
	return isWall_;
}

//�����_�擾
GSvector3 Player::getCameraLookPoint() {
	return cameraLookPoint_;
}

//�����_�Z�b�g
void Player::setCameraLookPoint(GSvector3 point) {
	cameraLookPoint_ = point;
}

void Player::setDebugFloat(int num, float value) {
	debugFloat_[num] = value;
}

//����
void Player::spone(int actorListNum) {

	

	switch (actorListNum) {
	case Actor_WireBeam:
		if (isThrowing_)break;
		world_->add_actor(std::make_shared<WireBeam>(world_, beamDirection_, beamDirection_ - transform_.position(), "Beam", 30.0f /*�L�΂����� */ , 1.0f, transform_.rotation(),this));
		isThrowing_ = true;
		break;
	}
}

void Player::setBeamDirection(GSvector3 direction) {
	beamDirection_ = transform_.position() + direction;
	beamDirection_.y = transform_.position().y + 1.5f;
}



void Player::setCenterPendulum(GSvector3 center) {
	centerPendulum_ = center;
}

GSvector3 Player::getCenterPendulum() {
	return centerPendulum_;
}

void Player::setDebugMoveSpeed(float speed) {
	debugMoveSpeed_ = speed;
}

//�����Ă�
void Player::setThrowing(bool throwing) {
	isThrowing_ = throwing;
}

//�����Ă�
bool Player::getThrowing() {
	return isThrowing_;
}

//�A�j���[�V�����^�C�}���Z�b�g
void Player::resetStateTimer() {
	stateMachine_.resetTimer();
}

//�G�ɐG��Ă邩
bool Player::isCollide() {
	return isCollide_;
}

void Player::changeMesh(GSuint model) {
	mesh_.changeModel(model);
	
}

void Player::SetDebugFloat(float a, float b, float c, float d, float e) {
	debugFloat_[0] = a;
	debugFloat_[1] = b;
	debugFloat_[2] = c;
	debugFloat_[3] = d;
	debugFloat_[4] = e;
}

bool Player::isTrampled(int trampleSwitch) {
	bool re = trampled_;
	if(trampleSwitch>=0)trampled_ = trampleSwitch;


	return re;
}

//�ǃL�b�N���ɕǂ̍��W��Ԃ�
bool Player::canWallKick() {
	//kickPosition = &wallKickPosition_;
	return canWallKick_;
}

void Player::setLastPosition(GSvector3 position) {
	lastPosition_ = position;
}

//���G���ԊJ�n
void Player::startInvincibilityTime() {
	invincibleTimer_ = 120.0f;
	isInvincible_ = true;
}

//�󒆃W�����v�\�����f���ĉ�-1
bool Player::canAirJump() {
	if (airJumpNum_ > 0) {
		airJumpNum_ -= 1;
		return true;
	}
	else {
		return false;
	}
}