#include "PlayerPendulumState.h"
#include "app/Namelist/PlayerStateList.h"
#include "app/Worlds/World.h"
#include "app/time/Time.h"
#include "app/Math/Math.h"
#include "app/Input/InputManager.h"
#include "app/Namelist/PlayerStateList.h"
#include "app/NameList/Assets.h"



// �R���X�g���N�^
PlayerPendulumState::PlayerPendulumState(Player* parent, IWorld* world, app::StateMachine* stateMachine) :
	StateBase(stateMachine), parent_(parent), world_(world)
{

}
// ������
void PlayerPendulumState::init()
{
    wireX_= 0.0f;
    wireY_= 0.0f;
    wireZ_= 0.0f;
    angle_= 0.0f;
    phi_  = 0.0f;
    wireRadius_      =0.0f;
    angleSpeed_      =0.0f;
    angleSpeedSpeed_ =0.0f; //�����x�̉����x
    angle90_         =0.0f;
    sinTheta_        =0.0f;
    previousPhi_     =0.0f;
    position_= parent_->transform().position();
    pendulumInvert_ = 1;

	//���C���[�ɕK�v�ȏ����l���v�Z����
	wirePosition_ = parent_->getCenterPendulum();
	GSvector3 playerPosition = parent_->transform().position();
	//���C���[�̒���(wireRadius�͐U��q�̔��a�ɂȂ�)
	wireX_ = wirePosition_.x - playerPosition.x;
	wireY_ = wirePosition_.y - playerPosition.y;
	wireZ_ = wirePosition_.z - playerPosition.z;
	wireRadius_ = sqrtf(wireX_ * wireX_ + (wireY_ * wireY_) + wireZ_ * wireZ_);

	//���C���[�̎x�_��Y���W��������l���ɍ��킹���ʒu
	pointH_ = GSvector3{ wirePosition_.x,playerPosition.y,wirePosition_.z };

    //PointH���炫��҂����ɐL�΂����x�N�g��
    GSvector3 phiA = GSvector3{ playerPosition.x - pointH_.x, 0, playerPosition.z - pointH_.z };

    //PointH���獶�ɂ܂������L�΂����x�N�g��
    GSvector3 phiB = GSvector3{ -abs(playerPosition.x - pointH_.x), 0, 0 };

    //�ォ�猩���p�x��
    //�^���ʂ̂Ƃ�����������Ɠ��ʂȏ���
    if (phiB == GSvector3::zero())
    {
        if (wireZ_ > 0)
        {
            phi_ = math::PI / 2;
        }
        else
        {
            phi_ = math::PI * 3 / 2;

        }
    }
    else
    {
        if (wireZ_ > 0)
        {
            phi_ = acos((phiA.x * phiB.x + phiA.y * phiB.y + phiA.z * phiB.z) /
                (sqrt(phiA.x * phiA.x + phiA.y * phiA.y + phiA.z * phiA.z) *
                    sqrt(phiB.x * phiB.x + phiB.y * phiB.y + phiB.z * phiB.z)));
        }
        else
        {
            phi_ = math::PI * 2 - acos((phiA.x * phiB.x + phiA.y * phiB.y + phiA.z * phiB.z) /
                (sqrt(phiA.x * phiA.x + phiA.y * phiA.y + phiA.z * phiA.z) *
                    sqrt(phiB.x * phiB.x + phiB.y * phiB.y + phiB.z * phiB.z)));
        }
    }

    //����0�x�ɂȂ�ʒu
    thetaZeroPosition_ = GSvector3{ wirePosition_.x + wireRadius_ * cos(phi_),
        wirePosition_.y, wirePosition_.z + wireRadius_ * sin(phi_) };

    //wirePosition���炻���܂ł̃x�N�g��
    GSvector3 thetaA = thetaZeroPosition_ - wirePosition_;

    //�x�_���炫��҂����܂ł̃x�N�g��
    GSvector3 thetaB = playerPosition - wirePosition_;




    //�����̊p�x(��)����
    if (wireY_ < 0)
    {
        angle_ = acos((thetaA.x * thetaB.x + thetaA.y * thetaB.y + thetaA.z * thetaB.z) /
            (sqrt(thetaA.x * thetaA.x + thetaA.y * thetaA.y + thetaA.z * thetaA.z) *
                sqrt(thetaB.x * thetaB.x + thetaB.y * thetaB.y + thetaB.z * thetaB.z)));
    }
    else
    {
        angle_ = math::PI * 2 - acos((thetaA.x * thetaB.x + thetaA.y * thetaB.y + thetaA.z * thetaB.z) /
            (sqrt(thetaA.x * thetaA.x + thetaA.y * thetaA.y + thetaA.z * thetaA.z) *
                sqrt(thetaB.x * thetaB.x + thetaB.y * thetaB.y + thetaB.z * thetaB.z)));
    }
    
    velocity_ = GSvector3::zero();
    mSAfP_Near = phi_;

}
// �I��
void PlayerPendulumState::final()
{
}
// �X�V
void PlayerPendulumState::update()
{
    GSvector3 playerPosition = parent_->transform().position();

    //���C���[����
    if (InputManager::IsAButtonTrigger()) {
        cameraLookPoint_ = parent_->GetPosition() + velocity_;
        parent_->changeState(PlayerStateList::State_Jump);
        parent_->setCameraLookPoint(cameraLookPoint_);
        parent_->ChangeMotionS(Motion_Jump, false); //���[�V�����ύX

        

        // ���[�V�����̕ύX
        // �ړ��ʂ�xy���������X�V
        parent_->velocity(velocity_);
        return;
    }

    //���n������I��
    if (parent_->isGround()) {
        /*parent_->changeState(PlayerStateList::State_Walk);
        parent_->ChangeMotionS(Motion_Idle, false); //���[�V�����ύX
        parent_->velocity(GSvector3::zero());
        return;*/
        wireY_= wirePosition_.y - playerPosition.y;
        wireRadius_ = sqrtf(wireX_ * wireX_ + (wireY_) * (wireY_));

    }


	w_ = G * M; //�d��[N]

    wireX_ = wireRadius_ * cos(angle_); //�΂߂̒���(�ォ�猩���Ƃ�)
    if ((int)round(angle_ * (180 / math::PI)) % 180 == 0)
    {
        wireY_ = 0;
    }
    else
    {
        wireY_ = wireRadius_ * sin(angle_);
    }

    //���C���[���̍��W����
    //X��Z���W��phi(y����]�̊p�x)���狁�߂�
    position_.x = wirePosition_.x + wireX_ * cos(phi_);

    //���W��U��q�̍��W��
    position_.y = wireY_ + wirePosition_.y;

    position_.z = wirePosition_.z + wireX_ * sin(phi_);

    angleSpeedSpeed_ = previousPosition_.y - position_.y;

    velocity_.x = (position_.x - previousPosition_.x) * 1.0f; //�}�W�b�N�i���o�[�ɂ��Ă邯�ǁA
    velocity_.z = (position_.z - previousPosition_.z) * 1.0f; //�}�����������Ƃ��ɕϐ��ɂ���΂���

    //����
    if (angleSpeedSpeed_ < 0) //�����
    {
        upDirection_ = 1;
    }
    else
    {
        upDirection_ = -1;
    }

    //vy�͐U��q�̉��S�͂��玝���Ă���
    //���S�� f = mv^2/r = mr��^2
    //�����̎���ϊ�����Ώc�����̑��xv�����܂�
    //�ւ͊p���x(angleSpeed)�Ar�͔��a(wireRadius)
    velocity_.y = sqrt((wireRadius_ * wireRadius_) * (angleSpeed_ * angleSpeed_)) * upDirection_* 0.016f;

    //���̈ʒu���L�^
    previousPosition_ = position_;

    //���Ŏ���Ă��p�x(��)��^����0�x�ɂȂ�悤�ɒ���
    angle90_ = -angle_ + math::PI / 180 * -90;
    sinTheta_ = sin(angle90_);

	//�ǂ�����������
	//�U��q�̉����x�ω�
	angleSpeed_ += (w_ / wireRadius_ * sinTheta_)*0.016f; //1/60
    //�ō����x�Ŏ~�߂Ƃ�
    if (angleSpeed_ > MaxAngleSpeed)
    {
        angleSpeed_ = MaxAngleSpeed;
    }
    else if (angleSpeed_ < -MaxAngleSpeed)
    {
        angleSpeed_ = -MaxAngleSpeed;
    }

    //�U��q�̊p�x��ω�
    angle_ += angleSpeed_ * 0.016f;
    if (angle_ < 0)
    {
        angle_ += 2 * math::PI;
    }
    else if (angle_ > 2 * math::PI)
    {
        angle_ -= 2 * math::PI;
    }

    //���݂̌�������U�ۑ����Ă���
    previousPhi_ = phi_;

    //���āA�������烳(����)�ω�
    if (parent_->input_ != GSvector3::zero()) {

        //�㑤�ɍs���Ă��甽�]
        if (angle_ < math::PI)
        {
            pendulumInvert_ = -1;
        }
        else
        {
            pendulumInvert_ = 1;
        }

        float shakeX = parent_->GetInput().x;
        float shakeZ = parent_->GetInput().z;
        if (shakeX < 0) {
            sthickAngle_ = atanf(shakeZ / shakeX) + math::PI;
        }
        else if (shakeZ < 0) {
            sthickAngle_ = atanf(shakeZ / shakeX) + math::PI * 2;
        }
        else {
            sthickAngle_ = atanf(shakeZ / shakeX);
        }

        if (sthickAngle_ >= math::PI) {
            sthickAngleForPhi_ = sthickAngle_ - math::PI;
        }
        else {
            sthickAngleForPhi_ = sthickAngle_ + math::PI;
        }

        //�}�E�X��U�����p�x�Ƃ�������180�x�񂵂��p�x�A���̊p�x�ɋ߂����ɍ��킹�Ȃ��Ɣ��]����
        //�܂�����(�}�E�X�U����������)
        if (sthickAngleForPhi_ < math::PI)
        {
            //��O�̕����߂�
            if (abs(phi_ - sthickAngleForPhi_) < abs(phi_ - (sthickAngleForPhi_ + math::PI)))
            {
                mSAfP_Near = sthickAngleForPhi_;
                angleSpeed_ -= ShakeLevel * 1/60*pendulumInvert_;
                //print("a");

            }
            else if (abs(phi_ - sthickAngleForPhi_) >= abs(phi_ - (sthickAngleForPhi_ + math::PI)))
            {
                //���Ɍ������Đi��ł�
                mSAfP_Near = sthickAngleForPhi_ + math::PI;
                angleSpeed_ += ShakeLevel * 1/60 * pendulumInvert_;

                //print("b");
            }
        }
        else//�}�E�X�U�����������㑤
        {
            //���̕����߂�
            if (abs(phi_ - sthickAngleForPhi_) < abs(phi_ - (sthickAngleForPhi_ - math::PI)))
            {
                //���Ɍ������Đi��ł�
                mSAfP_Near = sthickAngleForPhi_;
                angleSpeed_ -= ShakeLevel * 1/60 * pendulumInvert_;

            }
            else if (abs(phi_ - sthickAngleForPhi_) >= abs(phi_ - (sthickAngleForPhi_ - math::PI)))
            {
                mSAfP_Near = sthickAngleForPhi_ - math::PI;
                angleSpeed_ += ShakeLevel * 1/60 * pendulumInvert_;
            }
        }
        
    }


    //���܂��傤��
    phi_ += (mSAfP_Near - previousPhi_) * 1 / 60;


    //���ɉ������ɗ����Ă��㏸���Ȃ�
    if (angleSpeedSpeed_ > 0)
    {
        if (velocity_.y > 0)
        {
            velocity_.y = 0;
        }
    }


    //���ڕς����Ⴄ�̂ǂ����Ǝv����ȁ[
    //������V�������W����O�̍��W���������l�𑬓x�ɂ���
    parent_->velocity(position_-playerPosition);

    // ���[�V�����̕ύX
    // �ړ��ʂ�xy���������X�V
    //parent_->velocity(velocity_);

    cameraLookPoint_ = wirePosition_;
    cameraLookPoint_.y = wirePosition_.y - wireRadius_ * 0.5f;
    //�����_�ݒ�
    parent_->setCameraLookPoint(cameraLookPoint_);


    //�U��q�̌�������l���������Ă�����ɂ���(��������Ȃ��Ɛ������������Ȃ�)
    //�U��q�̃x�N�g���𐳋K������
    float norm = sqrtf(velocity_.x * velocity_.x + velocity_.z * velocity_.z);
    float mag = 1.0f / norm;
    my_Input_Direction_ = (velocity_ * mag);
    my_Input_Direction_.y = 0;
    parent_->SetInputDirection(my_Input_Direction_);


	return;




}
void PlayerPendulumState::lateUpdate()
{

}
// �`��
void PlayerPendulumState::draw() const
{
    GSvector2 startPoint = GSvector2::zero();
    GSvector2 endPoint = GSvector2::zero();
    GSvector3 handPosition = position_;
    handPosition.y += 1.5f; //��̈ʒu�̃I�t�Z�b�g
    GSvector3 centerPosition = wirePosition_;

    //���ꂼ��̍��W����ʍ��W�ɕϊ�
    gsCalculateScreen(&startPoint, &handPosition);
    gsCalculateScreen(&endPoint, &centerPosition);

    //��ʍ��W���}�C�i�X�ɂȂ�Ȃ��悤�ɂ���
    if (startPoint.x > 10000) {
        startPoint.x -= 20000.0f;
    }
    /*if (startPoint.y< 0) {
        startPoint.y += 1080.0f;
    }*/


    gsDrawText("s%f\n", startPoint.x);
    //gsDrawText("\ne%f", endPoint.x);

    //�摜�̒��S����
    static const GSvector2 center{16.0f,0.0f};
    //�F
    GScolor color = { 1.0f, 1.0f, 1.0f, 1.0f };

    float nagasa = sqrtf((endPoint.y - startPoint.y) * (endPoint.y - startPoint.y) + (endPoint.x - startPoint.x) * (endPoint.x - startPoint.x));
    GSvector2 end_scale{ 0.5f, nagasa/32.0f };

    GSvector2 position = GSvector2(endPoint);

    //�摜�̐؂蔲��
    static const GSrect Rect{ 0.0f, 0.0f, 32.0f,32.0f };

    //�p�x����
    float drawAngle = atan2f(endPoint.y - startPoint.y, endPoint.x - startPoint.x)*(180/math::PI)+90;

    gsDrawSprite2D(Texture_Wire, &position, &Rect, &center,  &color, &end_scale, drawAngle);
    //gsDrawSprite3D(Texture_Wire, &position_, &body, NULL, &color, &scale, 0.0f);
    
}
void PlayerPendulumState::lateDraw() const
{
}

void PlayerPendulumState::changeAngle() {
	//�����̕��
	GSquaternion rotation =
		GSquaternion::rotateTowards(
			parent_->transform().rotation(),
			GSquaternion::lookRotation(my_Input_Direction_), 12.0f);
	parent_->transform().rotation(rotation);
}


