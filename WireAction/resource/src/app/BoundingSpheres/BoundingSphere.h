#ifndef BOUNDING_SPHERE_H_
#define BOUNDING_SPHERE_H_

#include<gslib.h>

//境界球クラス
class BoundingSphere {
public:
	//コンストラクタ
	BoundingSphere(float radius = 0.0f, const GSvector3& center = GSvector3{ 0.0f,0.0f,0.0f });
	//平行移動
	BoundingSphere translate(const GSvector3& position) const;
	//座標変換
	BoundingSphere transform(const GSmatrix4& matrix) const;
	//重なってるか
	bool intersects(const BoundingSphere& other) const;
	//デバッグ表示
	void draw() const;
	// 判定が有効か
	const bool enable() const;
	// 判定が有効かの変更
	void enable(const bool flag);
	// 半径を取得
	const float radius() const;
	void setRadius(float radius);
	// 中心座標
	const GSvector3& center() const;
	void setCenter(const GSvector3& center);
	// デバックフラグを設定
	void setDebugFrag(bool frag);

public:
	//半径
	float		radius_{ 0.0f };
	//中心座標
	GSvector3	center_{ 0.0f,0.0f,0.0f };
	// フラグ
	bool		enable_;
	// デバックフラグ
	bool		debugFrag_;
};

#endif
