#ifndef ACTOR_H_
#define ACTOR_H_

#include <gslib.h>
#include <GStransform.h>
#include <string>
#include "app/BoundingSpheres/BoundingSphere.h"
#include "app/Tween/Tween.h"
#include "app/Ray/Line.h"

class IWorld;	//ワールド抽象インターフェースの前方宣言

//アクタークラス
class Actor {
public:
	//コンストラクタ
	Actor() = default;
	//仮想デストラクタ
	virtual ~Actor() = default;
	//更新
	virtual void update(float delta_time);
	//遅延更新
	virtual void lateUpdate(float delta_time);
	//描画
	virtual void draw() const;
	//半透明の描画
	virtual void draw_transparent() const;
	//GUIの描画
	virtual void draw_gui() const;
	//衝突リアクション
	virtual void react(Actor& other);
	//メッセージ処理
	virtual void handle_message(const std::string& message, void* param);
	//衝突判定
	void collide(Actor& other);
	//線分との衝突判定
	bool collide(const Line& line, GSvector3* intersect) const;

	//アクターとの衝突処理
	void collide_actor(Actor& other);


	//死亡する
	void die();
	//衝突しているか？
	bool is_collide(const Actor& other) const;
	//死亡しているか？
	bool is_dead() const;
	//名前を取得
	const std::string& name() const;
	//タグ名を取得
	const std::string& tag() const;
	//攻撃状態を取得
	const bool& attackState() const;
	//トランスフォームを取得(const版)
	const GStransform& transform() const;
	//トランスフォームを取得
	GStransform& transform();
	//移動量を取得
	const GSvector3& velocity() const;
	GSvector3& velocity();
	//移動量を設定
	void velocity(const GSvector3& vec);
	//衝突判定データを取得
	BoundingSphere collider() const;
	// 指定された場所までTweenで移動する
	TweenUnit& move_to(const GSvector3& to, float duration);
	//自然落下
	void gravityFall(float delta_time);

	
	//コピー禁止
	Actor(const Actor& other) = default;
	Actor& operator = (const Actor& other) = delete;

protected:
	//ワールド
	IWorld*			world_{ nullptr };
	//タグ名
	std::string		tag_;
	// 親、所有者タグ
	std::string ownerTag_;
	//名前
	std::string		name_;
	//攻撃状態
	bool		attackState_{false};
	//トランスフォーム
	GStransform		transform_;
	//移動量
	GSvector3		velocity_{ 0.0f,0.0f,0.0f };
	//衝突判定が有効か?
	bool			enable_collider_{ true };
	//衝突判定
	BoundingSphere	collider_;
	//死亡フラグ
	bool			dead_{ false };

	//重力値
	const float Gravity{ -0.016f }; //-0.016 //-0.008
};

#endif