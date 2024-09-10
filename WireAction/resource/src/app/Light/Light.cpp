#include "Light.h"

//�R���X�g���N�^
Light::Light(IWorld* world) {
	world_ = world;
	name_ = "Light";
	tag_ = "LightTag";
}

//�`��
void Light::draw() const {
	static float ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	static float diffuse[] = { 1.0f, 0.8f, 0.3f, 1.0f };
	static float specular[] = { 1.0f, 0.8f, 0.3f, 1.0f };
	static float position[] = { -10.0f,20.0f,10.0f,0.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	// �V���h�E�}�b�v�p�̃��C�g�̈ʒu��ݒ�
	GSvector3 shadow_map_light_position{ position[0], position[1], position[2] };
	gsSetShadowMapLightPosition(&shadow_map_light_position);
}