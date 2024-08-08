#include "ActorManager.h"
#include "app/Actors/Actor/Actor.h"

// �f�X�g���N�^
ActorManager::~ActorManager() {
    clear();
}

// �A�N�^�[�̒ǉ�
void ActorManager::add(std::shared_ptr<Actor> actor) {
    actors_.push_back(actor);
}

// �A�N�^�[�̍X�V
void ActorManager::update(float delta_time) {
    for (auto actor : actors_) {
        if (actor->is_dead())continue;
        actor->update(delta_time);
    }
}



// �A�N�^�[�̕`��
void ActorManager::draw() const {
    for (auto actor : actors_) {
        if (actor->is_dead())continue;
        actor->draw();
    }
}



// �A�N�^�[�̏Փ˔���
void ActorManager::collide() {
    for (auto i = actors_.begin(); i != actors_.end(); ++i) {
        for (auto j = std::next(i); j != actors_.end(); ++j) {
            (*i)->collide(**j);
        }
    }
}

// ���S���Ă���A�N�^�[�̍폜
void ActorManager::remove() {
    for (auto i = actors_.begin(); i != actors_.end(); ) {
        if ((*i)->is_dead()) {
            i = actors_.erase(i);
        }
        else {
            ++i;
        }
    }
}

// �A�N�^�[�̌���
std::shared_ptr<Actor> ActorManager::find(const std::string& name) const {
    for (auto actor : actors_) {
        if (actor->name() == name) {
            return actor;
        }
    }
    return nullptr;
}

// �w�肵���^�O�������A�N�^�[�̌���
	std::vector<std::shared_ptr<Actor>> ActorManager::findWithTag(const std::string& tag) const{
    std::vector<std::shared_ptr<Actor>> result;
    for (auto actor : actors_) {
        if (actor->tag() == tag) {
            result.push_back(actor);
        }
    }
    return result;
}

// �A�N�^�[����Ԃ�
int ActorManager::count() const {
    return (int)actors_.size();
}

// �w�肵���^�O�̃A�N�^�[����Ԃ�
int ActorManager::countWithTag(const std::string& tag) const {
    int result = 0;
    for (auto actor : actors_) {
        if (actor->tag() == tag) {
            ++result;
        }
    }
    return result;

}

std::list<std::shared_ptr<Actor>>& ActorManager::getActorList()
{
    return actors_;
}

// ����
void ActorManager::clear() {
    actors_.clear();
    // �������̊J��
    std::list<std::shared_ptr<Actor>>().swap(actors_);
}

void ActorManager::killAllEnemies(const std::string& tag) {

    for (auto i = actors_.begin(); i != actors_.end(); ) {
        if ((*i)->tag() == tag) {
            i = actors_.erase(i);
        }
        else {
            ++i;
        }
    }
}