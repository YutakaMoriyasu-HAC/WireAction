#include "app/AnimatedMesh/AnimatedMesh.h"
#include <cmath>
#include <algorithm>

//��ԃt���[����
const GSfloat LerpTime{ 10.0f };

//�R���X�g���N�^
AnimatedMesh::AnimatedMesh(GSuint id, GSuint motion, bool loop, GSuint num_bones) :
	id_{ id },
	motion_{ motion },
	motion_timer_{ 0.0f },
	motion_loop_{ loop },
	transform_{ GS_MATRIX4_IDENTITY },
	prev_motion_{ motion },
	prev_motion_timer_{ 0.0f },
	lerp_timer_{ 0.0f },
	local_bone_matrices_{ num_bones },
	bone_matrices_{ num_bones },
	motion_speed_{ 1.0f } {
}


//�X�V
void AnimatedMesh::update(float delta_time) {
	

	//�X�V�O�̃^�C�}�[�l������Ă���
	GSfloat prev_timer = motion_timer_;
	// �A�j���[�V�����^�C�}�̍X�V
	motion_timer_ += delta_time * motion_speed_;
	// ���[�v�A�j���[�V�������H
	if (motion_loop_)
	{
		// ���[�V�����^�C�}�����[�v������           �I�����Ԃɉ�����
		motion_timer_ = std::fmod(motion_timer_, motionEndTime() * motion_speed_);
	}
	else
	{
		// ���[�V�����^�C�}���N�����v����
		motion_timer_ = std::min(motion_timer_, motionEndTime() - 1.0f);
	}
	// ��ԃ^�C�}�̍X�V(LerpTime�ȏ�ɂȂ�Ȃ��悤�ɃN�����v����j
	lerp_timer_ = std::min(lerp_timer_ + delta_time, LerpTime);
	// ���[�V���������[�v�������ǂ����B
	   // ���݂̎��Ԃ�1�t���[���O�̎��Ԃ�菬�����ꍇ�́A���[�v�����Ɣ��f�ł���B
	bool looped = motion_timer_ < prev_timer;
	// �S�C�x���g���`�F�b�N���A�K�v�ł���΃C�x���g�𔭍s����
	for (const auto& event : events_)
	{
		// ���݂̃��[�V�������C�x���g�Ώۂ̃��[�V�����łȂ���΁A�������Ȃ�
		if (event->motion_ != motion_) continue;

		if (looped)
		{
			if (prev_timer < event->time_ || event->time_ <= motion_timer_)
			{
				event->callback_(); // �R�[���o�b�N�������Ăяo��
			}
		}
		else
		{
			if (prev_timer < event->time_ && event->time_ <= motion_timer_)
			{
				event->callback_(); // �R�[���o�b�N�������Ăяo��
			}
		}
	}


	if (is_end_motion())
	{
		motion_speed_ = 1.0f;
	}
}

//�`��
void AnimatedMesh::draw() const {
	//�X�P���g���̎����v�Z�𖳌��ɂ���
	gsDisable(GS_CALC_SKELETON);
	//�X�P���g���̃o�C���h
	gsBindSkeleton(id_);
	//�{�[���̕ϊ��s���ݒ�
	gsSetMatrixSkeleton(bone_matrices_.data());

	//���b�V���̕`��
	glPushMatrix();
		glMultMatrixf(transform_);
		gsDrawMesh(id_);
	glPopMatrix();

	//�X�P���g���̎����v�Z��L���ɂ���
	gsEnable(GS_CALC_SKELETON);
}

//���[�V�����̕ύX
void AnimatedMesh::change_motion(GSuint motion, bool loop) {
	//���݂Ɠ������[�V�����̏ꍇ�͉������Ȃ�
	if (motion_ == motion) return;
	//��Ԓ�(�O��)�́A�O��̃��[�V�������X�V���Ȃ��悤�ɂ���
	if (lerp_timer_ > (LerpTime * 0.5f)) {
		//�O��̃��[�V�����ԍ��ƃ��[�V�������Ԃ�ۑ�
		prev_motion_ = motion_;
		prev_motion_timer_ = motion_timer_;
		//��Ԓ��^�C�}�̃��Z�b�g
		lerp_timer_ = 0.0f;
	}
	//���[�V�����̍X�V
	motion_ = motion;
	//���[�V�����^�C�}�̏�����
	motion_timer_ = 0.0f;
	//���[�V�������[�v�t���O�̐ݒ�
	motion_loop_ = loop;
}

//���[�V�����̕ύX
void AnimatedMesh::change_motionS(GSuint motion, bool loop, float speed, float lerp, float startTime) {
	//���݂Ɠ������[�V�����̏ꍇ�͉������Ȃ�
	if (motion_ == motion) return;
	//��Ԓ�(�O��)�́A�O��̃��[�V�������X�V���Ȃ��悤�ɂ���
	if (lerp_timer_ > (LerpTime * lerp)) {
		//�O��̃��[�V�����ԍ��ƃ��[�V�������Ԃ�ۑ�
		prev_motion_ = motion_;
		prev_motion_timer_ = motion_timer_;
		//��Ԓ��^�C�}�̃��Z�b�g
		lerp_timer_ = 0.0f;
	}
	//���[�V�����̍X�V
	motion_ = motion;
	//���[�V�����^�C�}�̏�����
	motion_timer_ = startTime;
	//���[�V�������[�v�t���O�̐ݒ�
	motion_loop_ = loop;

	motion_speed_ = speed;
}

//���W�ϊ����s��
void AnimatedMesh::transform(const GSmatrix4& matrix) {
	transform_ = matrix;
	//�{�[�����Ƃ̃��[�J���ϊ��s����v�Z
	gsCalculateAnimationLerp(
		id_, prev_motion_, prev_motion_timer_,	//�O��̃A�j���[�V����
		id_, motion_, motion_timer_,			//���݂̃A�j���[�V����
		lerp_timer_ / LerpTime,
		local_bone_matrices_.data()
	);

	//�{�[���̕ϊ��s����v�Z
	gsBindSkeleton(id_);
	gsCalculateSkeleton(NULL, local_bone_matrices_.data(), bone_matrices_.data());
}

//���[�V�������I�����Ă��邩�H
bool AnimatedMesh::is_end_motion() const {
	//���[�v�̃��[�V�����͏I�����Ȃ�
	if (motion_loop_)return false;
	//�I�����Ă��邩�H
	return motion_timer_ >= (motionEndTime() - 1.0f);
}

//���݂̃��[�V�����̍Đ����Ԃ��擾
float AnimatedMesh::motion_time() const {
	return motion_timer_;
}

//���[�V�����̍Đ����Ԃ�ݒ�
void AnimatedMesh::motion_time(float time) {
	motion_timer_ = time;
}

//���[�V�����̏I�����Ԃ��擾
float AnimatedMesh::motionEndTime() const {
	return gsGetEndAnimationTime(id_, motion_);
}

//�{�[���̃��[���h�ϊ��s����擾
GSmatrix4 AnimatedMesh::bone_matrices(int bone_no)const {
	return bone_matrices_[bone_no] * transform_;
}

//���f����ύX
void AnimatedMesh::changeModel(GSuint model) {
	id_ = model;
}