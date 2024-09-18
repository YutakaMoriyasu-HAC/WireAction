#ifndef WORLD_H_
#define WORLD_H_

#include "IWorld.h"
#include "app/Actors/ActorManager/ActorManager.h"
#include "app/Effect/EffectManager.h"
#include <gslib.h>

//���[���h�N���X
class World : public IWorld {
public:
	//�R���X�g���N�^
	World() = default;
	//�f�X�g���N�^
	~World();
	//�X�V
	void update(float delta_time);
	//�`��
	void draw() const;
	//����
	void clear();
	//�J�����̒ǉ�
	void add_camera(std::shared_ptr<Actor> camera);
	//���C�g�̒ǉ�
	void add_light(std::shared_ptr<Actor> light);
	//�t�B�[���h�̒ǉ�
	void add_field(std::shared_ptr<Field> field);
	// �V���h�E�}�b�v�̕`��p�̊֐�
	static void shadowMapCallback(void* param, const GSmatrix4*, const GSmatrix4*);

	//�A�N�^�[��ǉ�
	virtual void add_actor(std::shared_ptr<Actor> actor) override;
	//�A�N�^�[�̌���
	virtual std::shared_ptr<Actor> find_actor(const std::string& name) const override;
	//�w�肵���^�O�������A�N�^�[�̌���
	virtual std::vector<std::shared_ptr<Actor>> find_actor_with_tag(const std::string& tag) const override;
	//�A�N�^�[����Ԃ�
	virtual int count_actor() const override;
	//�w�肵���^�O�������A�N�^�[����Ԃ�
	virtual int count_actor_with_tag(const std::string& tag) const override;
	//���b�Z�[�W�̑��M
	virtual void send_message(const std::string& message, void* param = nullptr) override;

	//�J�����̎擾
	virtual std::shared_ptr<Actor> camera() override;
	//���C�g�̎擾
	virtual std::shared_ptr<Actor> light() override;
	//�t�B�[���h�̎擾
	virtual std::shared_ptr<Field> field() override;

	// Effect�̒ǉ�
	Effect* addEffect(std::shared_ptr<Effect> effect) override;
	// Effect�̌���
	std::shared_ptr<Effect> findEffect(const std::string& name) override;
	//�G�t�F�N�g�̃N���A
	virtual void EffectClear()override;

	//�R�s�[�֎~
	World(const World& other) = delete;
	World& operator = (const World& other) = delete;

private:
	//�A�N�^�[�}�l�[�W���[
	ActorManager actors_;
	// Effect�}�l�[�W���[
	EffectManager effectManager_;
	//���C�g
	std::shared_ptr<Actor>		 light_{ nullptr };
	//�J����
	std::shared_ptr<Actor>		 camera_{ nullptr };
	//�t�B�[���h
	std::shared_ptr<Field>		 field_{ nullptr };
};

#endif