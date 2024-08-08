#include "Field.h"
#include "app/Ray/Ray.h"
#include "app/Ray/Line.h"
#include "app/BoundingSpheres/BoundingSphere.h"

//�R���X�g���N�^
Field::Field(GSuint octree, GSuint collider, GSuint skybox) :
	octree_{ octree }, collider_{ collider }, skybox_{ skybox } {
}

//�X�V
void Field::update(float delta_time) {}

//�`��
void Field::draw() const {
	//�X�J�C�{�b�N�X�̕`��
	gsDrawSkyboxCubemap(skybox_);
	//�I�N�g���[�̕`��
	gsDrawOctree(octree_);
}

//�����Ƃ̏Փ˔���
bool Field::collide(const Line& line, GSvector3* intersect, GSplane* plane) const {
	return gsOctreeCollisionLine(
		gsGetOctree(collider_),
		&line.start, &line.end, intersect, plane) == GS_TRUE;
}

//���C�Ƃ̏Փ˔���
bool Field::collide(const Ray& ray, float max_distance,
	GSvector3* intersect, GSplane* plane) const {
	Line line{
		ray.position,
		ray.position + (ray.direction.normalized() * max_distance) };
	return collide(line, intersect, plane);
}

//���̂Ƃ̏Փ˔���
bool Field::collide(const BoundingSphere& sphere, GSvector3* center) const {
	return gsOctreeCollisionSphere(
		gsGetOctree(collider_),
		&sphere.center, sphere.radius, center) == GS_TRUE;
}