#ifndef ASSETS_H_
#define ASSETS_H_

//���b�V���f�[�^
enum {
	Mesh_Player = 100,		//���@
	Mesh_Skybox,			//�X�J�C�{�b�N�X
	Mesh_Ball,
	Mesh_EnemyBird,
	Mesh_PlayerBall,
	Mesh_Coin,
};

//�e�N�X�`��
enum {
	Texture_Skybox = 200,
	Texture_Wire,
	Texture_Coin,
};

//�I�N�g���[
enum {
	Octree_Stage = 300,		//�X�e�[�W�p�I�N�g���[
	Octree_Collider,		//�Փ˔���p�I�N�g���[
};

//�G�t�F�N�g
enum {
	Effect_GetCoin = 400	//�R�C������
};

//���C�g
enum
{
	LightMap = 0,
	ReflectionProbe
};

#endif