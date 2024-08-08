#ifndef FIELD_FIELD_H_
#define FIELD_FIELD_H_

#include <gslib.h>
#include"app/Actors/ActorManager/ActorManager.h"
#include"app/Actors/Actor/Actor.h"

// �O���錾
class Ray;
class Line;
class BoundingSphere;

//�t�B�[���h�N���X
class Field {
public:
	//�R���X�g���N�^
	Field(GSuint octree, GSuint collider, GSuint skybox);

	//�X�V
	void update(float delta_time);
	//�`��
	void draw() const;

	//�����Ƃ̏Փ˔���
	bool collide(const Line& line,
		GSvector3* intersect = nullptr, GSplane* plane = nullptr) const;
	//���C�Ƃ̏Փ˔���
	bool collide(const Ray& ray, float max_distance,
		GSvector3* intersect = nullptr, GSplane* plane = nullptr) const;
	//���̂Ƃ̏Փ˔���
	bool collide(const BoundingSphere& sphere, GSvector3* center = nullptr) const;

	//�R�s�[�֎~
	Field(const Field& other) = delete;

private:
	//�`��p�I�N�g���[
	GSuint	octree_;
	//�Փ˔���p�I�N�g���[
	GSuint	collider_;
	//�X�J�C�{�b�N�X
	GSuint	skybox_;
};

#endif