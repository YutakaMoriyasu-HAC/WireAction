#include "app/Actors/Actor/Actor.h"

//�X�V
void Actor::update(float) {}

//�x���X�V
void Actor::lateUpdate(float) {}

//�`��
void Actor::draw() const {}

//�������I�u�W�F�N�g�̕`��
void Actor::draw_transparent() const {}

//GUI�̕`��
void Actor::draw_gui() const {}

//�Փ˃��A�N�V����
void Actor::react(Actor&) {}

//���b�Z�[�W����
void Actor::handle_message(const std::string& message, void*param) {}

//�Փ˔���
void Actor::collide(Actor& other) {
	//�ǂ���̃A�N�^�[���Փ˔��肪�L�����H
	if (enable_collider_ && other.enable_collider_) {
		//�Փ˔��������
		if (is_collide(other)) {
			//�Փ˂����ꍇ�́A���݂��ɏՓ˃��A�N�V�������Ƃ�
			react(other);
			other.react(*this);
		}
	}
}

//�����Ƃ̏Փ˔���
bool Actor::collide(const Line& line, GSvector3* intersect) const {
	return collider().intersects(line, intersect);
}

//�A�N�^�[�Ƃ̏Փˏ���
void Actor::collide_actor(Actor& other) {
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
	transform_.translate(-v, GStransform::Space::World);
	//�t�B�[���h�Ƃ̏Փ˔���
	//collide_field();
}

//���S����
void Actor::die() {
	dead_ = true;
}

//�Փ˂��Ă��邩�H
bool Actor::is_collide(const Actor& other) const {
	return collider().intersects(other.collider());
}

//���S���Ă��邩�H
bool Actor::is_dead() const {
	return dead_;
}

//���O���擾
const std::string& Actor::name() const {
	return name_;
}

//�^�O�����擾
const std::string& Actor::tag() const {
	return tag_;
}

//�U����Ԃ��擾
const bool& Actor::attackState() const {
	return attackState_;
}

//�g�����X�t�H�[�����擾(const��)
const GStransform& Actor::transform() const {
	return transform_;
}

//�g�����X�t�H�[�����擾
GStransform& Actor::transform() {
	return transform_;
}

// �ړ��ʂ̎擾
const GSvector3& Actor::velocity() const {
	return velocity_;
}

GSvector3& Actor::velocity()
{
	return velocity_;
}

void Actor::velocity(const GSvector3& vec)
{
	velocity_ = vec;
}

//�Փ˔���f�[�^���擾
BoundingSphere Actor::collider() const {
	return collider_.transform(transform_.localToWorldMatrix());
}

TweenUnit& Actor::move_to(const GSvector3& to, float duration) {
	// ���݂̏ꏊ����w�肳�ꂽ�ꏊ�܂ŁATween�ňړ�����
	return Tween::vector3(transform_.position(), to, duration,
		[=](GSvector3 pos) {transform_.position(pos); });
}

void Actor::gravityFall(float delta_time) {
	//�d�͒l���X�V
	velocity_.y += Gravity * delta_time;
}