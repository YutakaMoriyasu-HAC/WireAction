#ifndef ACTOR_MANAGER_H_
#define ACTOR_MANAGER_H_

#include <list>
#include <vector>
#include <string>
#include <memory>

class Actor;	// アクタークラスの前方宣言

// アクターマネージャー
class ActorManager {
public:
	// デフォルトコンストラクタ
	ActorManager() = default;
	// デストラクタ
	virtual ~ActorManager();
	// アクター追加
	void add(std::shared_ptr<Actor> actor);
	// アクターの追加
	template <class T, class... Args>
	void add(Args&&... args) {
		actors_.push_back(std::make_shared<T>(args...));
	}
	// アクターの更新
	void update(float delta_time);
	// アクターの描画
	void draw() const;
	// アクターの衝突判定
	void collide();
	// 死亡しているアクターの削除
	void remove();
	// アクターの検索
	std::shared_ptr<Actor> find(const std::string& name) const;
	// 指定したタグ名を持つアクターの検索 
	std::vector<std::shared_ptr<Actor>> findWithTag(const std::string& tag)const;
	// アクター数を返す
	int count() const;
	// 指定したタグ名を持つアクター数を返す
	int countWithTag(const std::string& tag) const;
	// アクターリストを返す
	std::list<std::shared_ptr<Actor>>& getActorList();
	// 消去
	void clear();

	//指定したアクターの全滅
	void killAllEnemies(const std::string& tag);

	// コピー禁止
	ActorManager(const ActorManager& other) = delete;
	ActorManager& operator = (const ActorManager& other) = delete;

protected:
	// アクターリスト
	std::list<std::shared_ptr<Actor>> actors_;
};

#endif