#ifndef WORLD_H_ 
#define WORLD_H_

#include "IWorld.h"
#include "app/Actors/ActorManager/ActorManager.h"
#include "app/Field/Field.h"
#include "app/Effect/EffectManager.h"


#include <gslib.h>

// ���[���h�N���X
class World :public IWorld {
public:
	// �R���X�g���N�^
	World();
	// �f�X�g���N�^
	~World();
	// �X�V
	void update(float delta_time);
	// �`��
	void draw() const;
	//�|�X�g�G�t�F�N�g���̕`��
	void postDraw()const;
	//�ʏ펞�̕`��
	void OnDraw()const;
	// ����
	void clear();
	// �J�����̒ǉ�
	void addCamera(std::shared_ptr<Actor> camera);
	// ���C�g�̒ǉ�
	void addLight(std::shared_ptr<Actor> light);
	// �t�B�[���h�̒ǉ�
	void addField(std::shared_ptr<Field> field);

	
	// �A�N�^�[�̒ǉ�
	template <class T, class... Args>
	void addactor(Args&&... args) {
		actors_.add<T>(args...);
	}
	// �A�N�^�[�̒ǉ�
	virtual void addActor(std::shared_ptr<Actor> actor) override;

	

	// �A�N�^�[�̌���
	virtual std::shared_ptr<Actor> findActor(const std::string& name) const override;
	// �w�肵���^�O�������A�N�^�[�̌���
	virtual std::vector<std::shared_ptr<Actor>> findActorWithTag(const std::string& tag) const override;
	// �A�N�^�[����Ԃ�
	virtual int countActor() const override;
	// �w�肵���^�O�������A�N�^�[����Ԃ�
	virtual int countActorWithTag(const std::string& tag) const override;
	// �A�N�^�[���X�g���擾
	std::list<std::shared_ptr<Actor>>& getActorsList() override;
	// �A�N�^�[���X�g���N���A
	void actorsClear() override;

	void addScore(int score) override;

	// �J�����̎擾
	virtual std::shared_ptr<Actor> camera() override;
	// �^�C�g�̎擾
	virtual std::shared_ptr<Actor> light() override;
	// �t�B�[���h�̎擾
	virtual std::shared_ptr<Field> field() override;

	// Gui�̒ǉ�
	//void addGui(std::shared_ptr<Gui> gui) override;

	// Effect�̒ǉ�
	Effect* addEffect(std::shared_ptr<Effect> effect) override;
	// Effect�̌���
	std::shared_ptr<Effect> findEffect(const std::string& name) override;
	//�G�t�F�N�g�̃N���A
	virtual void EffectClear()override;







	// �Q�[���I�[�o�[
	virtual void gameOver() override;
	// �Q�[���I�[�o�[�̏�����
	virtual void gameOverReset() override;

	// �Q�[���I�[�o�[���H
	bool IsGameOver() const;

	// �Q�[���N���A
	virtual void gameClear() override;
	// �Q�[���N���A�̏�����
	virtual void gameClearReset() override;

	// �Q�[���N���A���H
	bool IsGameClear() const;

	// �Q�[���I��
	void gameEnd() override;
	// �Q�[���I�����H
	bool isGameEnd() const;

	


	// �R�s�[�֎~
	World(const World& other) = delete;
	World& operator = (const World& other) = delete;

private:
	// �A�N�^�[�}�l�[�W���[
	ActorManager actors_;

	// Effect�}�l�[�W���[
	EffectManager effectManager_;

	// ���C�g
	std::shared_ptr<Actor> light_{ nullptr };
	// �J����
	std::shared_ptr<Actor> camera_{ nullptr };
	// �t�B�[���h
	std::shared_ptr<Field> field_{ nullptr };



private:

	// �Q�[���I�[�o�[���H
	bool		 isGameOver_{ false };
	// �Q�[���N���A���H
	bool		 isGameClear_{ false };
	// �Q�[���I��
	bool		 isGameEnd_{ false };



};

#endif