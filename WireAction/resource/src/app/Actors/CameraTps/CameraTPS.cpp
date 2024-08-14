#include "CameraTps.h"
#include "app/Worlds/IWorld.h"
#include "app/Ray/Line.h"
#include "app/Field/Field.h"
#include "app/Input/InputManager.h"
#include "app/Math/Math.h"
#include "app/Actors/Player/Player.h"
#include<memory>

//カメラの注視点の補正値
const GSvector3 ReferencePointOffset{ 0.0f,1.7f,0.0f };
float cameraDistance_{ 15.0f };

CameraTps::CameraTps(IWorld* world, const GSvector3& position, const GSvector3& at) :
	Actor()
{
	//ワールドを設定
	world_ = world;
	//タグの設定
	tag_ = "CameraTag";
	//名前の設定
	name_ = "Camera";
	//注視点の位置
	transform_.position(position); //0,8,3
	//注視点を設定（注視点の方向に向きを変える）
	transform_.lookAt(at); //0,0,-3

	//InputManager::RightAxisReversX(false);
	//InputManager::RightAxisReversY(false);

	startFlag_ = true;

	cameraAngle_.y = 25;

	

}

void CameraTps::update(float delta_time)
{
	//プレイヤー取得
	//find_actorはActorクラスで取ってきてしまうのでdynamic_pointer_castでPlayerクラスに変える
	std::shared_ptr<Player> player = std::dynamic_pointer_cast<Player>(world_->find_actor("Player"));

	//注視点取得
	smoothPlayerLookPos_ = player->getCameraLookPoint();
	float smoothPercent = 10.0f;
	//位置補正タイム
	if (smoothPlayerLookPos_.y != prevPlayerLookPos_.y) {
		smoothPlayerLookPos_.y = (smoothPlayerLookPos_.y - prevPlayerLookPos_.y) * (smoothPercent / 100.0f) + prevPlayerLookPos_.y;
	}
	

	if (startFlag_) {
		cameraAngle_.x = player->transform().localEulerAngles().y * (-1) + 270;
		cameraAngle_.y = 25;
		startFlag_ = false;
	}

	//LBでカメラリセット
	if (InputManager::IsCameraResetState()) {
		cameraAngle_.x = player->transform().localEulerAngles().y * (-1) + 270;
		cameraAngle_.y = 25;
	}

	//入力
	cameraAngle_.x += InputManager::CameraRotation().x * InputManager::GetCameraSensitivity() * delta_time;
	if (InputManager::CameraRotation().y == -1 && cameraLevel_ == 0) {
		cameraAngle_.y = 30;
		cameraDistance_ = 20.0f;
		cameraLevel_ = 1;
	}
	if (InputManager::CameraRotation().y == 1 && cameraLevel_ == 1) {
		cameraAngle_.y = 20;
		cameraDistance_ = 12.0f;
		cameraLevel_ = 0;
	}

	//cameraAngle_.y = 25;
	if (cameraAngle_.x < 0) cameraAngle_.x += 360;
	if (cameraAngle_.x > 360) cameraAngle_.x -= 360;
	if (cameraAngle_.y <= -40) cameraAngle_.y = -40;
	if (cameraAngle_.y >= 60) cameraAngle_.y = 60;
	cameraAngleE_ = cameraAngle_ * (math::PI / 180);

	GSvector3 position;

	//カメラのy座標決定
	position.y = smoothPlayerLookPos_.y + sinf(cameraAngleE_.y) * cameraDistance_;
	float xzPos = cosf(cameraAngleE_.y) * cameraDistance_;

	//カメラを上に向けたときの位置調整
	if (position.y < smoothPlayerLookPos_.y) {
		float bairitsu = (sinf(cameraAngleE_.y) * cameraDistance_) / -ReferencePointOffset.y;
		position.y = smoothPlayerLookPos_.y;
		xzPos = cosf(cameraAngleE_.y) * cameraDistance_ / bairitsu;
		if (xzPos >= cameraDistance_) {
			xzPos = cosf(cameraAngleE_.y) * cameraDistance_;
		}
	}

	//カメラのxz座標決定
	position.x = player->transform().position().x + cosf(cameraAngleE_.x) * xzPos;
	position.z = player->transform().position().z + sinf(cameraAngleE_.x) * xzPos;


	//注視点の位置を求める(プレイヤーの頭部の少し上あたりの座標)
	GSvector3 at = smoothPlayerLookPos_ + ReferencePointOffset;

	// フィールドとの衝突判定
	//collideField(0.1f);		//フィールドとの衝突判定
	Line line{ at,position };
	GSvector3 intersect;
	if (world_->field()->collide(line, &intersect))
	{
		position = intersect;
	}

	//スムースダンプによる滑らかな補間
	const float SmoothTime{ 12.0f };	//補間フレーム数
	const float MaxSpeed{ 1.0f };		//移動スピードの最大値
	position = GSvector3::smoothDamp(
		transform_.position(), position, velocity_,
		SmoothTime, MaxSpeed, delta_time);

	/*at = GSvector3::smoothDamp(
		prevPlayerLookPos_, at, velocity_,
		SmoothTime, MaxSpeed, delta_time);*/


	//視点の位置を設定
	transform_.position(position);
	//注視点を設定(注視点の方向に向きを変える)
	transform_.lookAt(at);

	//最後に前のフレームの注視点としてセット
	prevPlayerLookPos_ = smoothPlayerLookPos_;
}


void CameraTps::draw() const
{
	//視点の位置
	GSvector3 eye = transform_.position();
	//注視点の位置
	GSvector3 at = eye + transform_.forward();
	//カメラの設定
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		eye.x, eye.y, eye.z,//視点の位置
		at.x, at.y, at.z,//注視点の位置
		0.0f, 1.0f, 0.0f//視点の上方向
	);
}

