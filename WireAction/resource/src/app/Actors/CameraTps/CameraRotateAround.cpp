#include "CameraRotateAround.h"
#include "app/Worlds/IWorld.h"
#include "app/Field/Field.h"
#include "app/Ray/Line.h"

//プレイヤーからの相対座標(z座標のみ)
const GSvector3 PlayerOffset{ 0.0f, 0.0f, -5.0f };
//カメラの注視点の補正値
const GSvector3 ReferencePointOffset{ 0.0f,1.8f,0.0f };

//コンストラクタ
CameraRotateAround::CameraRotateAround(
	IWorld* world, const GSvector3& position, const GSvector3& at)
{
	//ワールドを設定
	world_ = world;
	//タグの設定
	tag_ = "CameraTag";
	//名前の設定
	name_ = "Camera";
	//視点の位置を設定
	transform_.position(position);
	//注視点を設定(注視点の方向に向きを変える)
	transform_.lookAt(at);

	//x軸周りの回転角度の初期化
	pitch_ = (at - position).getPitch();
	//y軸周りの回転角度の初期化
	yaw_ = (at - position).getYaw();
}

//更新
void CameraRotateAround::update(float delta_time) {
	//プレイヤーを検索
	Actor* player = world_->find_actor("Player");
	if (player == nullptr) return;

	//y軸まわりにカメラを回転させる
	if (gsGetKeyState(GKEY_LEFT))	yaw_ -= 1.0f * delta_time;
	if (gsGetKeyState(GKEY_RIGHT))	yaw_ += 1.0f * delta_time;
	//x軸まわりにカメラを回転させる
	if (gsGetKeyState(GKEY_UP))		pitch_ -= 1.0f * delta_time;
	if (gsGetKeyState(GKEY_DOWN))	pitch_ += 1.0f * delta_time;
	//x軸まわりの回転角度の制限をする
	pitch_ = CLAMP(pitch_, -10.0f, 30.0f);

	

	//注視点の座標を求める
	GSvector3 at = player->transform().position() + ReferencePointOffset;
	//カメラの座標を求める
	GSvector3 position = at + GSquaternion::euler(pitch_, yaw_, 0.0f) * PlayerOffset;

	//フィールドとの衝突判定
	Line line{ at, position };
	GSvector3 intersect;
	if (world_->field()->collide(line, &intersect)) {
		position = intersect;
	}

	//座標の設定
	transform_.position(position);
	//注視点の方向を見る
	transform_.lookAt(at);
}

//描画
void CameraRotateAround::draw() const {
	//視点の位置
	GSvector3 eye = transform_.position();
	//注視点の位置
	GSvector3 at = eye + transform().forward();
	//視点の上方向
	GSvector3 up = transform_.up();

	//カメラの設定
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		eye.x, eye.y, eye.z,	//視点の位置
		at.x, at.y, at.z,		//注視点の位置
		up.x, up.y, up.z		//視点の上方向
	);
}