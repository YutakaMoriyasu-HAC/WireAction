#ifndef BOUNDING_SPHERE_H_
#define BOUNDING_SPHERE_H_

#include<gslib.h>

//���E���N���X
class BoundingSphere {
public:
	//�R���X�g���N�^
	BoundingSphere(float radius = 0.0f, const GSvector3& center = GSvector3{ 0.0f,0.0f,0.0f });
	//���s�ړ�
	BoundingSphere translate(const GSvector3& position) const;
	//���W�ϊ�
	BoundingSphere transform(const GSmatrix4& matrix) const;
	//�d�Ȃ��Ă邩
	bool intersects(const BoundingSphere& other) const;
	//�f�o�b�O�\��
	void draw() const;
	// ���肪�L����
	const bool enable() const;
	// ���肪�L�����̕ύX
	void enable(const bool flag);
	// ���a���擾
	const float radius() const;
	void setRadius(float radius);
	// ���S���W
	const GSvector3& center() const;
	void setCenter(const GSvector3& center);
	// �f�o�b�N�t���O��ݒ�
	void setDebugFrag(bool frag);

public:
	//���a
	float		radius_{ 0.0f };
	//���S���W
	GSvector3	center_{ 0.0f,0.0f,0.0f };
	// �t���O
	bool		enable_;
	// �f�o�b�N�t���O
	bool		debugFrag_;
};

#endif
