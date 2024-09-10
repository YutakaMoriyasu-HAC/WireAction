#include "PlayerJumpState.h"
#include "app/Namelist/PlayerStateList.h"
#include "app/Worlds/World.h"
#include "app/time/Time.h"
#include "app/Math/Math.h"
#include "app/Input/InputManager.h"


// �ړ����x
const float MIN_SPEED{ 0.008f };
const float ACCELERATION{ 0.001f };
const float MAX_SPEED{ 0.05f };

// << �v���C���[�̈ړ� >>

// �R���X�g���N�^
PlayerJumpState::PlayerJumpState(Player* parent, IWorld* world, app::StateMachine* stateMachine) :
	StateBase(stateMachine), parent_(parent), world_(world)
{
}
// ������
void PlayerJumpState::init()
{
	my_Input_Direction_ = parent_->GetInput();

	//�J�������W
	cameraLookPoint_ = parent_->getCameraLookPoint();

	//���x�p��
	velocity_ = parent_->velocity();

	//���̃X�e�[�g���n�܂������̑��x�������ŋ��߂Ă���
	//�ʏ�̕��s���������X�s�[�h�̂܂ܕ�����ԂɂȂ������A���̑��x���ێ�����Ƃ�������
	stateStartSpeed = sqrtf((velocity_.x * velocity_.x) + (velocity_.z * velocity_.z));
	stateStartVec = velocity_;

	//�W�����v�񐔃��Z�b�g
	jumpNum_ = 1;

	//�{�^���������Ă�����
	buttonReleaseFlag_ = false;
	isWallKick_ = false;


}
// �I��
void PlayerJumpState::final()
{
}
// �X�V
void PlayerJumpState::update()
{
	//�܂����R����
	parent_->gravityFall(1.0f);

	//���x�p������
	velocity_ = parent_->velocity();
	moveSpeed_ = sqrtf((velocity_.x * velocity_.x) + (velocity_.z * velocity_.z));

	//���[�v
	if (parent_->IsMotionEnd() &&
		(parent_->GetMotionState() == Motion_JumpIn || parent_->GetMotionState() == Motion_BackRolling || parent_->GetMotionState() == Motion_JumpSpin)) {
		parent_->ChangeMotionS(Motion_JumpNow, true, 0.1f); //���[�V�����ύX
	}

	//�G�𓥂�Â����Ƃ�
	if (parent_->isTrampled(0)) {
		
		cameraLookPoint_.y = (cameraLookPoint_.y + parent_->GetPosition().y) / 2;
		if (InputManager::IsAButtonState()) {
			parent_->velocity().y = 0.35f;
		}
		else {
			parent_->velocity().y = 0.23f;
		}
		return;
	}


	//�ǃL�b�N
	if (parent_->canWallKick() && InputManager::IsAButtonTrigger()) {

		buttonReleaseFlag_ = false;
		my_Input_Direction_ *= -1;
		velocity_ = my_Input_Direction_ * MAX_SPEED*4;
		parent_->velocity(velocity_);
		stateStartSpeed = sqrtf((velocity_.x * velocity_.x) + (velocity_.z * velocity_.z));
		
		
		parent_->velocity().y = 0.32f;
		isWallKick_ = true; //�㏸���Ȃ��Ȃ�����false�ɂ���
		cameraLookPoint_.y = cameraLookPoint_.y + parent_->GetPosition().y;

		changeAngle(600.0f);
		//���낢��ς����������]������e�ɕԂ�
		parent_->SetInputDirection(my_Input_Direction_);
		return;
	}



	//�󒆃W�����v
	if (InputManager::IsAButtonTrigger() && jumpNum_ >= 1) {
		buttonReleaseFlag_ = false;
		//�X�e�B�b�N�|���Ă鎞�A�傫���ړ�
		if (parent_->input_ != GSvector3::zero()) {
			my_Input_Direction_ = parent_->GetInput();
			//�����Ă�����ɑ��x�������ĉ����x�ɂ���
			if (moveSpeed_ < MAX_SPEED) {
				
				velocity_ = my_Input_Direction_ * MAX_SPEED;
				parent_->velocity(velocity_);
				//�ʏ�̕��s���������X�s�[�h�̂܂ܕ�����ԂɂȂ������A���̑��x���ێ�����Ƃ�������
				stateStartSpeed = sqrtf((velocity_.x * velocity_.x) + (velocity_.z * velocity_.z));
			}
		}
		
		parent_->velocity().y = 0.23f;
		cameraLookPoint_.y = (cameraLookPoint_.y+parent_->GetPosition().y)/2;
		jumpNum_ -= 1;
		
		parent_->ChangeMotionS(Motion_JumpSpin, false,1.0f,0.5f,2.0f); //���[�V�����ύX
		parent_->resetStateTimer();
		
		
		
		changeAngle(600.0f);
		//���낢��ς����������]������e�ɕԂ�
		parent_->SetInputDirection(my_Input_Direction_);
		return;
	}

	//�{�f�B�A�^�b�N
	//�ʂ̃X�e�[�g�ɂ�����������
	if (InputManager::IsCameraResetTrigger()) {
		parent_->ChangeMotionS(Motion_BodyAttack, true, 1.0f, 0.5f, 0.0f); //���[�V�����ύX
		parent_->resetStateTimer();
		my_Input_Direction_ = parent_->GetInput();
		velocity_ = my_Input_Direction_ * 0.2f;
		velocity_.y = 0.18f;
		parent_->velocity(velocity_);
		changeAngle(600.0f);
		//���낢��ς����������]������e�ɕԂ�
		parent_->SetInputDirection(my_Input_Direction_);
		parent_->changeState(PlayerStateList::State_BodyAttack);
		return;
	}
	

	//X�{�^�����������烏�C���[�����Ɉڍs
	if (InputManager::IsXButtonTrigger()) {
		if (parent_->getThrowing())return;

		changeAngle(600.0f);
		parent_->setBeamDirection(my_Input_Direction_);
		parent_->changeState(PlayerStateList::State_ThrowWire);
		parent_->ChangeMotionS(Motion_JumpNow, true); //���[�V�����ύX
		parent_->velocity().y = 0.15f;
		

		cameraLookPoint_.x = parent_->GetPosition().x + velocity_.x;
		cameraLookPoint_.z = parent_->GetPosition().z + velocity_.z;

		//�����_�ݒ�
		parent_->setCameraLookPoint(cameraLookPoint_);
		return;
	}

	//�{�^���𗣂����u��
	if (!InputManager::IsAButtonState() && !buttonReleaseFlag_) {
		buttonReleaseFlag_ = true;
	}
	
	
	

	

	//�㏸�����{�^�������Ă����猸��
	//������������������
	if (buttonReleaseFlag_ && velocity_.y > 0) {
		velocity_.y -= ACCELERATION*10;
		if (velocity_.y < 0) {
			velocity_.y = 0;
		}
	}
	//�������Ȃ�ǃL�b�N�I���
	if (velocity_.y <= 0 && isWallKick_) {
		isWallKick_ = false;
	}

	

	//���W�擾
	position_ = parent_->GetPosition();

	//�����Ă����
	my_Input_Direction_ = parent_->GetInput();

	

	//�X�e�B�b�N���͂����������A���݂̃x�N�g������X�e�B�b�N���͕����܂ł̃x�N�g�������߂�
	if (parent_->input_ != GSvector3::zero() && !isWallKick_) {

		

		//my_Input_Direction_�̊p�x
		float inputAngle = atanf(my_Input_Direction_.z / my_Input_Direction_.x)*(180/math::PI);
		if (inputAngle > 360)inputAngle -= 360;
		if (inputAngle <0)inputAngle += 360;

		//�x�N�g���p�x
		float velocityAngle = atanf(velocity_.z / velocity_.x) * (180 / math::PI);
		if (velocityAngle > 360)velocityAngle -= 360;
		if (velocityAngle < 0)velocityAngle += 360;



		//�X�e�B�b�N�p�x�����̊p�x���60�x�ȓ��������瑬�x�p��
		if ((ABS(velocityAngle - inputAngle) < 60 || ABS(velocityAngle - inputAngle) >300) && stateStartSpeed!=0) {
			velocityToInputVector_ = (my_Input_Direction_ * stateStartSpeed) - velocity_;
		}
		else {
			velocityToInputVector_ = (my_Input_Direction_ * MAX_SPEED) - velocity_;
		}

		parent_->setDebugFloat(0, velocityAngle);
		parent_->setDebugFloat(1, inputAngle);
		parent_->setDebugFloat(2, ABS(velocityAngle - inputAngle));


		velocity_.x += velocityToInputVector_.x / 15.0f;
		velocity_.z += velocityToInputVector_.z / 15.0f;
	}
	
	


	

	// ���[�V�����̕ύX
	// �ړ��ʂ�xy���������X�V
	parent_->velocity(velocity_);
	//parent_->velocity().z = velocity_.z;

	cameraLookPoint_.x = parent_->GetPosition().x + velocity_.x;
	cameraLookPoint_.z = parent_->GetPosition().z + velocity_.z;


	//�v���C���[���W�����v�O�̍��W�������ɗ����Ă��璍���_��������
	if (parent_->GetPosition().y < cameraLookPoint_.y) {
		cameraLookPoint_.y = parent_->GetPosition().y;
	}

	//�����_�ݒ�
	parent_->setCameraLookPoint(cameraLookPoint_);

	
	//���n������I��
	if (parent_->isGround()) {
		parent_->changeState(PlayerStateList::State_Walk);
		parent_->ChangeMotionS(Motion_Idle, true); //���[�V�����ύX
		//���낢��ς����������]������e�ɕԂ�
		parent_->SetInputDirection(my_Input_Direction_);
		return;
	}


}
void PlayerJumpState::lateUpdate()
{
}
// �`��
void PlayerJumpState::draw() const
{
}
void PlayerJumpState::lateDraw() const
{
}

void PlayerJumpState::changeAngle(float speed) {
	//�����̕��
	GSquaternion rotation =
		GSquaternion::rotateTowards(
			parent_->transform().rotation(),
			GSquaternion::lookRotation(my_Input_Direction_), speed);
	parent_->transform().rotation(rotation);
}
