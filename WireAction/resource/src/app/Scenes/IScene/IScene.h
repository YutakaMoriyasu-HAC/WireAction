#ifndef ISCENE_H_
#define ISCENE_H_

#include <string>

//シーン抽象インターフェース
class IScene {
public:
	//仮想デストラクタ
	virtual ~IScene() = default;
	//開始
	virtual void start() = 0;
	//更新
	virtual void update(float delta_time) = 0;
	//描画
	virtual void draw() = 0;
	//次のシーン名を返す
	virtual std::string next() const = 0;
	//終了
	virtual void end() = 0;

	// 読み込み
	void load()
	{
		isLoadEnd_ = false;
		loadAssets();
		isLoadEnd_ = true;
	};

	// 読み込みは終了したか
	bool isLoadEnd()
	{
		return isLoadEnd_;
	}

	//終了しているか
	bool isEnd()const
	{
		return isEnd_;
	}

protected:
	// シーンを終了する
	void sceneEnd()
	{
		isEnd_ = true;
	}

	virtual void loadAssets() {};

protected:
	bool isEnd_{ false };
	std::string name_;

private:
	bool isLoadEnd_{ false };	// 読み込み終了フラグ

};

#endif