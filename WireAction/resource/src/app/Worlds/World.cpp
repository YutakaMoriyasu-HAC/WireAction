#include "app/Worlds/World.h"
#include "app/Field/Field.h"
#include "app/Actors/Actor/Actor.h"
#include <GSeffect.h>

//�f�X�g���N�^
World::~World() {
	clear();
}

//�X�V
void World::update(float delta_time) {
	

	//�t�B�[���h�̍X�V
	field_->update(delta_time);
	//�A�N�^�[�̍X�V
	actors_.update(delta_time);
	//�A�N�^�[�̏Փ�
	actors_.collide();
	//�A�N�^�[�̒x���X�V
	actors_.late_update(delta_time);
	//�A�N�^�[�̏���
	actors_.remove();
	//�J�����̍X�V
	camera_->update(delta_time);
	//���C�g�̍X�V
	light_->update(delta_time);
	// �G�t�F�N�g��update
	effectManager_.update(delta_time);
	// �G�t�F�N�g�̍X�V����
	gsUpdateEffect(delta_time);
}

//�`��
void World::draw() const {
	
	//�J�����̐ݒ�
	camera_->draw();
	//�G�t�F�N�g�p�̃J������ݒ�
	gsSetEffectCamera();
	//���C�g�̐ݒ�
	light_->draw();

	// �V���h�E�}�b�v�̕`��
	gsDrawShadowMap(World::shadowMapCallback, (void*)this);

	//�t�B�[���h�̕`��
	field_->draw();
	//�A�N�^�[�̕`��
	actors_.draw();
	//�������A�N�^�[�̕`��
	actors_.draw_transparent();
	// �G�t�F�N�g��draw
	effectManager_.draw();
	// �G�t�F�N�g�̕`��
	gsDrawEffect();
	//GUI�̕`��
	actors_.draw_gui();
}

//����
void World::clear() {
	//�A�N�^�[�̏���
	actors_.clear();
	// �J����������
	camera_.reset();
	// ���C�g������
	light_.reset();
	//�G�t�F�N�g�̃N���A
	effectManager_.clear();
}

//�J�����̒ǉ�
void World::add_camera(std::shared_ptr<Actor> camera) {
	camera_ = camera;
}

//���C�g�̒ǉ�
void World::add_light(std::shared_ptr<Actor> light) {
	light_ = light;
}

//�t�B�[���h�̒ǉ�
void World::add_field(std::shared_ptr<Field> field) {
	field_ = field;
}

// �V���h�E�}�b�v�̕`��p�̊֐�
void World::shadowMapCallback(void* param, const GSmatrix4*, const GSmatrix4*)
{
	World* self = (World*)param;
	// �V���h�E�}�b�v�ɃA�N�^�[��`��
	self->actors_.draw();

}

//�A�N�^�[�̒ǉ�
void World::add_actor(std::shared_ptr<Actor> actor) {
	actors_.add(actor);
}

//�A�N�^�[�̌���
std::shared_ptr<Actor> World::find_actor(const std::string& name)const {
	return actors_.find(name);
}

//�w�肵���^�O�������A�N�^�[�̌���
std::vector<std::shared_ptr<Actor>>World::find_actor_with_tag(const std::string& tag) const {
	return actors_.find_with_tag(tag);
}

//�A�N�^�[����Ԃ�
int World::count_actor() const {
	return actors_.count();
}

//�w�肵���^�O�������A�N�^�[����Ԃ�
int World::count_actor_with_tag(const std::string& tag) const {
	return actors_.count_with_tag(tag);
}

//���b�Z�[�W���M
void World::send_message(const std::string& message, void* param) {
	actors_.send_message(message, param);
}

//�J�����̎擾
std::shared_ptr<Actor> World::camera() {
	return camera_;
}

//���C�g�̎擾
std::shared_ptr<Actor> World::light() {
	return light_;
}

//�t�B�[���h�̎擾
std::shared_ptr<Field> World::field() {
	return field_;
}

//�G�t�F�N�g�̒ǉ�
Effect* World::addEffect(std::shared_ptr<Effect> effect)
{
	effectManager_.add(effect);
	return effect.get();
}

//�G�t�F�N�g�̌���
std::shared_ptr<Effect> World::findEffect(const std::string& name)
{
	return effectManager_.findEffect(name);
}

//�G�t�F�N�g�̃N���A
void World::EffectClear()
{
	effectManager_.clear();
}