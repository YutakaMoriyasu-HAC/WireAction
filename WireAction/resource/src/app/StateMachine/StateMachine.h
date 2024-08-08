#ifndef APP_STATE_MACHINE_STATEMACHINE_H_
#define APP_STATE_MACHINE_STATEMACHINE_H_

#include <memory> // ���X�}�[�g�|�C���^���g�p����̂ɕK�v
#include <stack>
#include <unordered_map>
#include <functional>

namespace app {

	class StateBase;

	// �X�e�[�g�}�V��
	class StateMachine
	{
	public:
		StateMachine() = default;
		~StateMachine();

		/// <summary>
		/// �X�e�[�g��ǉ�
		/// </summary>
		/// <param name="stateTag">�X�e�[�g�̔ԍ�(namelist��enum��p��)</param>
		/// <param name="state">std::make_shared�ŃV�F�A�[�h�|�C���^�[�𐶐����Ĉ����ɓn��</param>
		void addState(const int stateTag, std::shared_ptr<StateBase> state);

		/// <summary>
		/// �X�e�[�g��ǉ�
		/// </summary>
		/// <typeparam name="T">�ǉ�����State�̃N���X</typeparam>
		/// <typeparam name="...Args"></typeparam>
		/// <param name="stateTag">�X�e�[�g�̔ԍ�</param>
		/// <param name="...args">State�N���X�̈���</param>
		template <class T, class... Args>
		void addState(const int stateTag, Args&&... args) {
			stateList_[stateTag] = std::make_shared<T>(args...);
		}

		/// <summary>
		/// �����X�e�[�g��ݒ肷��(�X�^�b�N���̃X�e�[�g��Final�֐��͌Ă΂�Ȃ�)
		/// </summary>
		/// <param name="stateTag">�����ݒ肷��X�e�[�g�̔ԍ�</param>
		void reset(const int stateTag = -1);
		// �X�V
		void update();
		// �x���X�V
		void lateUpdate();
		// �`��
		void draw() const;
		// �x���`��
		void lateDraw() const;

		/// <summary>
		/// ���̃X�e�[�g�֑J��(�X�^�b�N���猻�݂̃X�e�[�g�����o���A���̃X�e�[�g��}��)
		/// </summary>
		/// <param name="stateTag">���̃X�e�[�g�̔ԍ�</param>
		void nextState(const int stateTag);

		/// <summary>
		/// �X�e�[�g��ύX(�X�^�b�N����ɂ��āA�ύX����X�e�[�g��}��)
		/// </summary>
		/// <param name="stateTag">�ύX����X�e�[�g�̔ԍ�</param>
		void chengeState(const int stateTag);

		/// <summary>
		/// �X�e�[�g����悹(�X�^�b�N�֎��̃X�e�[�g��}��)�B
		/// ���݂̃X�e�[�g���ꎞ���f���Ď��̃X�e�[�g�֑J�ڏo����B
		/// PopState()���ĂԂƌ��݂̃X�e�[�g�𑱂����瓮������B
		/// </summary>
		/// <param name="stateTag">���̃X�e�[�g�̔ԍ�</param>
		void pushState(const int stateTag);

		/// <summary>
		/// �X�e�[�g��1�߂�(�X�^�b�N����X�e�[�g���P���o��)
		/// </summary>
		void popState();
		// �ǉ������X�e�[�g���N���A
		void clearState();
		// ���݂̃X�e�[�g�̌o�ߎ��Ԃ��擾
		const float getNowStateTimer() const;
		// ���݂̃X�e�[�gID���擾
		const int getNowStateID() const;
		// 1�O�̃X�e�[�gID���擾
		const int getPrevStateID() const;

	private:
		int nowStateID_{ -1 };		// ���݂̃X�e�[�gid
		int prevStateID_{ -1 };	// 1�O�̃X�e�[�gid

	private:
		std::stack<float> timer_;
		std::shared_ptr<StateBase> currentState_;
		std::stack<std::shared_ptr<StateBase>> prevsState_;
		std::unordered_map<int, std::shared_ptr<StateBase>> stateList_;

	};
}

#endif
