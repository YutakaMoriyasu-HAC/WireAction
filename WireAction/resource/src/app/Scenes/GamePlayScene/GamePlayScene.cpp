#include "GamePlayScene.h"
#include "app/Field/Field.h"
#include "app/Actors/CameraTps/CameraRotateAround.h"
#include "app/Light/Light.h"
#include "app/Actors/Player/Player.h"
#include "app/NameList/Assets.h"

//�J�n
void GamePlayScene::start() {
	//�I���t���O��������
	is_end_ = false;

	//�v���C���[�̃��b�V���̓ǂݍ���
	gsLoadSkinMesh(Mesh_Player, "resource/Assets/Player/player.msh");

	//�X�J�C�h�[���p�̃��b�V����ǂݍ���
	gsLoadMesh(Mesh_Skybox, "resource/Assets/Skybox/skydome.msh");
	//�`��p�̃I�N�g���[��ǂݍ���
	gsLoadOctree(Octree_Stage, "resource/Assets/Octree/stage.oct");
	//�Փ˔���p�̃I�N�g���[��ǂݍ���
	gsLoadOctree(Octree_Collider, "resource/Assets/Octree/stage_collider.oct");

	//�t�B�[���h�N���X�̒ǉ�
	world_.add_field(new Field{ Octree_Stage, Octree_Collider, Mesh_Skybox });
	//�J�����N���X�̒ǉ�
	world_.add_camera(new CameraRotateAround{ &world_, GSvector3{ 0.0f, 3.2f, -4.8f }, GSvector3{ 0.0f, 1.92f, 0.0f } });
	//���C�g�N���X�̒ǉ�
	world_.add_light(new Light{ &world_ });

	//�v���C���[�̒ǉ�
	world_.add_actor(new Player{ &world_, GSvector3{ 0.0f, 0.125f, 0.0f } });
	

}

//�X�V
void GamePlayScene::update(float delta_time) {
	//�G���^�[�L�[�ŃV�[���I���Ƃ��Ă���
	if (gsGetKeyTrigger(GKEY_RETURN)) {
		is_end_ = true;
	}
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
	//�I�N�g���[�̍폜
	gsDeleteOctree(Octree_Stage);
	gsDeleteOctree(Octree_Collider);
}