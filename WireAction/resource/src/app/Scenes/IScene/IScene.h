#ifndef ISCENE_H_
#define ISCENE_H_

#include <string>

//�V�[�����ۃC���^�[�t�F�[�X
class IScene {
public:
	//���z�f�X�g���N�^
	virtual ~IScene() = default;
	//�J�n
	virtual void start() = 0;
	//�X�V
	virtual void update(float delta_time) = 0;
	//�`��
	virtual void draw() = 0;
	//���̃V�[������Ԃ�
	virtual std::string next() const = 0;
	//�I��
	virtual void end() = 0;

	// �ǂݍ���
	void load()
	{
		isLoadEnd_ = false;
		loadAssets();
		isLoadEnd_ = true;
	};

	// �ǂݍ��݂͏I��������
	bool isLoadEnd()
	{
		return isLoadEnd_;
	}

	//�I�����Ă��邩
	bool isEnd()const
	{
		return isEnd_;
	}

protected:
	// �V�[�����I������
	void sceneEnd()
	{
		isEnd_ = true;
	}

	virtual void loadAssets() {};

protected:
	bool isEnd_{ false };
	std::string name_;

private:
	bool isLoadEnd_{ false };	// �ǂݍ��ݏI���t���O

};

#endif