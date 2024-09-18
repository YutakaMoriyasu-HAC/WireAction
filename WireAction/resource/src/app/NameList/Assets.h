#ifndef ASSETS_H_
#define ASSETS_H_

//メッシュデータ
enum {
	Mesh_Player = 100,		//自機
	Mesh_Skybox,			//スカイボックス
	Mesh_Ball,
	Mesh_EnemyBird,
	Mesh_PlayerBall,
	Mesh_Coin,
};

//テクスチャ
enum {
	Texture_Skybox = 200,
	Texture_Wire,
	Texture_Coin,
};

//オクトリー
enum {
	Octree_Stage = 300,		//ステージ用オクトリー
	Octree_Collider,		//衝突判定用オクトリー
};

//エフェクト
enum {
	Effect_GetCoin = 400	//コイン入手
};

//ライト
enum
{
	LightMap = 0,
	ReflectionProbe
};

#endif