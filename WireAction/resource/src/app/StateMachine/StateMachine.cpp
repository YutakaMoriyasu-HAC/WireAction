#include "StateMachine.h"

#include "app/time/Time.h"
#include "StateBase.h"

using namespace app;

// �f�X�g���N�^
StateMachine::~StateMachine()
{
	reset();		// �X�^�b�N�����
	clearState();	// �ǉ������X�e�[�g�̃��X�g�����
}

/*=============================================================================
*
* std::make_shared<�N���X��>()�ŃV�F�A�[�h�|�C���^�[�𐶐����Ĉ����ɓn��
*
*============================================================================*/
// �X�e�[�g��ǉ�
void StateMachine::addState(const int stateTag, std::shared_ptr<StateBase> state)
{
	stateList_[stateTag] = state;
}
// �X�e�[�g���Z�b�g����
void StateMachine::reset(const int stateTag)
{
	// �X�^�b�N�����
	std::stack<std::shared_ptr<StateBase>>().swap(prevsState_);
	// �^�C�}�����
	std::stack<float>().swap(timer_);
	// �^�Oid��-1�Ȃ�I��
	if (stateTag == -1) return;
	// �V�����X�e�[�g��ǉ�
	currentState_ = stateList_[stateTag];
	// �V�����X�e�[�g�̏�����
	currentState_->init();
	// �V�����^�C�}��ǉ�
	timer_.push(0.0f);
	// ���݂̃X�e�[�g���Z�b�g
	nowStateID_ = stateTag;
}
// �X�V
void StateMachine::update()
{
	// ���݂̃X�e�[�g��null�̏ꍇ�͏��������Ȃ�
	if (currentState_== nullptr) return;
	// �^�C�}�̍X�V
	timer_.top() += app::Time::deltaTime;
	// ���݂̃X�e�[�g�̍X�V
	currentState_->update();
}
// �x���X�V
void StateMachine::lateUpdate()
{
	// ���݂̃X�e�[�g�̒x���X�V
	currentState_->lateUpdate();
}
// �`��
void StateMachine::draw() const
{
	// �O��̃X�e�[�g�̕`��
	if (currentState_->isTransparency() && !prevsState_.empty())
		prevsState_.top()->draw();
	// ���݂̃X�e�[�g�̕`��
	currentState_->draw();

	// �A�N�e�B�u���̂ݕ`��
	currentState_->drawOnlyWhenActive();
}
// �x���`��
void StateMachine::lateDraw() const
{
	// �O��̃X�e�[�g�̒x���`��
	if (currentState_->isTransparency() && !prevsState_.empty())
		prevsState_.top()->lateDraw();
	// ���݂̃X�e�[�g�̒x���`��
	currentState_->lateDraw();
}
// ���̃X�e�[�g��(���݂̃X�e�[�g��ύX)
void StateMachine::nextState(const int stateTag)
{
	// �X�e�[�g�̏I������
	currentState_->final();
	// ���݂̃X�e�[�g��1�O�̃X�e�[�g��
	prevStateID_ = nowStateID_;
	// �^�C�}��1���o��
	timer_.pop();
	// �V�����X�e�[�g��ǉ�
	currentState_ = stateList_[stateTag];
	// �V�����X�e�[�g�̏�����
	currentState_->init();
	// �V�����^�C�}��ǉ�
	timer_.push(0.0f);
	// �V�����X�e�[�g�����݂̃X�e�[�g��
	nowStateID_ = stateTag;
}
// �X�e�[�g��ύX(�X�^�b�N����ɂ��ĕύX)
void StateMachine::chengeState(const int stateTag)
{
	// ���݂̃X�e�[�g��1�O�̃X�e�[�g��
	if (nowStateID_ != -1) prevStateID_ = nowStateID_;
	// ���݂̃X�e�[�g�̏I������
	currentState_->final();
	// ��ɂȂ�܂ŏI���������Ă�
	while (!prevsState_.empty())
	{
		// �X�e�[�g�̏I������
		prevsState_.top()->final();
		// �X�e�[�g��1���o��
		prevsState_.pop();
	}
	std::stack<float>().swap(timer_);
	// �V�����X�e�[�g��ǉ�
	currentState_ = stateList_[stateTag];
	// �V�����X�e�[�g�̏�����
	currentState_->init();
	// �V�����^�C�}��ǉ�
	timer_.push(0.0f);
	// �V�����X�e�[�g�����݂̃X�e�[�g��
	nowStateID_ = stateTag;
}
// �X�e�[�g����悹
void StateMachine::pushState(const int stateTag)
{
	// ���݂̃X�e�[�g��1�O�̃X�e�[�g��
	if (nowStateID_ != -1) prevStateID_ = nowStateID_;
	// ���݂̃X�e�[�g��O�̃X�e�[�g�ֈڂ�
	prevsState_.push(currentState_);
	// �V�����X�e�[�g��ǉ�
	currentState_ = stateList_[stateTag];
	// �V�����X�e�[�g�̏�����
	currentState_->init();
	// �V�����^�C�}��ǉ�
	timer_.push(0.0f);
	// �V�����X�e�[�g�����݂̃X�e�[�g��
	nowStateID_ = stateTag;
}
// �X�e�[�g��1�߂�
void StateMachine::popState()
{
	if (prevsState_.empty()) return;
	// �ꎞ�I�ɕۑ�
	int id = prevStateID_;
	// ���݂̃X�e�[�g��1�O�̃X�e�[�g��
	if (nowStateID_ != -1) prevStateID_ = nowStateID_;
	else return;
	// �X�e�[�g�̏I������
	currentState_->final();
	// �O�̃X�e�[�g�����݂̃X�e�[�g�ֈڂ�
	currentState_ = prevsState_.top();
	// �X�e�[�g��1���o��
	prevsState_.pop();
	// �^�C�}��1���o��
	timer_.pop();
	// �V�����X�e�[�g�����݂̃X�e�[�g��
	nowStateID_ = id;
}
// �ǉ������X�e�[�g���N���A
void StateMachine::clearState()
{
	stateList_.clear();
	std::unordered_map<int, std::shared_ptr<StateBase>>().swap(stateList_);
	std::stack<float>().swap(timer_);
}
// ���݂̃X�e�[�g�̌o�ߎ��Ԃ��擾
const float StateMachine::getNowStateTimer() const
{
	return timer_.top();
}
// ���݂̃X�e�[�gID���擾
const int StateMachine::getNowStateID() const
{
	return nowStateID_;
}
// 1�O�̃X�e�[�gID���擾
const int StateMachine::getPrevStateID() const
{
	return prevStateID_;
}
