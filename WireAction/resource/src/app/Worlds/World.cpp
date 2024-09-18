#include "app/Worlds/World.h"
#include "app/Field/Field.h"
#include "app/Actors/Actor/Actor.h"
#include <GSeffect.h>

//デストラクタ
World::~World() {
	clear();
}

//更新
void World::update(float delta_time) {
	

	//フィールドの更新
	field_->update(delta_time);
	//アクターの更新
	actors_.update(delta_time);
	//アクターの衝突
	actors_.collide();
	//アクターの遅延更新
	actors_.late_update(delta_time);
	//アクターの消去
	actors_.remove();
	//カメラの更新
	camera_->update(delta_time);
	//ライトの更新
	light_->update(delta_time);
	// エフェクトのupdate
	effectManager_.update(delta_time);
	// エフェクトの更新処理
	gsUpdateEffect(delta_time);
}

//描画
void World::draw() const {
	
	//カメラの設定
	camera_->draw();
	//エフェクト用のカメラを設定
	gsSetEffectCamera();
	//ライトの設定
	light_->draw();

	// シャドウマップの描画
	gsDrawShadowMap(World::shadowMapCallback, (void*)this);

	//フィールドの描画
	field_->draw();
	//アクターの描画
	actors_.draw();
	//半透明アクターの描画
	actors_.draw_transparent();
	// エフェクトのdraw
	effectManager_.draw();
	// エフェクトの描画
	gsDrawEffect();
	//GUIの描画
	actors_.draw_gui();
}

//消去
void World::clear() {
	//アクターの消去
	actors_.clear();
	// カメラを消去
	camera_.reset();
	// ライトを消去
	light_.reset();
	//エフェクトのクリア
	effectManager_.clear();
}

//カメラの追加
void World::add_camera(std::shared_ptr<Actor> camera) {
	camera_ = camera;
}

//ライトの追加
void World::add_light(std::shared_ptr<Actor> light) {
	light_ = light;
}

//フィールドの追加
void World::add_field(std::shared_ptr<Field> field) {
	field_ = field;
}

// シャドウマップの描画用の関数
void World::shadowMapCallback(void* param, const GSmatrix4*, const GSmatrix4*)
{
	World* self = (World*)param;
	// シャドウマップにアクターを描画
	self->actors_.draw();

}

//アクターの追加
void World::add_actor(std::shared_ptr<Actor> actor) {
	actors_.add(actor);
}

//アクターの検索
std::shared_ptr<Actor> World::find_actor(const std::string& name)const {
	return actors_.find(name);
}

//指定したタグ名を持つアクターの検索
std::vector<std::shared_ptr<Actor>>World::find_actor_with_tag(const std::string& tag) const {
	return actors_.find_with_tag(tag);
}

//アクター数を返す
int World::count_actor() const {
	return actors_.count();
}

//指定したタグ名を持つアクター数を返す
int World::count_actor_with_tag(const std::string& tag) const {
	return actors_.count_with_tag(tag);
}

//メッセージ送信
void World::send_message(const std::string& message, void* param) {
	actors_.send_message(message, param);
}

//カメラの取得
std::shared_ptr<Actor> World::camera() {
	return camera_;
}

//ライトの取得
std::shared_ptr<Actor> World::light() {
	return light_;
}

//フィールドの取得
std::shared_ptr<Field> World::field() {
	return field_;
}

//エフェクトの追加
Effect* World::addEffect(std::shared_ptr<Effect> effect)
{
	effectManager_.add(effect);
	return effect.get();
}

//エフェクトの検索
std::shared_ptr<Effect> World::findEffect(const std::string& name)
{
	return effectManager_.findEffect(name);
}

//エフェクトのクリア
void World::EffectClear()
{
	effectManager_.clear();
}