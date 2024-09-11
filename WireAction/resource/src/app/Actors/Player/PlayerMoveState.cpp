#include "PlayerMoveState.h"
#include "app/Namelist/PlayerStateList.h"
#include "app/Worlds/World.h"
#include "app/time/Time.h"
#include "app/Math/Math.h"
#include "app/Input/InputManager.h"
#include "app/NameList/Assets.h"

// �ړ����x
const float MIN_SPEED{ 0.008f };
const float ACCELERATION{ 0.005f };
const float MAX_SPEED{ 0.2f };
const float ACCELERATION2{ 0.1f };
const float MAX_SPEED2{ 0.25f };

// << �v���C���[�̈ړ� >>

// �R���X�g���N�^
PlayerMoveState::PlayerMoveState(Player* parent, IWorld* world, app::StateMachine* stateMachine) :
	StateBase(stateMachine), parent_(parent), world_(world), isDash_(false)
{
	moveSpeed_ = 0;
}
// ������
void PlayerMoveState::init()
{
	//���x�p��
	velocity_ = parent_->velocity();

	my_Input_Direction_ = parent_->GetInput();
	if (parent_->GetMotionState() == Motion_Rolling) {
		rowSpeedKoroKoro_ = true;
	}
	else {
		if (velocity_.x == 0 && velocity_.z == 0) {
			parent_->ChangeMotionS(Motion_Idle, true); //���[�V�����ύX
		}
		else {
			parent_->ChangeMotionS(Motion_Run, true, 1.0f + moveSpeed_ * 10.0f); //���[�V�����ύX
		}
	}
	

	//�J�������W
	cameraLookPoint_ = parent_->getCameraLookPoint();

	

	//���̃X�e�[�g���n�܂������̑��x�������ŋ��߂Ă���
	//�ʏ�̕��s���������X�s�[�h�̂܂ܕ�����ԂɂȂ������A���̑��x���ێ�����Ƃ�������
	stateStartSpeed = sqrtf((velocity_.x * velocity_.x) + (velocity_.z * velocity_.z));

	//���͂�����Ή����A������Ύ~�܂�
	if (parent_->input_ != GSvector3::zero()) {
		SState_ = SpeedUp;
		
	}
	else {
		SState_ = SpeedDown;
	}

	//�O�̃t���[���̌����Ă�p�x
	previousAngle_ = math::angleFromVector2Deg(my_Input_Direction_.x, my_Input_Direction_.z);

}
// �I��
void PlayerMoveState::final()
{
}
// �X�V
void PlayerMoveState::update()
{
	//�܂����R����
	parent_->gravityFall(1.0f);

	//���x�p������
	velocity_ = parent_->velocity();
	moveSpeed_ = sqrtf((velocity_.x * velocity_.x) + (velocity_.z * velocity_.z));

	//�W�����v�{�^���������ꂽ��X�e�[�g�ύX
	if (InputManager::IsAButtonTrigger() && SState_!=brake) {
		GSvector3 lastPosition = parent_->GetPosition();
		cameraLookPoint_ = lastPosition + velocity_;
		parent_->changeState(PlayerStateList::State_Jump);
		parent_->velocity().y = 0.32f; //0.18
		parent_->setCameraLookPoint(cameraLookPoint_);
		parent_->ChangeMotionS(Motion_JumpIn, false); //���[�V�����ύX
		rowSpeedKoroKoro_ = false;
		parent_->setLastPosition(lastPosition);
		return;
	}

	//�n�ʂ��痎���Ă��X�e�[�g�ύX
	if (!parent_->isGround()) {
		parent_->changeState(PlayerStateList::State_Jump);
		parent_->setCameraLookPoint(cameraLookPoint_);
		rowSpeedKoroKoro_ = false;
		//������O�̈ړ��ʂ��v�Z
		GSvector3 lastPositionDirection = parent_->GetPosition() - previousPosition_;
		//parent_->setLastPosition(previousPosition_-(lastPositionDirection*10));
		parent_->setLastPosition(previousPosition_);
		return;
	}

	//X�{�^�����������烏�C���[�����Ɉڍs
	if (InputManager::IsXButtonTrigger()) {
		if (parent_->getThrowing())return;

		changeAngle(600.0f);
		parent_->setBeamDirection(my_Input_Direction_);
		parent_->changeState(PlayerStateList::State_ThrowWire);
		parent_->ChangeMotionS(Motion_JumpNow, false); //���[�V�����ύX
		rowSpeedKoroKoro_ = false;

		cameraLookPoint_.x = parent_->GetPosition().x + velocity_.x;
		cameraLookPoint_.z = parent_->GetPosition().z + velocity_.z;

		//�����_�ݒ�
		parent_->setCameraLookPoint(cameraLookPoint_);
		return;
	}

	//���Ⴊ��
	if (InputManager::IsCrouchState()) {
		parent_->setCameraLookPoint(cameraLookPoint_);
		parent_->changeState(PlayerStateList::State_Crouch);
		parent_->ChangeMotionS(Motion_Crouch, true); //���[�V�����ύX
		rowSpeedKoroKoro_ = false;
		return;
	}

	//�ᑬ���낱��I��
	if (parent_->IsMotionEnd() && rowSpeedKoroKoro_) {
		rowSpeedKoroKoro_ = false;
		parent_->ChangeMotionS(Motion_Run, true, 1.0f + moveSpeed_ * 10.0f); //���[�V�����ύX
		parent_->resetStateTimer();
	}

	//���W�擾
	position_ = parent_->GetPosition();
	//�����Ă�����擾
	my_Input_Direction_ = parent_->GetInput();

	float nowAngle = 0;

	//�ړ����x�̌v�Z
	//���x�ɉ����Ă���ɏ�ԕ�������
	switch (SState_) {
	case Stop:
		moveSpeed_ = 0;
		
		//�X�e�B�b�N���|���ꂽ�u��
		if (parent_->input_ != GSvector3::zero()) {
			moveSpeed_ = MIN_SPEED;
			SState_ = SpeedUp;
			parent_->ChangeMotionS(Motion_Run, true, 1.0f + moveSpeed_ * 10.0f); //���[�V�����ύX
			my_Input_Direction_ = parent_->GetInput();
		}

		//�����Ă�����ɑ��x�������ĉ����x�ɂ���
		velocity_ = my_Input_Direction_ * moveSpeed_;

		break;

	case SpeedUp:
		// �ړ����Ă��邩
		if (velocity_.length() != 0.0f) {

			changeAngle();
		}

		//���x����
		if (stateStartSpeed > MAX_SPEED) {
			moveSpeed_ = stateStartSpeed;
		}
		else if (moveSpeed_ + ACCELERATION < MAX_SPEED) {
			moveSpeed_ += ACCELERATION;
		}
		else {
			moveSpeed_ = MAX_SPEED;
		}

		if (moveSpeed_ > MAX_SPEED) {
			//�]���蒆�͌���
			parent_->ChangeMotionS(Motion_Rolling, true, moveSpeed_*10.0f,0.5f,8); //���[�V�����ύX
			moveSpeed_ -= ACCELERATION / 2;
			stateStartSpeed = moveSpeed_;
			

		}else if (!rowSpeedKoroKoro_) {
			//�����A�j�����ς���ĂȂ�������ύX
			parent_->ChangeMotionS(Motion_Run, true, 1.0f + moveSpeed_ * 10.0f); //���[�V�����ύX
		}

		//�X�e�B�b�N�������ꂽ�u��
		if (parent_->input2_ != GSvector3::zero() && parent_->input_ == GSvector3::zero()) {
			//�����J�n
			SState_ = SpeedDown;
			parent_->ChangeMotionS(Motion_Walk, true); //���[�V�����ύX
		}
		else {
			if (parent_->input2_ != parent_->input_) {
				my_Input_Direction_ = parent_->GetInput();
			}
		}

		nowAngle = math::angleFromVector2Deg(my_Input_Direction_.x, my_Input_Direction_.z);
		if (nowAngle < 0)nowAngle += 360;
		//�}�ɋt�����ɃX�e�B�b�N
		if (abs(180-abs(previousAngle_ - nowAngle)) <30
			&& nowAngle!=0
			&& moveSpeed_ > MIN_SPEED/2) {
			SState_ = brake;
			//�A�j����2���
			parent_->ChangeMotionS(Motion_Brake, true); //Brake
			rowSpeedKoroKoro_ = false;
			parent_->resetStateTimer();
		}

		
		//�����Ă�����ɑ��x�������ĉ����x�ɂ���
		velocity_ = my_Input_Direction_ * moveSpeed_;

		break;

	case SpeedDown:

		if (moveSpeed_ > MAX_SPEED*1.2f) {
			//�]���蒆�͌���
			parent_->ChangeMotionS(Motion_Rolling, true, moveSpeed_ * 10.0f, 0.5f, 8); //���[�V�����ύX
			//parent_->ChangeMotionS(Motion_Run, true, 1.0f + moveSpeed_); //���[�V�����ύX


		}
		else if (parent_->GetMotionState() != Motion_Run && !rowSpeedKoroKoro_) {
			//�����A�j�����ς���ĂȂ�������ύX
			parent_->ChangeMotionS(Motion_Run, true, 1.0f + moveSpeed_ * 10.0f); //���[�V�����ύX
		}



		if (moveSpeed_ - ACCELERATION*2 > 0) {
			moveSpeed_ -= ACCELERATION * 2;
			//���x����
			if (stateStartSpeed > moveSpeed_) {
				stateStartSpeed = moveSpeed_;
			}
		}
		else {
			moveSpeed_ = 0;
			SState_ = Stop;
			parent_->ChangeMotionS(Motion_Idle, true); //���[�V�����ύX
		}

		//�������������瑬�x��������
		if (velocity_.x * my_Input_Direction_.x <= 0 && velocity_.z * my_Input_Direction_.z <= 0) {
			moveSpeed_ *= -1;
		}

		//�X�e�B�b�N���|���ꂽ�u��
		if (parent_->input2_ == GSvector3::zero() && parent_->input_ != GSvector3::zero()) {
			my_Input_Direction_ = parent_->GetInput();
			nowAngle = math::angleFromVector2Deg(my_Input_Direction_.x, my_Input_Direction_.z);
			if (nowAngle < 0)nowAngle += 360;
			//�}�ɋt�����ɃX�e�B�b�N
			if (abs(180 - abs(previousAngle_ - nowAngle)) < 30
				&& nowAngle != 0
				) {
				SState_ = brake;
				parent_->ChangeMotionS(Motion_Brake, true); //Brake
				parent_->resetStateTimer();
				rowSpeedKoroKoro_ = false;
			}
			else if(!rowSpeedKoroKoro_) {
				moveSpeed_ = MIN_SPEED;
				SState_ = SpeedUp;
				parent_->ChangeMotionS(Motion_Run, true, 1.0f + moveSpeed_ * 10.0f); //���[�V�����ύX
			}
		}

		//�����Ă�����ɑ��x�������ĉ����x�ɂ���
		velocity_ = my_Input_Direction_ * moveSpeed_;

		break;

	case brake:
		if (moveSpeed_ - ACCELERATION*2 > 0) {
			moveSpeed_ -= ACCELERATION*2;
			//���x����
			if (stateStartSpeed > moveSpeed_) {
				stateStartSpeed = moveSpeed_;
			}
		}
		else {
			moveSpeed_ = 0;
			
			
			if (parent_->input_ != GSvector3::zero()) {
				SState_ = SpeedUp;
				parent_->ChangeMotionS(Motion_Run, true, 1.0f + moveSpeed_ * 10.0f); //���[�V�����ύX
			}
			else {
				SState_ = Stop;
				parent_->ChangeMotionS(Motion_Idle, true); //���[�V�����ύX
			}
			parent_->resetStateTimer();
		}

		//�����Ă�����ɑ��x�������ĉ����x�ɂ���
		velocity_ = my_Input_Direction_ * -moveSpeed_;
		
		//�W�����v�{�^���������ꂽ��X�e�[�g�ύX
		if (InputManager::IsAButtonTrigger()) {
			cameraLookPoint_ = parent_->GetPosition() + velocity_;
			parent_->changeState(PlayerStateList::State_Jump);
			parent_->velocity(GSvector3(my_Input_Direction_.x * 0.1f, 0.40f, my_Input_Direction_.z * 0.1f));
			parent_->setCameraLookPoint(cameraLookPoint_);
			parent_->ChangeMotionS(Motion_BackRolling, false); //���[�V�����ύX
			rowSpeedKoroKoro_ = false;
			return;
		}

		break;

	}

	
	

	//���낢��ς����������]������e�ɕԂ�
	parent_->SetInputDirection(my_Input_Direction_);

	// ���[�V�����̕ύX
	// �ړ��ʂ�xy���������X�V
	parent_->velocity().x = velocity_.x;
	parent_->velocity().z = velocity_.z;

	cameraLookPoint_ = parent_->GetPosition() + velocity_;

	//�ړ�
	//parent_->SetPosition(parent_->GetPosition() + velocity_);

	
	//�����_�ݒ�
	parent_->setCameraLookPoint(cameraLookPoint_);

	//����
	//parent_->setDebugMoveSpeed(nowAngle);
	parent_->SetDebugFloat(previousAngle_, nowAngle,moveSpeed_);

	//�Ō�ɑO�̃t���[���̒l���Ƃ��Ă���
	if (nowAngle != 0.0f) {
		previousAngle_ = nowAngle;
	}
	if (parent_->isGround()) {
		previousPosition_ = position_;
	}
}
void PlayerMoveState::lateUpdate()
{
	
}
// �`��
void PlayerMoveState::draw() const
{
}
void PlayerMoveState::lateDraw() const
{
}

void PlayerMoveState::changeAngle(float time) {
	//�����̕��
	GSquaternion rotation =
		GSquaternion::rotateTowards(
			parent_->transform().rotation(),
			GSquaternion::lookRotation(my_Input_Direction_), time);
	parent_->transform().rotation(rotation);
}
