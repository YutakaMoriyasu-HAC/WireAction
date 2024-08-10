#include "CameraRotateAround.h"
#include "app/Worlds/IWorld.h"
#include "app/Field/Field.h"
#include "app/Ray/Line.h"

//�v���C���[����̑��΍��W(z���W�̂�)
const GSvector3 PlayerOffset{ 0.0f, 0.0f, -5.0f };
//�J�����̒����_�̕␳�l
const GSvector3 ReferencePointOffset{ 0.0f,1.8f,0.0f };

//�R���X�g���N�^
CameraRotateAround::CameraRotateAround(
	IWorld* world, const GSvector3& position, const GSvector3& at)
{
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

	//x������̉�]�p�x�̏�����
	pitch_ = (at - position).getPitch();
	//y������̉�]�p�x�̏�����
	yaw_ = (at - position).getYaw();
}

//�X�V
void CameraRotateAround::update(float delta_time) {
	//�v���C���[������
	Actor* player = world_->find_actor("Player");
	if (player == nullptr) return;

	//y���܂��ɃJ��������]������
	if (gsGetKeyState(GKEY_LEFT))	yaw_ -= 1.0f * delta_time;
	if (gsGetKeyState(GKEY_RIGHT))	yaw_ += 1.0f * delta_time;
	//x���܂��ɃJ��������]������
	if (gsGetKeyState(GKEY_UP))		pitch_ -= 1.0f * delta_time;
	if (gsGetKeyState(GKEY_DOWN))	pitch_ += 1.0f * delta_time;
	//x���܂��̉�]�p�x�̐���������
	pitch_ = CLAMP(pitch_, -10.0f, 30.0f);

	

	//�����_�̍��W�����߂�
	GSvector3 at = player->transform().position() + ReferencePointOffset;
	//�J�����̍��W�����߂�
	GSvector3 position = at + GSquaternion::euler(pitch_, yaw_, 0.0f) * PlayerOffset;

	//�t�B�[���h�Ƃ̏Փ˔���
	Line line{ at, position };
	GSvector3 intersect;
	if (world_->field()->collide(line, &intersect)) {
		position = intersect;
	}

	//���W�̐ݒ�
	transform_.position(position);
	//�����_�̕���������
	transform_.lookAt(at);
}

//�`��
void CameraRotateAround::draw() const {
	//���_�̈ʒu
	GSvector3 eye = transform_.position();
	//�����_�̈ʒu
	GSvector3 at = eye + transform().forward();
	//���_�̏����
	GSvector3 up = transform_.up();

	//�J�����̐ݒ�
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		eye.x, eye.y, eye.z,	//���_�̈ʒu
		at.x, at.y, at.z,		//�����_�̈ʒu
		up.x, up.y, up.z		//���_�̏����
	);
}