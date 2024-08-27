#include "app/BoundingSpheres/BoundingSphere.h"


//コンストラクタ
BoundingSphere::BoundingSphere(float radius,const GSvector3& center) :
	radius_{ radius }, center_{ center }, enable_{ true } {
    debugFrag_ = true;
}

// 平行移動
BoundingSphere BoundingSphere::translate(const GSvector3& position) const {
    return BoundingSphere{ radius_, center_ + position };
}

// 座標返還
BoundingSphere BoundingSphere::transform(const GSmatrix4& matrix) const {
    return BoundingSphere{ radius_ * matrix.getScale().y, matrix.transform(center_) };
}

// 交差しているか？
bool BoundingSphere::intersects(const BoundingSphere& other) const {
    return gsCollisionSphereAndSphere(
        &center_, radius_, &other.center_, other.radius_) == GS_TRUE;
}

// 交差しているか？
bool BoundingSphere::intersects(const Line& other, GSvector3* intersect) const {
    return gsCollisionSphereAndLine(
        &center_, radius_, &other.start, &other.end, intersect) == GS_TRUE;
}

// 判定が有効か
const bool BoundingSphere::enable() const {
    return enable_;
}

// 判定が有効かの変更
void BoundingSphere::enable(const bool flag) {
    enable_ = flag;
}


// デバッグ表示
void BoundingSphere::draw() const {
#ifdef _DEBUG
    glPushMatrix();
    glTranslatef(center_.x, center_.y, center_.z);
    glutWireSphere(radius_, 16, 16);  // ワイヤーフレームの球体を描画
    glPopMatrix();
#endif
    
}

// 半径を取得
const float BoundingSphere::radius() const
{
    return radius_;
}

void BoundingSphere::setRadius(float radius)
{
    radius_ = radius;
}

// 中心座標
const GSvector3& BoundingSphere::center() const
{
    return center_;
}

void BoundingSphere::setCenter(const GSvector3& center)
{
    center_ = center;
}

// デバックフラグを設定
void BoundingSphere::setDebugFrag(bool frag) {
    debugFrag_ = frag;
}