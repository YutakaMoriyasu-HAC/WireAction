#ifndef APP_STATE_MACHINE_STATEMACHINE_H_
#define APP_STATE_MACHINE_STATEMACHINE_H_

#include <memory> // ←スマートポインタを使用するのに必要
#include <stack>
#include <unordered_map>
#include <functional>

namespace app {

	class StateBase;

	// ステートマシン
	class StateMachine
	{
	public:
		StateMachine() = default;
		~StateMachine();

		/// <summary>
		/// ステートを追加
		/// </summary>
		/// <param name="stateTag">ステートの番号(namelistでenumを用意)</param>
		/// <param name="state">std::make_sharedでシェアードポインターを生成して引数に渡す</param>
		void addState(const int stateTag, std::shared_ptr<StateBase> state);

		/// <summary>
		/// ステートを追加
		/// </summary>
		/// <typeparam name="T">追加するStateのクラス</typeparam>
		/// <typeparam name="...Args"></typeparam>
		/// <param name="stateTag">ステートの番号</param>
		/// <param name="...args">Stateクラスの引数</param>
		template <class T, class... Args>
		void addState(const int stateTag, Args&&... args) {
			stateList_[stateTag] = std::make_shared<T>(args...);
		}

		/// <summary>
		/// 初期ステートを設定する(スタック内のステートのFinal関数は呼ばれない)
		/// </summary>
		/// <param name="stateTag">初期設定するステートの番号</param>
		void reset(const int stateTag = -1);
		// 更新
		void update();
		// 遅延更新
		void lateUpdate();
		// 描画
		void draw() const;
		// 遅延描画
		void lateDraw() const;

		/// <summary>
		/// 次のステートへ遷移(スタックから現在のステートを取り出し、次のステートを挿入)
		/// </summary>
		/// <param name="stateTag">次のステートの番号</param>
		void nextState(const int stateTag);

		/// <summary>
		/// ステートを変更(スタックを空にして、変更するステートを挿入)
		/// </summary>
		/// <param name="stateTag">変更するステートの番号</param>
		void chengeState(const int stateTag);

		/// <summary>
		/// ステートを上乗せ(スタックへ次のステートを挿入)。
		/// 現在のステートを一時中断して次のステートへ遷移出来る。
		/// PopState()を呼ぶと現在のステートを続きから動かせる。
		/// </summary>
		/// <param name="stateTag">次のステートの番号</param>
		void pushState(const int stateTag);

		/// <summary>
		/// ステートを1つ戻す(スタックからステートを１つ取り出す)
		/// </summary>
		void popState();
		// 追加したステートをクリア
		void clearState();
		// 現在のステートの経過時間を取得
		const float getNowStateTimer() const;
		// 現在のステートIDを取得
		const int getNowStateID() const;
		// 1つ前のステートIDを取得
		const int getPrevStateID() const;

	private:
		int nowStateID_{ -1 };		// 現在のステートid
		int prevStateID_{ -1 };	// 1つ前のステートid

	private:
		std::stack<float> timer_;
		std::shared_ptr<StateBase> currentState_;
		std::stack<std::shared_ptr<StateBase>> prevsState_;
		std::unordered_map<int, std::shared_ptr<StateBase>> stateList_;

	};
}

#endif
