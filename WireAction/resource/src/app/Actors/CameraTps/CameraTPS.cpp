#include "CameraTPS.h"
#include "app/Worlds/IWorld.h"
#include "app/Field/Field.h"
#include "app/Ray/Line.h"

//�v���C���[����̑��΍��W
const GSvector3 PlayerOffset{ 0.0f,3.0f,-5.0f };
//�J�����̒����_�̕␳�l
const GSvector3 ReferencePointOffset{ 0.0f,1.7f,0.0f };

//�R���X�g���N�^
CameraTPS::CameraTPS(IWorld* world, const GSvector3& position, const GSvector3& at) {
	//���[���h��ݒ�
	world_ = world;
	//�^�O�̐ݒ�
	tag_ = "CameraTag";
	//���O�̐ݒ�
	name_ = "Camera";
	//���_�̈ʒu��ݒ�
	transform_.position(position);
	//�����_��ݒ�(�����_�̕����Ɍ�����ς���)
	transform_.lookAt(at);
}

//�X�V
void CameraTPS::update(float delta_time) {
	//�v���C���[������
	Actor* player = world_->find_actor("Player");
	if (player == nullptr) return;
	//���_�̈ʒu�����߂�(�v���C���[�̔w��̍��W)
	GSvector3 position = PlayerOffset * player->transform().localToWorldMatrix();
	//�����_�̈ʒu�����߂�(�v���C���[�̓����̏����゠����̍��W)
	GSvector3 at = player->transform().position() + ReferencePointOffset;

	//�t�B�[���h�Ƃ̏Փ˔���
	Line line{ at, position };
	GSvector3 intersect;
	if (world_->field()->collide(line, &intersect)) {
		position = intersect;
	}

	//�X���[�X�_���v�ɂ�銊�炩�ȕ��
	const float SmoothTime{ 12.0f };	//��ԃt���[����
	const float MaxSpeed{ 1.0f };		//�ړ��X�s�[�h�̍ő�l
	position = GSvector3::smoothDamp(
		transform_.position(), position, velocity_,
		SmoothTime, MaxSpeed, delta_time);

	//���_�̈ʒu��ݒ�
	transform_.position(position);
	//�����_��ݒ�(�����_�̕����Ɍ�����ς���)
	transform_.lookAt(at);
}

//�`��
void CameraTPS::draw() const {
	//���_�̈ʒu
	GSvector3 eye = transform_.position();
	//�����_�̈ʒu
	GSvector3 at = eye + transform().forward();

	//�J�����̐ݒ�
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		eye.x, eye.y, eye.z,	//���_�̈ʒu
		at.x, at.y, at.z,		//�����_�̈ʒu
		0.0f, 1.0f, 0.0f		//���_�̏����
	);
}