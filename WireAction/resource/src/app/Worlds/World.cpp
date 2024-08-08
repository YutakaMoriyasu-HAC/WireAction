#include "World.h"
#include "app/Actors/Actor/Actor.h"

#include<GSeffect.h>

World::World()
{
}

// �f�X�g���N�^
World::~World() {
    clear();
}

// �X�V
void World::update(float delta_time) {
    // �t�B�[���h�̍X�V
    //field_->update(delta_time);
    // �A�N�^�[�̍X�V
    actors_.update(delta_time);
    // �A�N�^�[�̏Փ�
    actors_.collide();
    //field()->collide(&actors_); 
    // �J�����̍X�V
    camera_->update(delta_time);
    // ���C�g�̍X�V
    //light_->update(delta_time);
    //�G�W�F�l���[�^�[�̍X�V
    //enemyGenerator_->update(delta_time);
    //gui�}�l�[�W���[�̍X�V
    //guiManager_.update(delta_time);
    // �G�t�F�N�g��update
    //effectManager_.update(delta_time);
    // �G�t�F�N�g�̍X�V����
    //gsUpdateEffect(delta_time);




    // �A�N�^�[�̏���
    actors_.remove();
}

// �`��
void World::draw() const {


}

void World::postDraw() const
{
    //�����_�[�^�[�Q�b�g�̐ݒ�
    //postEffectManager_.start();

    // �V�[���̕`��

    // �J�����̐ݒ�
    camera_->draw();
    // �G�t�F�N�g�p�̃J������ݒ�
    //gsSetEffectCamera();
    // �t�B�[���h�̕`��
    //field_->draw();
    // ���C�g�̐ݒ�
    //light_->draw();
    // �A�N�^�[�̕`��
    actors_.draw();
    // �G�t�F�N�g��draw
    //effectManager_.draw();
    // �G�t�F�N�g�̕`��
    //gsDrawEffect();
    // �����_�[�^�[�Q�b�g�̉���
    //postEffectManager_.end();




}

void World::OnDraw() const
{
    // �V�[���̕`��
// �J�����̐ݒ�
    camera_->draw();
    // �G�t�F�N�g�p�̃J������ݒ�
    //gsSetEffectCamera();


    // �t�B�[���h�̕`��
    //field_->draw();
    // ���C�g�̐ݒ�
    //light_->draw();
    // �A�N�^�[�̕`��
    actors_.draw();
    // �G�t�F�N�g��draw
    effectManager_.draw();
    // �G�t�F�N�g�̕`��
    gsDrawEffect();
}

// ����
void World::clear() {
    //�A�N�^�[�̏���
    actors_.clear();
    // �J����������
    camera_.reset();
    // ���C�g������
    light_.reset();
    //�G�t�F�N�g�̃N���A
    effectManager_.clear();
    //GUI�}�l�[�W���[�̏���
    //guiManager_.clear();

    isGameClear_ = false;
    isGameOver_ = false;
    isGameEnd_ = false;
}

// �J�����̒ǉ�
void World::addCamera(std::shared_ptr<Actor> camera) {
    camera_ = camera;
}

// ���C�g�̒ǉ�
void World::addLight(std::shared_ptr<Actor> light) {
    light_ = light;
}

void World::addField(std::shared_ptr<Field> field)
{
    field_ = field;
}




// �A�N�^�[�̒ǉ�
void World::addActor(std::shared_ptr<Actor> actor) {
    actors_.add(actor);
}

// �A�N�^�[�̌���
std::shared_ptr<Actor> World::findActor(const std::string& name) const {
    return actors_.find(name);
}

// �w�肵���^�O�������A�N�^�[�̌���
std::vector<std::shared_ptr<Actor>> World::findActorWithTag(const std::string& tag) const {
    return actors_.findWithTag(tag);
}

// �A�N�^�[����Ԃ�
int World::countActor() const {
    return actors_.count();
}

// �w�肵���^�O�������A�N�^�[����Ԃ�
int World::countActorWithTag(const std::string& tag) const {
    return actors_.countWithTag(tag);
}

std::list<std::shared_ptr<Actor>>& World::getActorsList()
{
    return actors_.getActorList();
}

void World::actorsClear()
{
    actors_.clear();
}

void World::addScore(int score)
{
}

// �J�����̎擾
std::shared_ptr<Actor> World::camera() {
    return camera_;
}

// ���C�g�̎擾
std::shared_ptr<Actor> World::light() {
    return light_;
}

std::shared_ptr<Field> World::field()
{
    return field_;
}

/*void World::addGui(std::shared_ptr<Gui> gui)
{
    //guiManager_.add(gui);
}*/

Effect* World::addEffect(std::shared_ptr<Effect> effect)
{
    effectManager_.add(effect);
    return effect.get();
}

std::shared_ptr<Effect> World::findEffect(const std::string& name)
{
    return effectManager_.findEffect(name);
}

void World::EffectClear()
{
    effectManager_.clear();
}












// �Q�[���I�[�o�[
void World::gameOver() {
    isGameOver_ = true;
}

// �Q�[���I�[�o�[�̏�����
void World::gameOverReset() {
    isGameOver_ = false;
}

// �Q�[���I�[�o�[���H
bool World::IsGameOver() const {
    return isGameOver_;
}

// �Q�[���N���A
void World::gameClear() {
    isGameClear_ = true;
}

//  �Q�[���N���A�̏�����
void World::gameClearReset() {
    isGameClear_ = false;
}

// �Q�[���N���A���H
bool World::IsGameClear() const
{
    return isGameClear_;
}

void World::gameEnd()
{
    isGameEnd_ = true;
}

bool World::isGameEnd() const
{
    return isGameEnd_;
}



