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

//�J�n
void GamePlayScene::start() {

	//�f�t�H���g�V�F�[�_�[�̏�����
	gsInitDefaultShader();

	//�I���t���O��������
	is_end_ = false;

	//�v���C���[�̃��b�V���̓ǂݍ���
	gsLoadSkinMesh(Mesh_Player, "resource/Assets/PlayerDog/player.msh");
	//gsLoadMesh(Mesh_PlayerBall, "resource/Assets/PlayerDog/playerBall.msh");
	gsLoadMesh(Mesh_Ball, "resource/Assets/Ball/Ball.msh");
	gsLoadSkinMesh(Mesh_EnemyBird, "resource/Assets/Enemy/EnemyBird/EnemyBird.mshb");

	//�X�J�C�h�[���p�̃��b�V����ǂݍ���
	gsLoadMeshFixed(Mesh_Skybox, "resource/Assets/Skybox/skydome.msh");
	//�`��p�̃I�N�g���[��ǂݍ���
	gsLoadOctree(Octree_Stage, "resource/Assets/TestStage/TestStageOc.oct");
	//�Փ˔���p�̃I�N�g���[��ǂݍ���
	gsLoadOctree(Octree_Collider, "resource/Assets/TestStage/TestStageCol.oct");

	// ���C�g�}�b�v�̓ǂݍ���
	gsLoadLightmap(LightMap, "resource/Assets/TestStage/LightMap/Lightmap.txt");
	// ���t���N�V�����v���[�u�̓ǂݍ���
	gsLoadReflectionProbe(ReflectionProbe, "resource/Assets/TestStage/ReflectionProbe.txt");

	gsLoadTexture(Texture_Wire, "resource/Assets/Ball/wire.png");
	

	//�t�B�[���h�N���X�̒ǉ�
	world_.add_field(std::make_shared<Field>(Octree_Stage, Octree_Collider, Mesh_Skybox ));

	//�v���C���[�̒ǉ�
	world_.add_actor(std::make_shared<Player>(&world_, GSvector3(0.0f, 0.125f, 0.0f)));

	//ball�I�u�W�F�N�g��z�u(��)
	world_.add_actor(std::make_shared<Ball>(&world_, GSvector3(0.0f, 6.0f, 0.0f)));
	world_.add_actor(std::make_shared<Ball>(&world_, GSvector3(-6.0f, 6.0f, -6.0f)));
	world_.add_actor(std::make_shared<Ball>(&world_, GSvector3(10.0f, 12.0f, -7.0f)));

	//�G
	world_.add_actor(std::make_shared<Enemy>(&world_, GSvector3(11.0f, 0.5f, 10.0f)));
	world_.add_actor(std::make_shared<Enemy>(&world_, GSvector3(8.0f, 0.5f, 10.0f)));
	world_.add_actor(std::make_shared<Enemy>(&world_, GSvector3(5.0f, 0.5f, 10.0f)));
	world_.add_actor(std::make_shared<Enemy>(&world_, GSvector3(2.0f, 0.5f, 10.0f)));
	world_.add_actor(std::make_shared<Enemy>(&world_, GSvector3(-1.0f, 0.5f, 10.0f)));
	world_.add_actor(std::make_shared<Enemy>(&world_, GSvector3(-4.0f, 0.5f, 10.0f)));

	//�J�����N���X�̒ǉ�
	world_.add_camera(std::make_shared<CameraTps>(&world_, GSvector3(8.0f, 3.0f, 0.0f), GSvector3(-5.0f, 0.0f, 0.0f)));
	//���C�g�N���X�̒ǉ�
	world_.add_light(std::make_shared<Light>(&world_));

	//������J�����O��L���ɂ���
	gsEnable(GS_FRUSTUM_CULLING);

	// �V���h�E�}�b�v�̍쐬
	static const GSuint shadow_map_size[] = { 2048, 2048 };
	gsCreateShadowMap(2, shadow_map_size, GS_TRUE);
	// �V���h�E�}�b�v��K�p���鋗��(���_����̋����j
	gsSetShadowMapDistance(60.0f);
	// �J�X�P�[�h�V���h�E�}�b�v�̕����ʒu�𒲐��i�f�t�H���g��0.5�j
	gsSetShadowMapCascadeLamda(0.7f);
	// �V���h�E�̔Z����ݒ�(0.0:�Z���`1.0:����)
	gsSetShadowMapAttenuation(0.0f);

	

	/*/ �t�H�O�̐ݒ�
	const static float fog_color[4]{ 1.0f, 0.723f, 0.216f, 1.0f };
	const static float fog_start{ 10.0f };
	const static float fog_end{ 300.0f };
	glFogi(GL_FOG_MODE, GL_LINEAR);     // ���`�t�H�O
	glFogfv(GL_FOG_COLOR, fog_color);   // �t�H�O�̐F
	glFogf(GL_FOG_START, fog_start);    // �t�H�O�̊J�n�ʒu�i���_����̋����j
	glFogf(GL_FOG_END, fog_end);        // �t�H�O�̏I���ʒu�i���_����̋����j
	glEnable(GL_FOG);                   // �t�H�O��L���ɂ���*/
	

}

//�X�V
void GamePlayScene::update(float delta_time) {
	/*/�G���^�[�L�[�ŃV�[���I���Ƃ��Ă���
	if (gsGetKeyTrigger(GKEY_RETURN)) {
		is_end_ = true;
	}*/
	//���[���h�̍X�V
	world_.update(delta_time);
}

//�`��
void GamePlayScene::draw() const {
	//���[���h�̕`��
	world_.draw();
}

//�I�����Ă��邩�H
bool GamePlayScene::is_end() const {
	return is_end_;		//�I���t���O��Ԃ�
}

//���̃V�[����Ԃ�
std::string GamePlayScene::next() const {
	return "TitleScene";
}

//�I��
void GamePlayScene::end() {
	//���[���h������
	world_.clear();
	//���b�V���̍폜
	gsDeleteSkinMesh(Mesh_Player);
	gsDeleteMesh(Mesh_Skybox);
	gsDeleteMesh(Mesh_Ball);
	gsDeleteSkinMesh(Mesh_EnemyBird);
	//gsDeleteMesh(Mesh_PlayerBall);
	//�I�N�g���[�̍폜
	gsDeleteOctree(Octree_Stage);
	gsDeleteOctree(Octree_Collider);

	// �V���h�E�}�b�v���폜
	gsDeleteShadowMap();

	// ���C�g�}�b�v�̍폜
	gsDeleteLightmap(LightMap);
	//���t���N�V�����v���[�u�̍폜
	gsDeleteReflectionProbe(ReflectionProbe);

	gsDeleteTexture(Texture_Wire);
}