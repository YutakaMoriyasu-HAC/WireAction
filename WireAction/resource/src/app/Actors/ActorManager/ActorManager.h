#ifndef ACTOR_MANAGER_H_
#define ACTOR_MANAGER_H_

#include<list>
#include<vector>
#include<string>
#include<memory>

class Actor;	//アクタークラスの前方宣言

//アクターマネージャー
class ActorManager {
public:
	//デフォルトコンストラクタ
	ActorManager() = default;
	//デストラクタ
	~ActorManager();
	//アクターの追加
	void add(std::shared_ptr<Actor> actor);
	// アクターの追加
	template <class T, class... Args>
	void add(Args&&... args) {
		actors_.push_back(std::make_shared<T>(args...));
	}
	//アクターの更新
	void update(float delta_time);
	//アクターの遅延更新
	void late_update(float delta_time);
	//アクターの描画
	void draw() const;
	//半透明アクターの描画
	void draw_transparent() const;
	//アクターのGUI描画
	void draw_gui() const;
	//アクターの衝突判定
	void collide();
	//死亡しているアクターの削除
	void remove();
	//アクターの検索
	std::shared_ptr<Actor> find(const std::string& name)const;
	//指定したタグ名を持つアクターの検索
	std::vector<std::shared_ptr<Actor>> find_with_tag(const std::string& tag) const;
	//アクター数を返す
	int count() const;
	//指定したタグ名を持つアクター数を返す
	int count_with_tag(const std::string& tag) const;
	//メッセージの送信
	void send_message(const std::string& message, void* param);
	//消去
	void clear();
	//コピー禁止
	ActorManager(const ActorManager& other) = delete;
	ActorManager& operator=(const ActorManager& other) = delete;

private:
	//アクターリスト
	std::list<std::shared_ptr<Actor>> actors_;
};

#endif