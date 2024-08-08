#include "ActorManager.h"
#include "app/Actors/Actor/Actor.h"

// デストラクタ
ActorManager::~ActorManager() {
    clear();
}

// アクターの追加
void ActorManager::add(std::shared_ptr<Actor> actor) {
    actors_.push_back(actor);
}

// アクターの更新
void ActorManager::update(float delta_time) {
    for (auto actor : actors_) {
        if (actor->is_dead())continue;
        actor->update(delta_time);
    }
}



// アクターの描画
void ActorManager::draw() const {
    for (auto actor : actors_) {
        if (actor->is_dead())continue;
        actor->draw();
    }
}



// アクターの衝突判定
void ActorManager::collide() {
    for (auto i = actors_.begin(); i != actors_.end(); ++i) {
        for (auto j = std::next(i); j != actors_.end(); ++j) {
            (*i)->collide(**j);
        }
    }
}

// 死亡しているアクターの削除
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

// アクターの検索
std::shared_ptr<Actor> ActorManager::find(const std::string& name) const {
    for (auto actor : actors_) {
        if (actor->name() == name) {
            return actor;
        }
    }
    return nullptr;
}

// 指定したタグ名を持つアクターの検索
	std::vector<std::shared_ptr<Actor>> ActorManager::findWithTag(const std::string& tag) const{
    std::vector<std::shared_ptr<Actor>> result;
    for (auto actor : actors_) {
        if (actor->tag() == tag) {
            result.push_back(actor);
        }
    }
    return result;
}

// アクター数を返す
int ActorManager::count() const {
    return (int)actors_.size();
}

// 指定したタグのアクター数を返す
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

// 消去
void ActorManager::clear() {
    actors_.clear();
    // メモリの開放
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