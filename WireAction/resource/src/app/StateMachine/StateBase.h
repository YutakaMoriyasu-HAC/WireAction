#ifndef APP_STATE_BASE_STATEBASE_H_
#define APP_STATE_BASE_STATEBASE_H_

#include <memory>

namespace app {

	class StateMachine;	// �X�e�[�g�}�V���̑O���錾

	// �X�e�[�g�̊��N���X
	class StateBase {
	public:
		StateBase(StateMachine* stateMachine) : 
			stateMachine_(stateMachine){}
		// ���z�f�X�g���N�^
		virtual ~StateBase() = default;
		// ������
		virtual void init() = 0;
		// �I��
		virtual void final() = 0;
		// �X�V
		virtual void update() = 0;
		// �x���X�V
		virtual void lateUpdate() = 0;
		// �`��
		virtual void draw() const = 0;
		// �x���`��
		virtual void lateDraw() const = 0;

		// �X�e�[�g���A�N�e�B�u���̂ݕ`��
		virtual void drawOnlyWhenActive() const {};

	public:
		// ���߂��邩�̃t���O���擾
		bool isTransparency() const { return isTransparency_; }

	protected:
		StateMachine* stateMachine_;

	protected:
		bool isTransparency_{ false };
	};
}

#endif