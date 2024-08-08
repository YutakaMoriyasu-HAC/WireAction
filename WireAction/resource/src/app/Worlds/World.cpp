#include "World.h"
#include "app/Actors/Actor/Actor.h"

#include<GSeffect.h>

World::World()
{
}

// デストラクタ
World::~World() {
    clear();
}

// 更新
void World::update(float delta_time) {
    // フィールドの更新
    //field_->update(delta_time);
    // アクターの更新
    actors_.update(delta_time);
    // アクターの衝突
    actors_.collide();
    //field()->collide(&actors_); 
    // カメラの更新
    camera_->update(delta_time);
    // ライトの更新
    //light_->update(delta_time);
    //敵ジェネレーターの更新
    //enemyGenerator_->update(delta_time);
    //guiマネージャーの更新
    //guiManager_.update(delta_time);
    // エフェクトのupdate
    //effectManager_.update(delta_time);
    // エフェクトの更新処理
    //gsUpdateEffect(delta_time);




    // アクターの消去
    actors_.remove();
}

// 描画
void World::draw() const {


}

void World::postDraw() const
{
    //レンダーターゲットの設定
    //postEffectManager_.start();

    // シーンの描画

    // カメラの設定
    camera_->draw();
    // エフェクト用のカメラを設定
    //gsSetEffectCamera();
    // フィールドの描画
    //field_->draw();
    // ライトの設定
    //light_->draw();
    // アクターの描画
    actors_.draw();
    // エフェクトのdraw
    //effectManager_.draw();
    // エフェクトの描画
    //gsDrawEffect();
    // レンダーターゲットの解除
    //postEffectManager_.end();




}

void World::OnDraw() const
{
    // シーンの描画
// カメラの設定
    camera_->draw();
    // エフェクト用のカメラを設定
    //gsSetEffectCamera();


    // フィールドの描画
    //field_->draw();
    // ライトの設定
    //light_->draw();
    // アクターの描画
    actors_.draw();
    // エフェクトのdraw
    effectManager_.draw();
    // エフェクトの描画
    gsDrawEffect();
}

// 消去
void World::clear() {
    //アクターの消去
    actors_.clear();
    // カメラを消去
    camera_.reset();
    // ライトを消去
    light_.reset();
    //エフェクトのクリア
    effectManager_.clear();
    //GUIマネージャーの消去
    //guiManager_.clear();

    isGameClear_ = false;
    isGameOver_ = false;
    isGameEnd_ = false;
}

// カメラの追加
void World::addCamera(std::shared_ptr<Actor> camera) {
    camera_ = camera;
}

// ライトの追加
void World::addLight(std::shared_ptr<Actor> light) {
    light_ = light;
}

void World::addField(std::shared_ptr<Field> field)
{
    field_ = field;
}




// アクターの追加
void World::addActor(std::shared_ptr<Actor> actor) {
    actors_.add(actor);
}

// アクターの検索
std::shared_ptr<Actor> World::findActor(const std::string& name) const {
    return actors_.find(name);
}

// 指定したタグ名を持つアクターの検索
std::vector<std::shared_ptr<Actor>> World::findActorWithTag(const std::string& tag) const {
    return actors_.findWithTag(tag);
}

// アクター数を返す
int World::countActor() const {
    return actors_.count();
}

// 指定したタグ名を持つアクター数を返す
int World::countActorWithTag(const std::string& tag) const {
    return actors_.countWithTag(tag);
}

std::list<std::shared_ptr<Actor>>& World::getActorsList()
{
    return actors_.getActorList();
}

void World::actorsClear()
{
    actors_.clear();
}

void World::addScore(int score)
{
}

// カメラの取得
std::shared_ptr<Actor> World::camera() {
    return camera_;
}

// ライトの取得
std::shared_ptr<Actor> World::light() {
    return light_;
}

std::shared_ptr<Field> World::field()
{
    return field_;
}

/*void World::addGui(std::shared_ptr<Gui> gui)
{
    //guiManager_.add(gui);
}*/

Effect* World::addEffect(std::shared_ptr<Effect> effect)
{
    effectManager_.add(effect);
    return effect.get();
}

std::shared_ptr<Effect> World::findEffect(const std::string& name)
{
    return effectManager_.findEffect(name);
}

void World::EffectClear()
{
    effectManager_.clear();
}












// ゲームオーバー
void World::gameOver() {
    isGameOver_ = true;
}

// ゲームオーバーの初期化
void World::gameOverReset() {
    isGameOver_ = false;
}

// ゲームオーバーか？
bool World::IsGameOver() const {
    return isGameOver_;
}

// ゲームクリア
void World::gameClear() {
    isGameClear_ = true;
}

//  ゲームクリアの初期化
void World::gameClearReset() {
    isGameClear_ = false;
}

// ゲームクリアか？
bool World::IsGameClear() const
{
    return isGameClear_;
}

void World::gameEnd()
{
    isGameEnd_ = true;
}

bool World::isGameEnd() const
{
    return isGameEnd_;
}



