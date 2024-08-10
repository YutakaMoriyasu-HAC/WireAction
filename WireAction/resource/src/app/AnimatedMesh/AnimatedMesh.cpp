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
	bone_matrices_{ num_bones }{
}


//更新
void AnimatedMesh::update(float delta_time) {
	//モーションタイマの更新
	motion_timer_ += delta_time;
	// ループアニメーションか？
	if (motion_loop_) {
		// モーションタイマをループさせる
		motion_timer_ = std::fmod(motion_timer_, motion_end_time());
	}
	else {
		// モーションタイマをクランプする
		motion_timer_ = std::min(motion_timer_, motion_end_time() - 1.0f);
	}
	// 補間タイマの更新(LerpTime以上にならないようにクランプする）
	lerp_timer_ = std::min(lerp_timer_ + delta_time, LerpTime);
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
	return motion_timer_ >= (motion_end_time() - 1.0f);
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
float AnimatedMesh::motion_end_time() const {
	return gsGetEndAnimationTime(id_, motion_);
}

//ボーンのワールド変換行列を取得
GSmatrix4 AnimatedMesh::bone_matrices(int bone_no)const {
	return bone_matrices_[bone_no] * transform_;
}