#ifndef ASSETS_H_
#define ASSETS_H_

//メッシュデータ
enum {
	Mesh_Player,		//自機
	Mesh_Skybox			//スカイボックス
};

//テクスチャ
enum {
	Texture_Skybox
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