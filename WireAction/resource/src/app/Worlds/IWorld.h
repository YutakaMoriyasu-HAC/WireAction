#ifndef IWORLD_H_ 
#define IWORLD_H_

#include <string>
#include <vector>
#include <memory>
#include <list>
#include <gslib.h>

class Actor;
class Field;
class Item;
class InventoryFrame;
class Effect;
class Gui;
class TimeLimitClock;
class Gauge;
class PostEffect;
class KillCountManager;
// ���[���h���ۃC���^�[�t�F�[�X
class IWorld
{
public:
	// ���z�f�X�g���N�^
	virtual ~IWorld() = default;


	// �A�N�^�[��ǉ�
	virtual void addActor(std::shared_ptr<Actor> actor) = 0;

	// �A�N�^�[�̌���
	virtual std::shared_ptr<Actor> findActor(const std::string& name) const = 0;
	// �w�肵���^�O�������A�N�^�[�̌���
	virtual std::vector<std::shared_ptr<Actor>> findActorWithTag(const std::string& tag) const = 0;
	// �A�N�^�[����Ԃ�
	virtual int countActor() const = 0;
	// �w�肵���^�O�������A�N�^�[����Ԃ�
	virtual int countActorWithTag(const std::string& tag) const = 0;
	// �A�N�^�[�̃��X�g���擾
	virtual std::list<std::shared_ptr<Actor>>& getActorsList() = 0;
	// �A�N�^�[���X�g���N���A
	virtual void actorsClear() = 0;



	// �X�R�A�̉��Z
	virtual void addScore(int score) = 0;
	// �Q�[���I�[�o�[
	virtual void gameOver() = 0;
	// �Q�[���I�[�o�[�̏�����
	virtual void gameOverReset() = 0;
	// �Q�[���N���A
	virtual void gameClear() = 0;
	// �Q�[���N���A�̏�����
	virtual void gameClearReset() = 0;

	// �Q�[���I��
	virtual void gameEnd() = 0;

	// �J�����̎擾
	virtual std::shared_ptr<Actor> camera() = 0;
	// ���C�g�̎擾
	virtual std::shared_ptr<Actor> light() = 0;
	// �t�B�[���h�̎擾
	virtual std::shared_ptr<Field> field() = 0;

	//Gui�̒ǉ�
	//virtual void addGui(std::shared_ptr<Gui> gui) = 0;

	// �G�t�F�N�g�̒ǉ�
	virtual Effect* addEffect(std::shared_ptr<Effect> effect) = 0;
	// �G�t�F�N�g�̌���
	virtual std::shared_ptr<Effect> findEffect(const std::string& name) = 0;
	//�G�t�F�N�g�̃N���A
	virtual void EffectClear() = 0;







	// �Q�[���I�[�o�[���H
	virtual bool IsGameOver() const = 0;

	virtual bool IsGameClear() const = 0;




};

#endif