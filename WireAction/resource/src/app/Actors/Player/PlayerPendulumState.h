#ifndef PLAYER_PENDULUM_STATE_H_
#define PLAYER_PENDULUM_STATE_H_

#include <memory>

#include "app/StateMachine/StateBase.h"
#include "app/Actors/Player/Player.h"

// �v���C���[�̈ړ������N���X
class PlayerPendulumState : public app::StateBase
{
public:
	// �R���X�g���N�^
	PlayerPendulumState(Player* parent, IWorld* world, app::StateMachine* stateMachine);
	// �f�X�g���N�^
	~PlayerPendulumState() = default;
	// ������
	void init() override;
	// �I��
	void final() override;
	// �X�V
	void update() override;
	void lateUpdate()override;
	// �`��
	void draw() const override;
	void lateDraw()const override;



private:
	void changeAngle();



private:
	Player* parent_;
	IWorld* world_;
	GSvector3 position_;
	GSvector3 my_Input_Direction_{ 0,0,0 };
	GSvector3 velocity_{ 0.0f,0.0f,0.0f }; //���x
	GSvector3 wirePosition_{ 0,0,0 }; //�U��q�̒��S
	GSvector3 pointH_{ 0,0,0 }; //���C���[�̎x�_��Y���W��������l���ɍ��킹�����W
	GSvector3 thetaZeroPosition_{ 0,0,0 };//����0�x�ɂȂ�ʒu
	GSvector3 previousPosition_{ 0,0,0 }; //�U��q����1�t���[���O�̍��W

	//�J�����̒����_(�W�����v�ŕς�邽�ߕϐ��ɂ��Ă���)
	GSvector3 cameraLookPoint_{ 0,0,0 };

	float w_{ 0.0f };//�d��[N]
	const float G = 9.8f; //�d�͉����x
	const float M = 3.5f; //�̏d
	const float MaxAngleSpeed = 5.0f; //�ō���]���x(�U��q�̉�]�̍ō����x)
	const float ShakeLevel = 3.0f; //�U��₷��(�U��q�̑��x�ɔ{����������)

	float wireX_{0.0f};
	float wireY_{0.0f};
	float wireZ_{0.0f};
	float angle_{ 0.0f };
	float phi_{ 0.0f };
	float wireRadius_{ 0.0f };
	float angleSpeed_{ 0.0f };
	float angleSpeedSpeed_{ 0.0f }; //�����x�̉����x
	float angle90_{ 0.0f };
	float sinTheta_{ 0.0f };

	float previousPhi_{ 0.0f };

	bool isWire_{ true }; //���C���[���ڑ�����Ă��邩

	float upDirection_{ 1.0f }; //������Ȃ�1�A�������Ȃ�-1

};


#endif