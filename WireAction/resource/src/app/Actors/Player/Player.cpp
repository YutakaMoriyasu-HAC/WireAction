#include "Player.h"
#include "app/Worlds/IWorld.h"
#include "app/Field/Field.h"
#include "app/Ray/Line.h"
#include "app/NameList/Assets.h"

//���[�V�����ԍ�
enum {
	MotionIdle = 1,     // �A�C�h��
	MotionForwardWalk = 2,     // �O�i
	MotionBackwardWalk = 3,     // ���
	MotionLeftWalk = 4,     // ������
	MotionRightWalk = 5,     // �E����
	MotionFire = 11,    // �ˌ�
	MotionDamage = 14,    // �_���\�W
	MotionJump = 17     // �W�����v
};

//�ړ����x
const float WalkSpeed{ 0.1f }; //0.025
//�����̍���
const float PlayerHeight{ 1.0f };
//�Փ˔���p�̔��a
const float PlayerRadius{ 0.5f };
//�����̃I�t�Z�b�g
const float FootOffset{ 0.1f };
//�d�͒l
const float Gravity{ -0.016f };

//�R���X�g���N�^
Player::Player(IWorld* world, const GSvector3& position) :
	mesh_{ Mesh_Player, MotionIdle, true },
	motion_{ MotionIdle },
	motion_loop_{ true },
	state_{ State::Move },
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
}

//�X�V
void Player::update(float delta_time) {
	//��Ԃ̍X�V
	update_state(delta_time);
	//�d�͒l���X�V
	velocity_.y += Gravity * delta_time;
	//�d�͂�������
	transform_.translate(0.0f, velocity_.y, 0.0f);
	//�t�B�[���h�Ƃ̏Փ˔���
	collide_field();
	//���[�V������ύX
	mesh_.change_motion(motion_, motion_loop_);
	//���b�V���̃��[�V�������X�V
	mesh_.update(delta_time);
	//���[���h�ϊ��s���ݒ�
	mesh_.transform(transform_.localToWorldMatrix());
}

//�`��
void Player::draw() const {
	//���b�V����`��
	mesh_.draw();
	//�Փ˔��苅�̃f�o�b�O�\��
	collider().draw();
}

//�Փ˃��A�N�V����
void Player::react(Actor& other) {
	//�_���[�W���̏ꍇ�͉������Ȃ�
	if (state_ == State::Damage) return;
	//�G�̍U������ƏՓ˂������H
	if (other.tag() == "EnemyAttackTag") {
		//�^�[�Q�b�g�����̃x�N�g�������߂�
		GSvector3 to_target = other.transform().position() - transform().position();
		//y�����͖����ɂ���
		to_target.y = 0.0f;
		//�^�[�Q�b�g�����Ƌt�����Ƀm�b�N�o�b�N����ړ��ʂ����߂�
		velocity_ = -to_target.getNormalized() * 0.4f;
		//�_���[�W��ԂɑJ�ڂ���
		change_state(State::Damage, MotionDamage, false);
		return;
	}
	//�G�ƏՓ˂������H
	if (other.tag() == "EnemyTag") {
		//�A�N�^�[���m���d�Ȃ�Ȃ��悤�ɕ␳����
		collide_actor(other);
	}

	
}

//��Ԃ̍X�V
void Player::update_state(float delta_time) {
	//��ԑJ��
	switch (state_) {
	case State::Move:	move(delta_time);	break;
	case State::Attack:	attack(delta_time);	break;
	case State::Damage:	damage(delta_time);	break;
	}
	//��ԃ^�C�}�̍X�V
	state_timer_ += delta_time;
}

//��Ԃ̕ύX
void Player::change_state(State state, GSuint motion, bool loop) {
	motion_ = motion;
	motion_loop_ = loop;
	state_ = state;
	state_timer_ = 0.0f;
}

//�ړ�����
void Player::move(float delta_time) {

	//�X�y�[�X�L�[�Œe������
	if (gsGetKeyState(GKEY_SPACE)) {
		//�U����ԂɑJ�ڂ���
		change_state(State::Attack, MotionFire);
		
		return;
	}

	//�J�����̑O�����x�N�g�����擾
	GSvector3 forward = world_->camera()->transform().forward();
	forward.y = 0.0f;
	//�J�����̉E�����x�N�g�����擾
	GSvector3 right = world_->camera()->transform().right();
	right.y = 0.0f;
	//�L�[�̓��͒l����ړ��x�N�g�����v�Z
	GSvector3 velocity{ 0.0f,0.0f,0.0f };
	if (gsGetKeyState(GKEY_W)) velocity +=  forward;
	if (gsGetKeyState(GKEY_S)) velocity += -forward;
	if (gsGetKeyState(GKEY_A)) velocity += -right;
	if (gsGetKeyState(GKEY_D)) velocity +=  right;
	velocity = velocity.normalized() * WalkSpeed * delta_time;

	//�ړ����ĂȂ���΃A�C�h�����
	GSuint motion{ MotionIdle };
	//�ړ����Ă��邩�H
	if (velocity.length() != 0.0f) {
		//�����̕��
		GSquaternion rotation =
			GSquaternion::rotateTowards(
				transform_.rotation(),
				GSquaternion::lookRotation(velocity), 12.0f * delta_time);
		transform_.rotation(rotation);

		//�ړ����̃��[�V�����ɂ���
		motion = MotionForwardWalk;
	}
	
	//���[�V�����̕ύX
	change_state(State::Move, motion);

	//�ړ��ʂ�xz���������X�V
	velocity_.x = velocity.x;
	velocity_.z = velocity.z;

	//���s�ړ�����(���[�J�����W�n�)
	transform_.translate(velocity_, GStransform::Space::World);
	
}

//�U����
void Player::attack(float delta_time) {
	//�U�����[�V�����̏I����҂�
	if (state_timer_ >= mesh_.motion_end_time()) {
		move(delta_time);
	}
}

//�_���[�W��
void Player::damage(float delta_time) {
	//�m�b�N�o�b�N����(���[���h���W�n)
	transform_.translate(velocity_ * delta_time, GStransform::Space::World);
	//����������
	velocity_ -= GSvector3{ velocity_.x,0.0f,velocity_.z } * 0.5f * delta_time;

	//�_���[�W���[�V�����̏I����҂�
	if (state_timer_ >= mesh_.motion_end_time()) {
		move(delta_time);
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
	}

	//�n�ʂƂ̏Փ˔���(�����Ƃ̌�������)
	GSvector3 position = transform_.position();
	Line line;
	line.start	= position + collider_.center;
	line.end = position + GSvector3{ 0.0f,-FootOffset, 0.0f };
	GSvector3 intersect;	//�n�ʂƂ̌�_

	if (world_->field()->collide(line, &intersect)) {
		//��_�̈ʒu����y���W�̂ݕ␳����
		position.y = intersect.y;
		//���W��ύX����
		transform_.position(position);
		//�d�͂�����������
		velocity_.y = 0.0f;
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
	float length = collider_.radius + other.collider().radius;
	//�Փ˔��苅�̏d�Ȃ��Ă��钷�������߂�
	float overlap = length - distance;
	//�d�Ȃ��Ă��镔���̔����̋������������ړ��ʂ����߂�
	GSvector3 v = (position - target).getNormalized() * overlap * 0.5f;
	transform_.translate(v, GStransform::Space::World);
	//�t�B�[���h�Ƃ̏Փ˔���
	collide_field();
}

