#ifndef APP_STATE_BASE_STATEBASE_H_
#define APP_STATE_BASE_STATEBASE_H_

#include <memory>

namespace app {

	class StateMachine;	// ステートマシンの前方宣言

	// ステートの基底クラス
	class StateBase {
	public:
		StateBase(StateMachine* stateMachine) : 
			stateMachine_(stateMachine){}
		// 仮想デストラクタ
		virtual ~StateBase() = default;
		// 初期化
		virtual void init() = 0;
		// 終了
		virtual void final() = 0;
		// 更新
		virtual void update() = 0;
		// 遅延更新
		virtual void lateUpdate() = 0;
		// 描画
		virtual void draw() const = 0;
		// 遅延描画
		virtual void lateDraw() const = 0;

		// ステートがアクティブ時のみ描画
		virtual void drawOnlyWhenActive() const {};

	public:
		// 透過するかのフラグを取得
		bool isTransparency() const { return isTransparency_; }

	protected:
		StateMachine* stateMachine_;

	protected:
		bool isTransparency_{ false };
	};
}

#endif