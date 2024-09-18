#include "Enemy.h"
#include "app/NameList/Assets.h"
#include "EnemyMotionList.h"
#include "app/Worlds/IWorld.h"
#include "app/Math/Math.h"
#include "app/Field/Field.h"

Enemy::Enemy(IWorld* world, const GSvector3& position) :
	mesh_{ Mesh_EnemyBird, 0, true },
	motion_{ EnemyMotionList::Motion_Run }, 
	motion_loop_{true} {
	//���W�̏�����
	transform_.position(position);
	transform_.localScale(GSvector3(2, 2, 2));
	//���b�V���̕ϊ��s���������
	mesh_.transform(transform_.localToWorldMatrix());
	//�^�O
	tag_ = "EnemyTag";
	name_ = "EnemyBird";
	//�Փ˔��苅�̐ݒ�
	collider_ = BoundingSphere{ 0.4f , GSvector3{0.0f,0.5f,0.0f} };
	world_ = world;
	player_ = world_->find_actor("Player");
	velocity_ = GSvector3::zero();
}

void Enemy::update(float delta_time) {
	//���[�V������ύX
	mesh_.change_motionS(motion_, motion_loop_,1.0f);
	mesh_.update(delta_time);
	//���b�V���̕ϊ��s���������
	mesh_.transform(transform_.localToWorldMatrix());

	if (timer_ > 0) {
		timer_--;
		if (timer_ <= 0) {
			die();
		}
		return;
	}

	//�t�B�[���h�Ƃ̏Փ˔���
	collide_field();
	//���R����
	gravityFall(delta_time);
	
	targetPosition_ = player_->transform().position();

	//���������ꂽ��~�܂�
	if (math::radiusFromVector2(targetPosition_ - transform_.position()) >= 15.0f) {

		if (motion_ == Motion_Run) {
			motion_=Motion_Idle;
			velocity_ = GSvector3::zero();
		}
	}
	else {
		//�������瑖��o��
		if (motion_ == Motion_Idle) {
			motion_ = Motion_Run;
		}

		changeAngle(10.0f);
		GSvector3 directionToPlayer = targetPosition_ - transform_.position();
		direction_ = directionToPlayer * math::normal(directionToPlayer.x, directionToPlayer.z) * 0.1f;
		velocity_ = GSvector3(direction_.x, velocity_.y, direction_.z);
	}

	//�ړ�
	transform_.position(transform_.position() + velocity_);

	//�ޗ��ɗ������Ƃ�
	if (transform_.position().y < -30.0f) {
		die();
	}

	

}

void Enemy::draw() const {
	mesh_.draw();
	collider().draw();
	
}

//�I�u�W�F�N�g���m�̏Փ�
void Enemy::react(Actor& other) {

	if (other.name() == "Player" && other.attackState()) {
		
		transform_.localScale(GSvector3(2, 0.4, 2));
		timer_ = 50.0f;
		
	}
	if (other.tag() == "EnemyTag" ) {
		collide_actor(other);
		collide_field();
	}
}

//��]
void Enemy::changeAngle(float time) {
	GSvector3 target = targetPosition_ - transform_.position();
	target.y = 0;
	//�����̕��
	GSquaternion rotation =
		GSquaternion::rotateTowards(
			transform().rotation(),
			GSquaternion::lookRotation(target), time);
	transform().rotation(rotation);
}

//�t�B�[���h�Ƃ̏Փ˔���
void Enemy::collide_field() {
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
	line.start = position + collider_.center_;
	line.end = position + GSvector3{ 0.0f,-0.5f, 0.0f };;
	GSvector3 intersect;	//�n�ʂƂ̌�_

	if (world_->field()->collide(line, &intersect)) {
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