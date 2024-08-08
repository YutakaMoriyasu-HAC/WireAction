#ifndef IWORLD_H_ 
#define IWORLD_H_

#include <string>
#include <vector>
#include <memory>
#include <list>
#include <gslib.h>

class Actor;
class Field;
class Item;
class InventoryFrame;
class Effect;
class Gui;
class TimeLimitClock;
class Gauge;
class PostEffect;
class KillCountManager;
// ワールド抽象インターフェース
class IWorld
{
public:
	// 仮想デストラクタ
	virtual ~IWorld() = default;


	// アクターを追加
	virtual void addActor(std::shared_ptr<Actor> actor) = 0;

	// アクターの検索
	virtual std::shared_ptr<Actor> findActor(const std::string& name) const = 0;
	// 指定したタグ名を持つアクターの検索
	virtual std::vector<std::shared_ptr<Actor>> findActorWithTag(const std::string& tag) const = 0;
	// アクター数を返す
	virtual int countActor() const = 0;
	// 指定したタグ名を持つアクター数を返す
	virtual int countActorWithTag(const std::string& tag) const = 0;
	// アクターのリストを取得
	virtual std::list<std::shared_ptr<Actor>>& getActorsList() = 0;
	// アクターリストをクリア
	virtual void actorsClear() = 0;



	// スコアの加算
	virtual void addScore(int score) = 0;
	// ゲームオーバー
	virtual void gameOver() = 0;
	// ゲームオーバーの初期化
	virtual void gameOverReset() = 0;
	// ゲームクリア
	virtual void gameClear() = 0;
	// ゲームクリアの初期化
	virtual void gameClearReset() = 0;

	// ゲーム終了
	virtual void gameEnd() = 0;

	// カメラの取得
	virtual std::shared_ptr<Actor> camera() = 0;
	// ライトの取得
	virtual std::shared_ptr<Actor> light() = 0;
	// フィールドの取得
	virtual std::shared_ptr<Field> field() = 0;

	//Guiの追加
	//virtual void addGui(std::shared_ptr<Gui> gui) = 0;

	// エフェクトの追加
	virtual Effect* addEffect(std::shared_ptr<Effect> effect) = 0;
	// エフェクトの検索
	virtual std::shared_ptr<Effect> findEffect(const std::string& name) = 0;
	//エフェクトのクリア
	virtual void EffectClear() = 0;







	// ゲームオーバーか？
	virtual bool IsGameOver() const = 0;

	virtual bool IsGameClear() const = 0;




};

#endif