#ifndef ASSETS_H_
#define ASSETS_H_

//メッシュデータ
enum {
	Mesh_Player,		//自機
	Mesh_Skybox,			//スカイボックス
	Mesh_Ball,
	Mesh_EnemyBird,
	Mesh_PlayerBall,
};

//テクスチャ
enum {
	Texture_Skybox,
	Texture_Wire,
};

//オクトリー
enum {
	Octree_Stage,		//ステージ用オクトリー
	Octree_Collider		//衝突判定用オクトリー
};

//ライト
enum
{
	LightMap = 0,
	ReflectionProbe
};

#endif