#include "app/BoundingSpheres/BoundingSphere.h"


//�R���X�g���N�^
BoundingSphere::BoundingSphere(float radius,const GSvector3& center) :
	radius_{ radius }, center_{ center }, enable_{ true } {
    debugFrag_ = true;
}

// ���s�ړ�
BoundingSphere BoundingSphere::translate(const GSvector3& position) const {
    return BoundingSphere{ radius_, center_ + position };
}

// ���W�Ԋ�
BoundingSphere BoundingSphere::transform(const GSmatrix4& matrix) const {
    return BoundingSphere{ radius_ * matrix.getScale().y, matrix.transform(center_) };
}

// �������Ă��邩�H
bool BoundingSphere::intersects(const BoundingSphere& other) const {
    return gsCollisionSphereAndSphere(
        &center_, radius_, &other.center_, other.radius_) == GS_TRUE;
}

// �������Ă��邩�H
bool BoundingSphere::intersects(const Line& other, GSvector3* intersect) const {
    return gsCollisionSphereAndLine(
        &center_, radius_, &other.start, &other.end, intersect) == GS_TRUE;
}

// ���肪�L����
const bool BoundingSphere::enable() const {
    return enable_;
}

// ���肪�L�����̕ύX
void BoundingSphere::enable(const bool flag) {
    enable_ = flag;
}


// �f�o�b�O�\��
void BoundingSphere::draw() const {
#ifdef _DEBUG
    glPushMatrix();
    glTranslatef(center_.x, center_.y, center_.z);
    glutWireSphere(radius_, 16, 16);  // ���C���[�t���[���̋��̂�`��
    glPopMatrix();
#endif
    
}

// ���a���擾
const float BoundingSphere::radius() const
{
    return radius_;
}

void BoundingSphere::setRadius(float radius)
{
    radius_ = radius;
}

// ���S���W
const GSvector3& BoundingSphere::center() const
{
    return center_;
}

void BoundingSphere::setCenter(const GSvector3& center)
{
    center_ = center;
}

// �f�o�b�N�t���O��ݒ�
void BoundingSphere::setDebugFrag(bool frag) {
    debugFrag_ = frag;
}