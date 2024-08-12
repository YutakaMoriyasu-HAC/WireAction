#include "app/Worlds/World.h"
#include "app/Field/Field.h"
#include "app/Actors/Actor/Actor.h"

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
}

//�`��
void World::draw() const {
	//�J�����̐ݒ�
	camera_->draw();
	//���C�g�̐ݒ�
	light_->draw();
	//�t�B�[���h�̕`��
	field_->draw();
	//�A�N�^�[�̕`��
	actors_.draw();
	//�������A�N�^�[�̕`��
	actors_.draw_transparent();
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