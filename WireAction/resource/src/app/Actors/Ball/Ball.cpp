#include "Ball.h"
#include "app/NameList/Assets.h"

Ball::Ball(IWorld* world, const GSvector3& position) :mesh_{ Mesh_Ball, 0, true } {
	//���W�̏�����
	transform_.position(position);
	//���b�V���̕ϊ��s���������
	mesh_.transform(transform_.localToWorldMatrix());
	//�^�O
	tag_ = "WireObjectTag";
	name_ = "Ball";
	//�Փ˔��苅�̐ݒ�
	collider_ = BoundingSphere{ 1.0f };
}

void Ball::update(float delta_time) {



}

void Ball::draw() const {
	mesh_.draw();
	collider().draw();
}