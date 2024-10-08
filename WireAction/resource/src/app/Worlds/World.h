#ifndef WORLD_H_
#define WORLD_H_

#include "IWorld.h"
#include "app/Actors/ActorManager/ActorManager.h"
#include "app/Effect/EffectManager.h"
#include <gslib.h>

//ワールドクラス
class World : public IWorld {
public:
	//コンストラクタ
	World() = default;
	//デストラクタ
	~World();
	//更新
	void update(float delta_time);
	//描画
	void draw() const;
	//消去
	void clear();
	//カメラの追加
	void add_camera(std::shared_ptr<Actor> camera);
	//ライトの追加
	void add_light(std::shared_ptr<Actor> light);
	//フィールドの追加
	void add_field(std::shared_ptr<Field> field);
	// シャドウマップの描画用の関数
	static void shadowMapCallback(void* param, const GSmatrix4*, const GSmatrix4*);

	//アクターを追加
	virtual void add_actor(std::shared_ptr<Actor> actor) override;
	//アクターの検索
	virtual std::shared_ptr<Actor> find_actor(const std::string& name) const override;
	//指定したタグ名を持つアクターの検索
	virtual std::vector<std::shared_ptr<Actor>> find_actor_with_tag(const std::string& tag) const override;
	//アクター数を返す
	virtual int count_actor() const override;
	//指定したタグ名を持つアクター数を返す
	virtual int count_actor_with_tag(const std::string& tag) const override;
	//メッセージの送信
	virtual void send_message(const std::string& message, void* param = nullptr) override;

	//カメラの取得
	virtual std::shared_ptr<Actor> camera() override;
	//ライトの取得
	virtual std::shared_ptr<Actor> light() override;
	//フィールドの取得
	virtual std::shared_ptr<Field> field() override;

	// Effectの追加
	Effect* addEffect(std::shared_ptr<Effect> effect) override;
	// Effectの検索
	std::shared_ptr<Effect> findEffect(const std::string& name) override;
	//エフェクトのクリア
	virtual void EffectClear()override;

	//コピー禁止
	World(const World& other) = delete;
	World& operator = (const World& other) = delete;

private:
	//アクターマネージャー
	ActorManager actors_;
	// Effectマネージャー
	EffectManager effectManager_;
	//ライト
	std::shared_ptr<Actor>		 light_{ nullptr };
	//カメラ
	std::shared_ptr<Actor>		 camera_{ nullptr };
	//フィールド
	std::shared_ptr<Field>		 field_{ nullptr };
};

#endif