#ifndef WORLD_H_ 
#define WORLD_H_

#include "IWorld.h"
#include "app/Actors/ActorManager/ActorManager.h"
#include "app/Field/Field.h"
#include "app/Effect/EffectManager.h"


#include <gslib.h>

// ワールドクラス
class World :public IWorld {
public:
	// コンストラクタ
	World();
	// デストラクタ
	~World();
	// 更新
	void update(float delta_time);
	// 描画
	void draw() const;
	//ポストエフェクト時の描画
	void postDraw()const;
	//通常時の描画
	void OnDraw()const;
	// 消去
	void clear();
	// カメラの追加
	void addCamera(std::shared_ptr<Actor> camera);
	// ライトの追加
	void addLight(std::shared_ptr<Actor> light);
	// フィールドの追加
	void addField(std::shared_ptr<Field> field);

	
	// アクターの追加
	template <class T, class... Args>
	void addactor(Args&&... args) {
		actors_.add<T>(args...);
	}
	// アクターの追加
	virtual void addActor(std::shared_ptr<Actor> actor) override;

	

	// アクターの検索
	virtual std::shared_ptr<Actor> findActor(const std::string& name) const override;
	// 指定したタグ名を持つアクターの検索
	virtual std::vector<std::shared_ptr<Actor>> findActorWithTag(const std::string& tag) const override;
	// アクター数を返す
	virtual int countActor() const override;
	// 指定したタグ名を持つアクター数を返す
	virtual int countActorWithTag(const std::string& tag) const override;
	// アクターリストを取得
	std::list<std::shared_ptr<Actor>>& getActorsList() override;
	// アクターリストをクリア
	void actorsClear() override;

	void addScore(int score) override;

	// カメラの取得
	virtual std::shared_ptr<Actor> camera() override;
	// タイトの取得
	virtual std::shared_ptr<Actor> light() override;
	// フィールドの取得
	virtual std::shared_ptr<Field> field() override;

	// Guiの追加
	//void addGui(std::shared_ptr<Gui> gui) override;

	// Effectの追加
	Effect* addEffect(std::shared_ptr<Effect> effect) override;
	// Effectの検索
	std::shared_ptr<Effect> findEffect(const std::string& name) override;
	//エフェクトのクリア
	virtual void EffectClear()override;







	// ゲームオーバー
	virtual void gameOver() override;
	// ゲームオーバーの初期化
	virtual void gameOverReset() override;

	// ゲームオーバーか？
	bool IsGameOver() const;

	// ゲームクリア
	virtual void gameClear() override;
	// ゲームクリアの初期化
	virtual void gameClearReset() override;

	// ゲームクリアか？
	bool IsGameClear() const;

	// ゲーム終了
	void gameEnd() override;
	// ゲーム終了か？
	bool isGameEnd() const;

	


	// コピー禁止
	World(const World& other) = delete;
	World& operator = (const World& other) = delete;

private:
	// アクターマネージャー
	ActorManager actors_;

	// Effectマネージャー
	EffectManager effectManager_;

	// ライト
	std::shared_ptr<Actor> light_{ nullptr };
	// カメラ
	std::shared_ptr<Actor> camera_{ nullptr };
	// フィールド
	std::shared_ptr<Field> field_{ nullptr };



private:

	// ゲームオーバーか？
	bool		 isGameOver_{ false };
	// ゲームクリアか？
	bool		 isGameClear_{ false };
	// ゲーム終了
	bool		 isGameEnd_{ false };



};

#endif