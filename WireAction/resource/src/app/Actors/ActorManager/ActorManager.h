#ifndef ACTOR_MANAGER_H_
#define ACTOR_MANAGER_H_

#include <list>
#include <vector>
#include <string>
#include <memory>

class Actor;	// �A�N�^�[�N���X�̑O���錾

// �A�N�^�[�}�l�[�W���[
class ActorManager {
public:
	// �f�t�H���g�R���X�g���N�^
	ActorManager() = default;
	// �f�X�g���N�^
	virtual ~ActorManager();
	// �A�N�^�[�ǉ�
	void add(std::shared_ptr<Actor> actor);
	// �A�N�^�[�̒ǉ�
	template <class T, class... Args>
	void add(Args&&... args) {
		actors_.push_back(std::make_shared<T>(args...));
	}
	// �A�N�^�[�̍X�V
	void update(float delta_time);
	// �A�N�^�[�̕`��
	void draw() const;
	// �A�N�^�[�̏Փ˔���
	void collide();
	// ���S���Ă���A�N�^�[�̍폜
	void remove();
	// �A�N�^�[�̌���
	std::shared_ptr<Actor> find(const std::string& name) const;
	// �w�肵���^�O�������A�N�^�[�̌��� 
	std::vector<std::shared_ptr<Actor>> findWithTag(const std::string& tag)const;
	// �A�N�^�[����Ԃ�
	int count() const;
	// �w�肵���^�O�������A�N�^�[����Ԃ�
	int countWithTag(const std::string& tag) const;
	// �A�N�^�[���X�g��Ԃ�
	std::list<std::shared_ptr<Actor>>& getActorList();
	// ����
	void clear();

	//�w�肵���A�N�^�[�̑S��
	void killAllEnemies(const std::string& tag);

	// �R�s�[�֎~
	ActorManager(const ActorManager& other) = delete;
	ActorManager& operator = (const ActorManager& other) = delete;

protected:
	// �A�N�^�[���X�g
	std::list<std::shared_ptr<Actor>> actors_;
};

#endif