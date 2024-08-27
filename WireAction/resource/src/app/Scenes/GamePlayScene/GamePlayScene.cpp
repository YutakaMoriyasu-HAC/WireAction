#include "GamePlayScene.h"
#include "app/Field/Field.h"
#include "app/Actors/CameraTps/CameraRotateAround.h"
#include "app/Actors/CameraTps/CameraTps.h"
#include "app/Light/Light.h"
#include "app/Actors/Player/Player.h"
#include "app/NameList/Assets.h"
#include "app/Actors/Ball/Ball.h"
#include "app/Actors/Enemy/Enemy.h"
#include <GSstandard_shader.h>

//開始
void GamePlayScene::start() {

	//デフォルトシェーダーの初期化
	gsInitDefaultShader();

	//終了フラグを初期化
	is_end_ = false;

	//プレイヤーのメッシュの読み込み
	gsLoadSkinMesh(Mesh_Player, "resource/Assets/PlayerDog/player.msh");
	//gsLoadMesh(Mesh_PlayerBall, "resource/Assets/PlayerDog/playerBall.msh");
	gsLoadMesh(Mesh_Ball, "resource/Assets/Ball/Ball.msh");
	gsLoadSkinMesh(Mesh_EnemyBird, "resource/Assets/Enemy/EnemyBird/EnemyBird.mshb");

	//スカイドーム用のメッシュを読み込む
	gsLoadMeshFixed(Mesh_Skybox, "resource/Assets/Skybox/skydome.msh");
	//描画用のオクトリーを読み込む
	gsLoadOctree(Octree_Stage, "resource/Assets/TestStage/TestStageOc.oct");
	//衝突判定用のオクトリーを読み込む
	gsLoadOctree(Octree_Collider, "resource/Assets/TestStage/TestStageCol.oct");

	// ライトマップの読み込み
	gsLoadLightmap(LightMap, "resource/Assets/TestStage/LightMap/Lightmap.txt");
	// リフレクションプローブの読み込み
	gsLoadReflectionProbe(ReflectionProbe, "resource/Assets/TestStage/ReflectionProbe.txt");

	gsLoadTexture(Texture_Wire, "resource/Assets/Ball/wire.png");
	

	//フィールドクラスの追加
	world_.add_field(std::make_shared<Field>(Octree_Stage, Octree_Collider, Mesh_Skybox ));

	//プレイヤーの追加
	world_.add_actor(std::make_shared<Player>(&world_, GSvector3(0.0f, 0.125f, 0.0f)));

	//ballオブジェクトを配置(仮)
	world_.add_actor(std::make_shared<Ball>(&world_, GSvector3(0.0f, 6.0f, 0.0f)));
	world_.add_actor(std::make_shared<Ball>(&world_, GSvector3(-6.0f, 6.0f, -6.0f)));
	world_.add_actor(std::make_shared<Ball>(&world_, GSvector3(10.0f, 12.0f, -7.0f)));

	//敵
	world_.add_actor(std::make_shared<Enemy>(&world_, GSvector3(11.0f, 0.5f, 10.0f)));
	world_.add_actor(std::make_shared<Enemy>(&world_, GSvector3(8.0f, 0.5f, 10.0f)));
	world_.add_actor(std::make_shared<Enemy>(&world_, GSvector3(5.0f, 0.5f, 10.0f)));
	world_.add_actor(std::make_shared<Enemy>(&world_, GSvector3(2.0f, 0.5f, 10.0f)));
	world_.add_actor(std::make_shared<Enemy>(&world_, GSvector3(-1.0f, 0.5f, 10.0f)));
	world_.add_actor(std::make_shared<Enemy>(&world_, GSvector3(-4.0f, 0.5f, 10.0f)));

	//カメラクラスの追加
	world_.add_camera(std::make_shared<CameraTps>(&world_, GSvector3(8.0f, 3.0f, 0.0f), GSvector3(-5.0f, 0.0f, 0.0f)));
	//ライトクラスの追加
	world_.add_light(std::make_shared<Light>(&world_));

	//視錐台カリングを有効にする
	gsEnable(GS_FRUSTUM_CULLING);

	// シャドウマップの作成
	static const GSuint shadow_map_size[] = { 2048, 2048 };
	gsCreateShadowMap(2, shadow_map_size, GS_TRUE);
	// シャドウマップを適用する距離(視点からの距離）
	gsSetShadowMapDistance(60.0f);
	// カスケードシャドウマップの分割位置を調整（デフォルトは0.5）
	gsSetShadowMapCascadeLamda(0.7f);
	// シャドウの濃さを設定(0.0:濃い～1.0:薄い)
	gsSetShadowMapAttenuation(0.0f);

	

	/*/ フォグの設定
	const static float fog_color[4]{ 1.0f, 0.723f, 0.216f, 1.0f };
	const static float fog_start{ 10.0f };
	const static float fog_end{ 300.0f };
	glFogi(GL_FOG_MODE, GL_LINEAR);     // 線形フォグ
	glFogfv(GL_FOG_COLOR, fog_color);   // フォグの色
	glFogf(GL_FOG_START, fog_start);    // フォグの開始位置（視点からの距離）
	glFogf(GL_FOG_END, fog_end);        // フォグの終了位置（視点からの距離）
	glEnable(GL_FOG);                   // フォグを有効にする*/
	

}

//更新
void GamePlayScene::update(float delta_time) {
	/*/エンターキーでシーン終了としておく
	if (gsGetKeyTrigger(GKEY_RETURN)) {
		is_end_ = true;
	}*/
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
	gsDeleteMesh(Mesh_Ball);
	gsDeleteSkinMesh(Mesh_EnemyBird);
	//gsDeleteMesh(Mesh_PlayerBall);
	//オクトリーの削除
	gsDeleteOctree(Octree_Stage);
	gsDeleteOctree(Octree_Collider);

	// シャドウマップを削除
	gsDeleteShadowMap();

	// ライトマップの削除
	gsDeleteLightmap(LightMap);
	//リフレクションプローブの削除
	gsDeleteReflectionProbe(ReflectionProbe);

	gsDeleteTexture(Texture_Wire);
}