#include "app/AnimatedMesh/AnimatedMesh.h"
#include <cmath>
#include <algorithm>

//補間フレーム数
const GSfloat LerpTime{ 10.0f };

//コンストラクタ
AnimatedMesh::AnimatedMesh(GSuint id, GSuint motion, bool loop, GSuint num_bones) :
	id_{ id },
	motion_{ motion },
	motion_timer_{ 0.0f },
	motion_loop_{ loop },
	transform_{ GS_MATRIX4_IDENTITY },
	prev_motion_{ motion },
	prev_motion_timer_{ 0.0f },
	lerp_timer_{ 0.0f },
	local_bone_matrices_{ num_bones },
	bone_matrices_{ num_bones },
	motion_speed_{ 1.0f },
	endTimePlus_{0.0f} {
}


//更新
void AnimatedMesh::update(float delta_time) {
	

	//更新前のタイマー値を取っておく
	GSfloat prev_timer = motion_timer_;
	// アニメーションタイマの更新
	motion_timer_ += delta_time * motion_speed_;
	// ループさせる
	if (motion_timer_ > motionEndTime() - endTimePlus_) {
		if (motion_loop_) {
			motion_timer_ = 0;
		}
		else {
			motion_timer_ = motionEndTime() - endTimePlus_;
		}
	}


	// 補間タイマの更新(LerpTime以上にならないようにクランプする）
	lerp_timer_ = std::min(lerp_timer_ + delta_time, LerpTime);
	// モーションがループしたかどうか。
	   // 現在の時間が1フレーム前の時間より小さい場合は、ループしたと判断できる。
	bool looped = motion_timer_ < prev_timer;
	// 全イベントをチェックし、必要であればイベントを発行する
	for (const auto& event : events_)
	{
		// 現在のモーションがイベント対象のモーションでなければ、何もしない
		if (event->motion_ != motion_) continue;

		if (looped)
		{
			if (prev_timer < event->time_ || event->time_ <= motion_timer_)
			{
				event->callback_(); // コールバック処理を呼び出す
			}
		}
		else
		{
			if (prev_timer < event->time_ && event->time_ <= motion_timer_)
			{
				event->callback_(); // コールバック処理を呼び出す
			}
		}
	}


	if (is_end_motion())
	{
		//motion_speed_ = 1.0f;
	}
}

//描画
void AnimatedMesh::draw() const {
	//スケルトンの自動計算を無効にする
	gsDisable(GS_CALC_SKELETON);
	//スケルトンのバインド
	gsBindSkeleton(id_);
	//ボーンの変換行列を設定
	gsSetMatrixSkeleton(bone_matrices_.data());

	//メッシュの描画
	glPushMatrix();
		glMultMatrixf(transform_);
		gsDrawMesh(id_);
	glPopMatrix();

	//スケルトンの自動計算を有効にする
	gsEnable(GS_CALC_SKELETON);
}

//モーションの変更
void AnimatedMesh::change_motion(GSuint motion, bool loop) {
	//現在と同じモーションの場合は何もしない
	if (motion_ == motion) return;
	//補間中(前半)は、前回のモーションを更新しないようにする
	if (lerp_timer_ > (LerpTime * 0.5f)) {
		//前回のモーション番号とモーション時間を保存
		prev_motion_ = motion_;
		prev_motion_timer_ = motion_timer_;
		//補間中タイマのリセット
		lerp_timer_ = 0.0f;
	}
	//モーションの更新
	motion_ = motion;
	//モーションタイマの初期化
	motion_timer_ = 0.0f;
	//モーションループフラグの設定
	motion_loop_ = loop;
}

//モーションの変更
void AnimatedMesh::change_motionS(GSuint motion, bool loop, float speed, float lerp, float startTime, float endTime) {

	//速さだけは更新
	motion_speed_ = speed;

	//現在と同じモーションの場合は何もしない
	if (motion_ == motion) return;
	//補間中(前半)は、前回のモーションを更新しないようにする
	if (lerp_timer_ > (LerpTime * lerp)) {
		//前回のモーション番号とモーション時間を保存
		prev_motion_ = motion_;
		prev_motion_timer_ = motion_timer_;
		//補間中タイマのリセット
		lerp_timer_ = 0.0f;
	}
	//モーションの更新
	motion_ = motion;
	//モーションタイマの初期化
	motion_timer_ = startTime;
	//モーションループフラグの設定
	motion_loop_ = loop;

	

	//終わりの時間
	endTimePlus_=endTime;
}

//座標変換を行う
void AnimatedMesh::transform(const GSmatrix4& matrix) {
	transform_ = matrix;
	//ボーンごとのローカル変換行列を計算
	gsCalculateAnimationLerp(
		id_, prev_motion_, prev_motion_timer_,	//前回のアニメーション
		id_, motion_, motion_timer_,			//現在のアニメーション
		lerp_timer_ / LerpTime,
		local_bone_matrices_.data()
	);

	//ボーンの変換行列を計算
	gsBindSkeleton(id_);
	gsCalculateSkeleton(NULL, local_bone_matrices_.data(), bone_matrices_.data());
}

//モーションが終了しているか？
bool AnimatedMesh::is_end_motion() const {
	//ループのモーションは終了しない
	if (motion_loop_)return false;
	//終了しているか？
	return motion_timer_ >= (motionEndTime() - endTimePlus_);
}

//現在のモーションの再生時間を取得
float AnimatedMesh::motion_time() const {
	return motion_timer_;
}

//モーションの再生時間を設定
void AnimatedMesh::motion_time(float time) {
	motion_timer_ = time;
}

//モーションの終了時間を取得
float AnimatedMesh::motionEndTime() const {
	return gsGetEndAnimationTime(id_, motion_);
}

//ボーンのワールド変換行列を取得
GSmatrix4 AnimatedMesh::bone_matrices(int bone_no)const {
	return bone_matrices_[bone_no] * transform_;
}

//モデルを変更
void AnimatedMesh::changeModel(GSuint model) {
	id_ = model;
}