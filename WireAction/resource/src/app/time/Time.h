#ifndef APP_TIME_TIME_H_
#define APP_TIME_TIME_H_

namespace app {
	// 時間クラス
	class Time
	{
	public:
		static float deltaTime; // デルタタイム

		static float deltaRate;	//デルタタイムレート

	public:
		// デルタタイムを更新
		static void UpdateDeltaTime(const float time);
		// デルタタイム倍率を変動
		static void setDeltaTimeRate(const float rate);
	};
}

#endif