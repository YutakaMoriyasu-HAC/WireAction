#include "GamePlayScene.h"
#include "app/Field/Field.h"
#include "app/Actors/CameraTps/CameraRotateAround.h"
#include "app/Light/Light.h"
#include "app/Actors/Player/Player.h"
#include "app/NameList/Assets.h"

//開始
void GamePlayScene::start() {
	//終了フラグを初期化
	is_end_ = false;

	//プレイヤーのメッシュの読み込み
	gsLoadSkinMesh(Mesh_Player, "resource/Assets/Player/player.msh");

	//スカイドーム用のメッシュを読み込む
	gsLoadMesh(Mesh_Skybox, "resource/Assets/Skybox/skydome.msh");
	//描画用のオクトリーを読み込む
	gsLoadOctree(Octree_Stage, "resource/Assets/Octree/stage.oct");
	//衝突判定用のオクトリーを読み込む
	gsLoadOctree(Octree_Collider, "resource/Assets/Octree/stage_collider.oct");

	//フィールドクラスの追加
	world_.add_field(new Field{ Octree_Stage, Octree_Collider, Mesh_Skybox });
	//カメラクラスの追加
	world_.add_camera(new CameraRotateAround{ &world_, GSvector3{ 0.0f, 3.2f, -4.8f }, GSvector3{ 0.0f, 1.92f, 0.0f } });
	//ライトクラスの追加
	world_.add_light(new Light{ &world_ });

	//プレイヤーの追加
	world_.add_actor(new Player{ &world_, GSvector3{ 0.0f, 0.125f, 0.0f } });
	

}

//更新
void GamePlayScene::update(float delta_time) {
	//エンターキーでシーン終了としておく
	if (gsGetKeyTrigger(GKEY_RETURN)) {
		is_end_ = true;
	}
	//ワールドの更新
	world_.update(delta_time);
}

//描画
void GamePlayScene::draw() const {
	//ワールドの描画
	world_.draw();
}

//終了しているか？
bool GamePlayScene::is_end() const {
	return is_end_;		//終了フラグを返す
}

//次のシーンを返す
std::string GamePlayScene::next() const {
	return "TitleScene";
}

//終了
void GamePlayScene::end() {
	//ワールドを消去
	world_.clear();
	//メッシュの削除
	gsDeleteSkinMesh(Mesh_Player);
	gsDeleteMesh(Mesh_Skybox);
	//オクトリーの削除
	gsDeleteOctree(Octree_Stage);
	gsDeleteOctree(Octree_Collider);
}