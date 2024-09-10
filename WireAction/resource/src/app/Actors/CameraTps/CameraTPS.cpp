#include "CameraTps.h"
#include "app/Worlds/IWorld.h"
#include "app/Ray/Line.h"
#include "app/Field/Field.h"
#include "app/Input/InputManager.h"
#include "app/Math/Math.h"
#include "app/Actors/Player/Player.h"
#include<memory>

//�J�����̒����_�̕␳�l
const GSvector3 ReferencePointOffset{ 0.0f,1.7f,0.0f };
float cameraDistance_{ 15.0f };

CameraTps::CameraTps(IWorld* world, const GSvector3& position, const GSvector3& at) :
	Actor()
{
	//���[���h��ݒ�
	world_ = world;
	//�^�O�̐ݒ�
	tag_ = "CameraTag";
	//���O�̐ݒ�
	name_ = "Camera";
	//�����_�̈ʒu
	transform_.position(position); //0,8,3
	//�����_��ݒ�i�����_�̕����Ɍ�����ς���j
	transform_.lookAt(at); //0,0,-3

	//InputManager::RightAxisReversX(false);
	//InputManager::RightAxisReversY(false);

	startFlag_ = true;

	cameraAngle_.y = 25;

	

}

void CameraTps::update(float delta_time)
{
	//�v���C���[�擾
	//find_actor��Actor�N���X�Ŏ���Ă��Ă��܂��̂�dynamic_pointer_cast��Player�N���X�ɕς���
	std::shared_ptr<Player> player = std::dynamic_pointer_cast<Player>(world_->find_actor("Player"));

	//�����_�擾
	smoothPlayerLookPos_ = player->getCameraLookPoint();
	float smoothPercent = 10.0f;
	//�ʒu�␳�^�C��
	if (smoothPlayerLookPos_.y != prevPlayerLookPos_.y) {
		smoothPlayerLookPos_.y = (smoothPlayerLookPos_.y - prevPlayerLookPos_.y) * (smoothPercent / 100.0f) + prevPlayerLookPos_.y;
	}
	

	if (startFlag_) {
		cameraAngle_.x = player->transform().localEulerAngles().y * (-1) + 270;
		cameraAngle_.y = 25;
		startFlag_ = false;
	}

	//LB�ŃJ�������Z�b�g
	if (InputManager::IsCameraResetState()) {
		cameraAngle_.x = player->transform().localEulerAngles().y * (-1) + 270;
		cameraAngle_.y = 25;
	}

	//����
	cameraAngle_.x += InputManager::CameraRotation().x * InputManager::GetCameraSensitivity() * delta_time;
	if (InputManager::CameraRotation().y == -1 && cameraLevel_ == 0) {
		cameraAngle_.y = 30;
		cameraDistance_ = 20.0f;
		cameraLevel_ = 1;
	}
	if (InputManager::CameraRotation().y == 1 && cameraLevel_ == 1) {
		cameraAngle_.y = 20;
		cameraDistance_ = 12.0f;
		cameraLevel_ = 0;
	}

	//cameraAngle_.y = 25;
	if (cameraAngle_.x < 0) cameraAngle_.x += 360;
	if (cameraAngle_.x > 360) cameraAngle_.x -= 360;
	if (cameraAngle_.y <= -40) cameraAngle_.y = -40;
	if (cameraAngle_.y >= 60) cameraAngle_.y = 60;
	cameraAngleE_ = cameraAngle_ * (math::PI / 180);

	GSvector3 position;

	//�J������y���W����
	position.y = smoothPlayerLookPos_.y + sinf(cameraAngleE_.y) * cameraDistance_;
	float xzPos = cosf(cameraAngleE_.y) * cameraDistance_;

	//�J��������Ɍ������Ƃ��̈ʒu����
	if (position.y < smoothPlayerLookPos_.y) {
		float bairitsu = (sinf(cameraAngleE_.y) * cameraDistance_) / -ReferencePointOffset.y;
		position.y = smoothPlayerLookPos_.y;
		xzPos = cosf(cameraAngleE_.y) * cameraDistance_ / bairitsu;
		if (xzPos >= cameraDistance_) {
			xzPos = cosf(cameraAngleE_.y) * cameraDistance_;
		}
	}

	//�J������xz���W����
	position.x = player->transform().position().x + cosf(cameraAngleE_.x) * xzPos;
	position.z = player->transform().position().z + sinf(cameraAngleE_.x) * xzPos;


	//�����_�̈ʒu�����߂�(�v���C���[�̓����̏����゠����̍��W)
	GSvector3 at = smoothPlayerLookPos_ + ReferencePointOffset;

	// �t�B�[���h�Ƃ̏Փ˔���
	//collideField(0.1f);		//�t�B�[���h�Ƃ̏Փ˔���
	Line line{ at,position };
	GSvector3 intersect;
	if (world_->field()->collide(line, &intersect))
	{
		position = intersect;
	}

	//�X���[�X�_���v�ɂ�銊�炩�ȕ��
	const float SmoothTime{ 12.0f };	//��ԃt���[����
	const float MaxSpeed{ 1.0f };		//�ړ��X�s�[�h�̍ő�l
	position = GSvector3::smoothDamp(
		transform_.position(), position, velocity_,
		SmoothTime, MaxSpeed, delta_time);

	/*at = GSvector3::smoothDamp(
		prevPlayerLookPos_, at, velocity_,
		SmoothTime, MaxSpeed, delta_time);*/


	//���_�̈ʒu��ݒ�
	transform_.position(position);
	//�����_��ݒ�(�����_�̕����Ɍ�����ς���)
	transform_.lookAt(at);

	//�Ō�ɑO�̃t���[���̒����_�Ƃ��ăZ�b�g
	prevPlayerLookPos_ = smoothPlayerLookPos_;
}


void CameraTps::draw() const
{
	//���_�̈ʒu
	GSvector3 eye = transform_.position();
	//�����_�̈ʒu
	GSvector3 at = eye + transform_.forward();
	//�J�����̐ݒ�
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		eye.x, eye.y, eye.z,//���_�̈ʒu
		at.x, at.y, at.z,//�����_�̈ʒu
		0.0f, 1.0f, 0.0f//���_�̏����
	);
}

