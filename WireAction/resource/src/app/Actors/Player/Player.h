#ifndef PLAYER_H_
#define PLAYER_H_

#include "app/Actors/Actor/Actor.h"
#include "app/AnimatedMesh/AnimatedMesh.h"
#include "app/StateMachine/StateMachine.h"

//�v���C���[�N���X
class Player : public Actor {


public:
	//�R���X�g���N�^
	Player(IWorld* world = nullptr, const GSvector3& position = GSvector3{ 0.0f,0.0f,0.0f });
	//�X�V
	virtual void update(float delta_time) override;
	virtual void lateUpdate(float delta_time)override;
	//�`��
	virtual void draw() const override;
	//�Փ˃��A�N�V����
	virtual void react(Actor& other) override;
	// ���[�V�������I�����Ă��邩
	const bool IsMotionEnd() const;
	// ���[�V������ύX
	void ChangeMotionS(GSuint motion, bool loopFlag = true, float speed = 1, float lerp = 0.5f, float endTime = 0.0f, float startTime = 0.0f);
	// ���[�V�����̏I�����Ԃ��擾
	const float GetMotionEndTime() const;
	//�v���C���[�̍��W�𑼂̃N���X�Ɏ󂯓n���Ƃ��̊֐�
	const GSvector3 GetPosition() const;
	//���̃N���X����󂯎�������W��ϐ��ɂ���֐�
	void SetPosition(GSvector3 pos);

	//���݂̃��[�V�����ԍ���Ԃ�
	const int GetMotionState() const;

	//�Ō�Ɍ����Ă���������Ԃ�
	const GSvector3 GetInputDirection() const;

	//�Ō�Ɍ����Ă����������Z�b�g
	void SetInputDirection(GSvector3 direction);

	//input_��Ԃ�
	const GSvector3 GetInput()const;
	//�X�e�[�g�ύX
	void changeState(const int state);

	//�n�ʂɂ��Ă�H
	bool isGround();

	//�����_�擾
	GSvector3 getCameraLookPoint();

	//�����_�Z�b�g
	void setCameraLookPoint(GSvector3 point);

	void setDebugFloat(int num, float value);

	//�v���C���[�N���X���珢��
	void spone(int actorListNum);

	//�r�[�����o������������
	void setBeamDirection(GSvector3 direction);

	//���R����
	void gravityFall(float delta_time);

	//�Z�b�g�ӂ��ƃI�t�Z�b�g�Z�b�g
	void setFootOffset(float footOffset);
private:


	//�_���[�W��
	void damage(float delta_time);

	//�t�B�[���h�Ƃ̏Փˏ���
	void collide_field();
	//�A�N�^�[�Ƃ̏Փˏ���
	void collide_actor(Actor& other);

	// �X�e�[�g�̏�����
	void InitState();


	//���͏��X�V
	void ControllerUpdate();

	// �ړ�����
	GSvector3 MoveInput();

public:
	GSvector3 input_ = { 0,0,0 };	//����1
	GSvector3 input2_ = { 0,0,0 };	//����2(1�t���[���O�̓���)

private:
	//�A�j���[�V�������b�V��
	AnimatedMesh	mesh_;
	//���[�V�����ԍ�
	GSuint			motion_;
	//���[�V�����̃��[�v�w��
	bool			motion_loop_;
	//��ԃ^�C�}
	float			state_timer_;

	// ���
	app::StateMachine stateMachine_;
	//���[�V�����̊J�n����
	float startTime_{ 0.0f };
	//���[�V�����̃��[�v�w��
	bool motionLoop_;
	//�A�j���Đ����x
	float animSpeed_{ 1.0f };
	//���[�v�{��
	float lerpSize_{ 0.5f };
	//������������ɑ������[�V�������I�����܂���
	float motionEndTimePlus_{ 0.0f };
	//�Ō�Ɍ����Ă�������(�����̒l�͍ŏ��Ɍ����Ă������)
	GSvector3 input_Direction_{ 0,0,1 };

	bool isGround_{ false };

	//�J�����̒����_(�W�����v�ŕς�邽�ߕϐ��ɂ��Ă���)
	GSvector3 cameraLookPoint_{ 0,0,0 };

	float debugFloat_[7]{ 0.0f,0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };

	GSvector3 beamDirection_ { GSvector3::zero()};

	//�n�ʂƏՓ˔��肷�邩
	bool canCollideField_{ true };
	
	float footOffset_{ -1.0f };

};

#endif