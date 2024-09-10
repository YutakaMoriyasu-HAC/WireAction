#ifndef ACTOR_H_
#define ACTOR_H_

#include <gslib.h>
#include <GStransform.h>
#include <string>
#include "app/BoundingSpheres/BoundingSphere.h"
#include "app/Tween/Tween.h"
#include "app/Ray/Line.h"

class IWorld;	//���[���h���ۃC���^�[�t�F�[�X�̑O���錾

//�A�N�^�[�N���X
class Actor {
public:
	//�R���X�g���N�^
	Actor() = default;
	//���z�f�X�g���N�^
	virtual ~Actor() = default;
	//�X�V
	virtual void update(float delta_time);
	//�x���X�V
	virtual void lateUpdate(float delta_time);
	//�`��
	virtual void draw() const;
	//�������̕`��
	virtual void draw_transparent() const;
	//GUI�̕`��
	virtual void draw_gui() const;
	//�Փ˃��A�N�V����
	virtual void react(Actor& other);
	//���b�Z�[�W����
	virtual void handle_message(const std::string& message, void* param);
	//�Փ˔���
	void collide(Actor& other);
	//�����Ƃ̏Փ˔���
	bool collide(const Line& line, GSvector3* intersect) const;

	//�A�N�^�[�Ƃ̏Փˏ���
	void collide_actor(Actor& other);


	//���S����
	void die();
	//�Փ˂��Ă��邩�H
	bool is_collide(const Actor& other) const;
	//���S���Ă��邩�H
	bool is_dead() const;
	//���O���擾
	const std::string& name() const;
	//�^�O�����擾
	const std::string& tag() const;
	//�U����Ԃ��擾
	const bool& attackState() const;
	//�g�����X�t�H�[�����擾(const��)
	const GStransform& transform() const;
	//�g�����X�t�H�[�����擾
	GStransform& transform();
	//�ړ��ʂ��擾
	const GSvector3& velocity() const;
	GSvector3& velocity();
	//�ړ��ʂ�ݒ�
	void velocity(const GSvector3& vec);
	//�Փ˔���f�[�^���擾
	BoundingSphere collider() const;
	// �w�肳�ꂽ�ꏊ�܂�Tween�ňړ�����
	TweenUnit& move_to(const GSvector3& to, float duration);
	//���R����
	void gravityFall(float delta_time);

	
	//�R�s�[�֎~
	Actor(const Actor& other) = default;
	Actor& operator = (const Actor& other) = delete;

protected:
	//���[���h
	IWorld*			world_{ nullptr };
	//�^�O��
	std::string		tag_;
	// �e�A���L�҃^�O
	std::string ownerTag_;
	//���O
	std::string		name_;
	//�U�����
	bool		attackState_{false};
	//�g�����X�t�H�[��
	GStransform		transform_;
	//�ړ���
	GSvector3		velocity_{ 0.0f,0.0f,0.0f };
	//�Փ˔��肪�L����?
	bool			enable_collider_{ true };
	//�Փ˔���
	BoundingSphere	collider_;
	//���S�t���O
	bool			dead_{ false };

	//�d�͒l
	const float Gravity{ -0.016f }; //-0.016 //-0.008
};

#endif