#ifndef PLAYER_QUICKWIRE_STATE_H_
#define PLAYER_QUICKWIRE_STATE_H_

#include <memory>

#include "app/StateMachine/StateBase.h"
#include "app/Actors/Player/Player.h"

// �v���C���[�̈ړ������N���X
class PlayerQuickWireState : public app::StateBase
{
public:
	// �R���X�g���N�^
	PlayerQuickWireState(Player* parent, IWorld* world, app::StateMachine* stateMachine);
	// �f�X�g���N�^
	~PlayerQuickWireState() = default;
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




private:
	Player* parent_;
	IWorld* world_;
	GSvector3 position_;
	GSvector3 my_Input_Direction_{ 0,0,0 };
	GSvector3 velocity_{ 0.0f,0.0f,0.0f }; //���x
	GSvector3 wirePosition_{ 0,0,0 }; //�U��q�̒��S

	float timer_{ 0.0f };
	float time_{ 60.0f };

};

#endif