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
#include "app/Input/InputManager.h"
#include "app/Actors/WireBeam/WireBeam.h"
#include <imgui/imgui.h>


//�ړ����x
const float WalkSpeed{ 0.1f }; //0.025
//�����̍���
const float PlayerHeight{ 1.0f };
//�Փ˔���p�̔��a
const float PlayerRadius{ 0.5f };
//�����̃I�t�Z�b�g
//const float FootOffset{ 0.1f };
//�d�͒l
const float Gravity{ -0.016f }; //-0.016 //-0.008


//�R���X�g���N�^
Player::Player(IWorld* world, const GSvector3& position) :
	mesh_{ Mesh_Player, 0, true },
	motion_{ Motion_Idle },
	motion_loop_{ true },
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

	
}

//�X�V
void Player::update(float delta_time) {
	//���͏���
	ControllerUpdate();

	//�t�B�[���h�Ƃ̏Փ˔���
	collide_field();
	
	
	//���[�V������ύX
	mesh_.change_motionS(motion_, motion_loop_,animSpeed_,lerpSize_, startTime_);
	//���b�V���̃��[�V�������X�V
	mesh_.update(delta_time);
	//���[���h�ϊ��s���ݒ�
	mesh_.transform(transform_.localToWorldMatrix());
	//�X�e�[�g�}�V���𓮂���
	stateMachine_.update();

	footOffset_ = velocity_.y;

	//�ړ�
	SetPosition(transform_.position() + velocity_);
	
	if (stateMachine_.getNowStateID() == State_QuickWire) {
		attackState_ = true;
	}
	else {
		attackState_ = false;
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
	
	//�G�ƏՓ˂������H
	if (other.tag() == "EnemyTag") {

		isCollide_ = true;

		//�n�ʂƂ̏Փ˔���(�����Ƃ̌�������)
		GSvector3 position = transform_.position();
		

		if (other.transform().position().y<position.y && footOffset_ < 0 && !isGround_) {
			attackState_ = true;
			trampled_ = true;
			return;
		}
		else {
			//����Â��ĂȂ��I
			attackState_ = false;
			//�_���[�W
			if (stateMachine_.getNowStateID() != State_QuickWire) {
				changeState(PlayerStateList::State_Damage);
				isCollide_ = false;
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
	return stateMachine_.getNowStateTimer() >= GetMotionEndTime();
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
	}
	else {
		isWall_ = false;
	}

	//�n�ʂƂ̏Փ˔���(�����Ƃ̌�������)
	GSvector3 position = transform_.position();
	Line line;
	line.start	= position + collider_.center_;
	line.end = position + GSvector3{ 0.0f,footOffset_, 0.0f };
	GSvector3 intersect;	//�n�ʂƂ̌�_

	if (world_->field()->collide(line, &intersect) && footOffset_<0) {
		//��_�̈ʒu����y���W�̂ݕ␳����
		position.y = intersect.y;
		//���W��ύX����
		transform_.position(position);
		//�d�͂�����������
		velocity_.y = 0.0f;
		isGround_ = true;
	}
	else {
		isGround_ = false;
	}

}

//�A�N�^�[�Ƃ̏Փˏ���
void Player::collide_actor(Actor& other) {
	//y���W���������W�����߂�
	GSvector3 position = transform_.position();
	position.y = 0.0f;
	GSvector3 target = other.transform().position();
	target.y = 0.0f;
	//����Ƃ̋���
	float distance = GSvector3::distance(position, target);
	//�Փ˔��苅�̔��a���m�����������������߂�
	float length = collider_.radius_ + other.collider().radius_;
	//�Փ˔��苅�̏d�Ȃ��Ă��钷�������߂�
	float overlap = length - distance;
	//�d�Ȃ��Ă��镔���̔����̋������������ړ��ʂ����߂�
	GSvector3 v = (position - target).getNormalized() * overlap * 0.5f;
	transform_.translate(v, GStransform::Space::World);
	//�t�B�[���h�Ƃ̏Փ˔���
	collide_field();
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

void Player::gravityFall(float delta_time) {
	//�d�͒l���X�V
	velocity_.y += Gravity * delta_time;
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