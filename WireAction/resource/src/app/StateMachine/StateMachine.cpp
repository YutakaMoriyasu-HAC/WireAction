#include "StateMachine.h"

#include "app/time/Time.h"
#include "StateBase.h"

using namespace app;

// デストラクタ
StateMachine::~StateMachine()
{
	reset();		// スタックを空に
	clearState();	// 追加したステートのリストを空に
}

/*=============================================================================
*
* std::make_shared<クラス名>()でシェアードポインターを生成して引数に渡す
*
*============================================================================*/
// ステートを追加
void StateMachine::addState(const int stateTag, std::shared_ptr<StateBase> state)
{
	stateList_[stateTag] = state;
}
// ステートをセットする
void StateMachine::reset(const int stateTag)
{
	// スタックを空に
	std::stack<std::shared_ptr<StateBase>>().swap(prevsState_);
	// タイマを空に
	std::stack<float>().swap(timer_);
	// タグidが-1なら終了
	if (stateTag == -1) return;
	// 新しいステートを追加
	currentState_ = stateList_[stateTag];
	// 新しいステートの初期化
	currentState_->init();
	// 新しいタイマを追加
	timer_.push(0.0f);
	// 現在のステートをセット
	nowStateID_ = stateTag;
}
// 更新
void StateMachine::update()
{
	// 現在のステートがnullの場合は処理をしない
	if (currentState_== nullptr) return;
	// タイマの更新
	timer_.top() += app::Time::deltaTime;
	// 現在のステートの更新
	currentState_->update();
}
// 遅延更新
void StateMachine::lateUpdate()
{
	// 現在のステートの遅延更新
	currentState_->lateUpdate();
}
// 描画
void StateMachine::draw() const
{
	// 前回のステートの描画
	if (currentState_->isTransparency() && !prevsState_.empty())
		prevsState_.top()->draw();
	// 現在のステートの描画
	currentState_->draw();

	// アクティブ時のみ描画
	currentState_->drawOnlyWhenActive();
}
// 遅延描画
void StateMachine::lateDraw() const
{
	// 前回のステートの遅延描画
	if (currentState_->isTransparency() && !prevsState_.empty())
		prevsState_.top()->lateDraw();
	// 現在のステートの遅延描画
	currentState_->lateDraw();
}
// 次のステートへ(現在のステートを変更)
void StateMachine::nextState(const int stateTag)
{
	// ステートの終了処理
	currentState_->final();
	// 現在のステートを1つ前のステートへ
	prevStateID_ = nowStateID_;
	// タイマを1つ取り出す
	timer_.pop();
	// 新しいステートを追加
	currentState_ = stateList_[stateTag];
	// 新しいステートの初期化
	currentState_->init();
	// 新しいタイマを追加
	timer_.push(0.0f);
	// 新しいステートを現在のステートへ
	nowStateID_ = stateTag;
}
// ステートを変更(スタックを空にして変更)
void StateMachine::chengeState(const int stateTag)
{
	// 現在のステートを1つ前のステートへ
	if (nowStateID_ != -1) prevStateID_ = nowStateID_;
	// 現在のステートの終了処理
	currentState_->final();
	// 空になるまで終了処理を呼ぶ
	while (!prevsState_.empty())
	{
		// ステートの終了処理
		prevsState_.top()->final();
		// ステートを1つ取り出す
		prevsState_.pop();
	}
	std::stack<float>().swap(timer_);
	// 新しいステートを追加
	currentState_ = stateList_[stateTag];
	// 新しいステートの初期化
	currentState_->init();
	// 新しいタイマを追加
	timer_.push(0.0f);
	// 新しいステートを現在のステートへ
	nowStateID_ = stateTag;
}
// ステートを上乗せ
void StateMachine::pushState(const int stateTag)
{
	// 現在のステートを1つ前のステートへ
	if (nowStateID_ != -1) prevStateID_ = nowStateID_;
	// 現在のステートを前のステートへ移す
	prevsState_.push(currentState_);
	// 新しいステートを追加
	currentState_ = stateList_[stateTag];
	// 新しいステートの初期化
	currentState_->init();
	// 新しいタイマを追加
	timer_.push(0.0f);
	// 新しいステートを現在のステートへ
	nowStateID_ = stateTag;
}
// ステートを1つ戻す
void StateMachine::popState()
{
	if (prevsState_.empty()) return;
	// 一時的に保存
	int id = prevStateID_;
	// 現在のステートを1つ前のステートへ
	if (nowStateID_ != -1) prevStateID_ = nowStateID_;
	else return;
	// ステートの終了処理
	currentState_->final();
	// 前のステートを現在のステートへ移す
	currentState_ = prevsState_.top();
	// ステートを1つ取り出す
	prevsState_.pop();
	// タイマを1つ取り出す
	timer_.pop();
	// 新しいステートを現在のステートへ
	nowStateID_ = id;
}
// 追加したステートをクリア
void StateMachine::clearState()
{
	stateList_.clear();
	std::unordered_map<int, std::shared_ptr<StateBase>>().swap(stateList_);
	std::stack<float>().swap(timer_);
}
// 現在のステートの経過時間を取得
const float StateMachine::getNowStateTimer() const
{
	return timer_.top();
}
// 現在のステートIDを取得
const int StateMachine::getNowStateID() const
{
	return nowStateID_;
}
// 1つ前のステートIDを取得
const int StateMachine::getPrevStateID() const
{
	return prevStateID_;
}
