#ifndef ANIMATED_MESH_H_
#define ANIMATED_MESH_H_

#include <gslib.h>
#include <vector>
#include<functional>
#include<memory>

//�A�j���[�V�����C�x���g�\����
//�A�j���[�V�����C�x���g�i�A�j���[�V�����̎w�肳�ꂽ�^�C�~���O�ŏ��������s�j�Ɋւ������ێ�����
struct AnimationEvent
{
	//�R���X�g���N�^
	AnimationEvent(GSuint motion, GSfloat time, std::function<void()> callback) :
		motion_{ motion },
		time_{ time },
		callback_{ callback }
	{
	}
	//�C�x���g�𔭐������郂�[�V�����ԍ�
	GSuint motion_;
	//�C�x���g�𔭐�������^�C�~���O
	GSfloat time_;
	//�C�x���g�������̃R�[���o�b�N
	std::function<void()>callback_;
};


//�A�j���[�V�����t�����b�V���N���X
class AnimatedMesh {

public:
	//�R���X�g���N�^
	AnimatedMesh(GSuint id, GSuint motion = 0, bool loop = true, GSuint num_bones = 256);
	//�X�V
	void update(float delta_time);
	//�`��
	void draw() const;
	//���[�V�����̕ύX
	void change_motion(GSuint motion, bool loop = true);
	void change_motionS(GSuint motion, bool loop = true, float speed = 1.0f, float lerp = 0.5f, float startTime = 0.0f,float endTime=0.0f);
	//�ϊ��s���ݒ肷��
	void transform(const GSmatrix4& matrix);
	//���[�V�������I�����Ă��邩�H
	bool is_end_motion() const;
	//���݂̃��[�V�����̍Đ����Ԃ��擾
	float motion_time() const;
	//���݂̃��[�V�����̍Đ����Ԃ�ݒ�
	void motion_time(float time);
	//���[�V�����̏I�����Ԃ��擾
	float motionEndTime() const;
	//�{�[���̃��[���h�ϊ��s����擾
	GSmatrix4 bone_matrices(int bone_no) const;

	const float DefaultLerpTime{ 0.5f };

	void changeModel(GSuint model);

private:
	//�A�Z�b�gID
	GSuint		id_;
	//���[�V�����ԍ�
	GSuint		motion_;
	//���[�V�����^�C�}
	GSfloat		motion_timer_;
	//���[�V�������[�v�t���O
	bool		motion_loop_;
	//���[���h�ϊ��s��
	GSmatrix4	transform_;
	//�O��Đ��������[�V�����ԍ�
	GSuint		prev_motion_;
	//�O��Đ������ŏI�A�j���[�V�����^�C�}
	GSfloat		prev_motion_timer_;
	//��ԃ^�C�}
	GSfloat		lerp_timer_;
	//���x
	GSfloat		motion_speed_;

	GSfloat endTimePlus_;

	//�{�[���̃��[�J���p�ϊ��s��
	std::vector<GSmatrix4> local_bone_matrices_;
	//�{�[���̕ϊ��s��
	std::vector<GSmatrix4> bone_matrices_;
	//�����̃A�j���[�V�����C�x���g���i�[���邽�߂�vector
	std::vector<std::unique_ptr<AnimationEvent>>events_;
};

#endif