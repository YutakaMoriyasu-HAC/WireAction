#include "PlayerPendulumState.h"
#include "app/Namelist/PlayerStateList.h"
#include "app/Worlds/World.h"
#include "app/time/Time.h"
#include "app/Math/Math.h"
#include "app/Input/InputManager.h"
#include "app/Namelist/PlayerStateList.h"
#include "app/NameList/Assets.h"



// コンストラクタ
PlayerPendulumState::PlayerPendulumState(Player* parent, IWorld* world, app::StateMachine* stateMachine) :
	StateBase(stateMachine), parent_(parent), world_(world)
{

}
// 初期化
void PlayerPendulumState::init()
{
    wireX_= 0.0f;
    wireY_= 0.0f;
    wireZ_= 0.0f;
    angle_= 0.0f;
    phi_  = 0.0f;
    wireRadius_      =0.0f;
    angleSpeed_      =0.0f;
    angleSpeedSpeed_ =0.0f; //加速度の加速度
    angle90_         =0.0f;
    sinTheta_        =0.0f;
    previousPhi_     =0.0f;
    position_= parent_->transform().position();
    pendulumInvert_ = 1;

	//ワイヤーに必要な初期値を計算する
	wirePosition_ = parent_->getCenterPendulum();
	GSvector3 playerPosition = parent_->transform().position();
	//ワイヤーの長さ(wireRadiusは振り子の半径になる)
	wireX_ = wirePosition_.x - playerPosition.x;
	wireY_ = wirePosition_.y - playerPosition.y;
	wireZ_ = wirePosition_.z - playerPosition.z;
	wireRadius_ = sqrtf(wireX_ * wireX_ + (wireY_ * wireY_) + wireZ_ * wireZ_);

	//ワイヤーの支点のY座標だけを主人公に合わせた位置
	pointH_ = GSvector3{ wirePosition_.x,playerPosition.y,wirePosition_.z };

    //PointHからきゃぴっちに伸ばしたベクトル
    GSvector3 phiA = GSvector3{ playerPosition.x - pointH_.x, 0, playerPosition.z - pointH_.z };

    //PointHから左にまっすぐ伸ばしたベクトル
    GSvector3 phiB = GSvector3{ -abs(playerPosition.x - pointH_.x), 0, 0 };

    //上から見た角度Φ
    //真正面のときだけちょっと特別な処理
    if (phiB == GSvector3::zero())
    {
        if (wireZ_ > 0)
        {
            phi_ = math::PI / 2;
        }
        else
        {
            phi_ = math::PI * 3 / 2;

        }
    }
    else
    {
        if (wireZ_ > 0)
        {
            phi_ = acos((phiA.x * phiB.x + phiA.y * phiB.y + phiA.z * phiB.z) /
                (sqrt(phiA.x * phiA.x + phiA.y * phiA.y + phiA.z * phiA.z) *
                    sqrt(phiB.x * phiB.x + phiB.y * phiB.y + phiB.z * phiB.z)));
        }
        else
        {
            phi_ = math::PI * 2 - acos((phiA.x * phiB.x + phiA.y * phiB.y + phiA.z * phiB.z) /
                (sqrt(phiA.x * phiA.x + phiA.y * phiA.y + phiA.z * phiA.z) *
                    sqrt(phiB.x * phiB.x + phiB.y * phiB.y + phiB.z * phiB.z)));
        }
    }

    //Θが0度になる位置
    thetaZeroPosition_ = GSvector3{ wirePosition_.x + wireRadius_ * cos(phi_),
        wirePosition_.y, wirePosition_.z + wireRadius_ * sin(phi_) };

    //wirePositionからそこまでのベクトル
    GSvector3 thetaA = thetaZeroPosition_ - wirePosition_;

    //支点からきゃぴっちまでのベクトル
    GSvector3 thetaB = playerPosition - wirePosition_;




    //初期の角度(Θ)決定
    if (wireY_ < 0)
    {
        angle_ = acos((thetaA.x * thetaB.x + thetaA.y * thetaB.y + thetaA.z * thetaB.z) /
            (sqrt(thetaA.x * thetaA.x + thetaA.y * thetaA.y + thetaA.z * thetaA.z) *
                sqrt(thetaB.x * thetaB.x + thetaB.y * thetaB.y + thetaB.z * thetaB.z)));
    }
    else
    {
        angle_ = math::PI * 2 - acos((thetaA.x * thetaB.x + thetaA.y * thetaB.y + thetaA.z * thetaB.z) /
            (sqrt(thetaA.x * thetaA.x + thetaA.y * thetaA.y + thetaA.z * thetaA.z) *
                sqrt(thetaB.x * thetaB.x + thetaB.y * thetaB.y + thetaB.z * thetaB.z)));
    }
    
    velocity_ = GSvector3::zero();
    mSAfP_Near = phi_;

}
// 終了
void PlayerPendulumState::final()
{
}
// 更新
void PlayerPendulumState::update()
{
    GSvector3 playerPosition = parent_->transform().position();

    //ワイヤー離す
    if (InputManager::IsAButtonTrigger()) {
        cameraLookPoint_ = parent_->GetPosition() + velocity_;
        parent_->changeState(PlayerStateList::State_Jump);
        parent_->setCameraLookPoint(cameraLookPoint_);
        parent_->ChangeMotionS(Motion_Jump, false); //モーション変更

        

        // モーションの変更
        // 移動量のxy成分だけ更新
        parent_->velocity(velocity_);
        return;
    }

    //着地したら終了
    if (parent_->isGround()) {
        /*parent_->changeState(PlayerStateList::State_Walk);
        parent_->ChangeMotionS(Motion_Idle, false); //モーション変更
        parent_->velocity(GSvector3::zero());
        return;*/
        wireY_= wirePosition_.y - playerPosition.y;
        wireRadius_ = sqrtf(wireX_ * wireX_ + (wireY_) * (wireY_));

    }


	w_ = G * M; //重力[N]

    wireX_ = wireRadius_ * cos(angle_); //斜めの長さ(上から見たとき)
    if ((int)round(angle_ * (180 / math::PI)) % 180 == 0)
    {
        wireY_ = 0;
    }
    else
    {
        wireY_ = wireRadius_ * sin(angle_);
    }

    //ワイヤー中の座標決定
    //XとZ座標はphi(y軸回転の角度)から求める
    position_.x = wirePosition_.x + wireX_ * cos(phi_);

    //座標を振り子の座標に
    position_.y = wireY_ + wirePosition_.y;

    position_.z = wirePosition_.z + wireX_ * sin(phi_);

    angleSpeedSpeed_ = previousPosition_.y - position_.y;

    velocity_.x = (position_.x - previousPosition_.x) * 1.0f; //マジックナンバーにしてるけど、
    velocity_.z = (position_.z - previousPosition_.z) * 1.0f; //急加速したいときに変数にすればいい

    //向き
    if (angleSpeedSpeed_ < 0) //上向き
    {
        upDirection_ = 1;
    }
    else
    {
        upDirection_ = -1;
    }

    //vyは振り子の遠心力から持ってくる
    //遠心力 f = mv^2/r = mrω^2
    //↑この式を変換すれば縦向きの速度vが求まる
    //ωは角速度(angleSpeed)、rは半径(wireRadius)
    velocity_.y = sqrt((wireRadius_ * wireRadius_) * (angleSpeed_ * angleSpeed_)) * upDirection_* 0.016f;

    //今の位置を記録
    previousPosition_ = position_;

    //仮で取ってた角度(Θ)を真下が0度になるように調整
    angle90_ = -angle_ + math::PI / 180 * -90;
    sinTheta_ = sin(angle90_);

	//壁が無かった時
	//振り子の加速度変化
	angleSpeed_ += (w_ / wireRadius_ * sinTheta_)*0.016f; //1/60
    //最高速度で止めとく
    if (angleSpeed_ > MaxAngleSpeed)
    {
        angleSpeed_ = MaxAngleSpeed;
    }
    else if (angleSpeed_ < -MaxAngleSpeed)
    {
        angleSpeed_ = -MaxAngleSpeed;
    }

    //振り子の角度を変化
    angle_ += angleSpeed_ * 0.016f;
    if (angle_ < 0)
    {
        angle_ += 2 * math::PI;
    }
    else if (angle_ > 2 * math::PI)
    {
        angle_ -= 2 * math::PI;
    }

    //現在の向きを一旦保存しておく
    previousPhi_ = phi_;

    //さて、ここからΦ(向き)変化
    if (parent_->input_ != GSvector3::zero()) {

        //上側に行ってたら反転
        if (angle_ < math::PI)
        {
            pendulumInvert_ = -1;
        }
        else
        {
            pendulumInvert_ = 1;
        }

        float shakeX = parent_->GetInput().x;
        float shakeZ = parent_->GetInput().z;
        if (shakeX < 0) {
            sthickAngle_ = atanf(shakeZ / shakeX) + math::PI;
        }
        else if (shakeZ < 0) {
            sthickAngle_ = atanf(shakeZ / shakeX) + math::PI * 2;
        }
        else {
            sthickAngle_ = atanf(shakeZ / shakeX);
        }

        if (sthickAngle_ >= math::PI) {
            sthickAngleForPhi_ = sthickAngle_ - math::PI;
        }
        else {
            sthickAngleForPhi_ = sthickAngle_ + math::PI;
        }

        //マウスを振った角度とそこから180度回した角度、Φの角度に近い方に合わせないと反転する
        //まず下側(マウス振った向きが)
        if (sthickAngleForPhi_ < math::PI)
        {
            //手前の方が近い
            if (abs(phi_ - sthickAngleForPhi_) < abs(phi_ - (sthickAngleForPhi_ + math::PI)))
            {
                mSAfP_Near = sthickAngleForPhi_;
                angleSpeed_ -= ShakeLevel * 1/60*pendulumInvert_;
                //print("a");

            }
            else if (abs(phi_ - sthickAngleForPhi_) >= abs(phi_ - (sthickAngleForPhi_ + math::PI)))
            {
                //奥に向かって進んでる
                mSAfP_Near = sthickAngleForPhi_ + math::PI;
                angleSpeed_ += ShakeLevel * 1/60 * pendulumInvert_;

                //print("b");
            }
        }
        else//マウス振った向きが上側
        {
            //奥の方が近い
            if (abs(phi_ - sthickAngleForPhi_) < abs(phi_ - (sthickAngleForPhi_ - math::PI)))
            {
                //奥に向かって進んでる
                mSAfP_Near = sthickAngleForPhi_;
                angleSpeed_ -= ShakeLevel * 1/60 * pendulumInvert_;

            }
            else if (abs(phi_ - sthickAngleForPhi_) >= abs(phi_ - (sthickAngleForPhi_ - math::PI)))
            {
                mSAfP_Near = sthickAngleForPhi_ - math::PI;
                angleSpeed_ += ShakeLevel * 1/60 * pendulumInvert_;
            }
        }
        
    }


    //回りましょうよ
    phi_ += (mSAfP_Near - previousPhi_) * 1 / 60;


    //下に加速中に離しても上昇しない
    if (angleSpeedSpeed_ > 0)
    {
        if (velocity_.y > 0)
        {
            velocity_.y = 0;
        }
    }


    //直接変えちゃうのどうかと思うよなー
    //だから新しい座標から前の座標を引いた値を速度にする
    parent_->velocity(position_-playerPosition);

    // モーションの変更
    // 移動量のxy成分だけ更新
    //parent_->velocity(velocity_);

    cameraLookPoint_ = wirePosition_;
    cameraLookPoint_.y = wirePosition_.y - wireRadius_ * 0.5f;
    //注視点設定
    parent_->setCameraLookPoint(cameraLookPoint_);


    //振り子の向きを主人公が向いてる方向にする(これをやらないと正しく減衰しない)
    //振り子のベクトルを正規化する
    float norm = sqrtf(velocity_.x * velocity_.x + velocity_.z * velocity_.z);
    float mag = 1.0f / norm;
    my_Input_Direction_ = (velocity_ * mag);
    my_Input_Direction_.y = 0;
    parent_->SetInputDirection(my_Input_Direction_);


	return;




}
void PlayerPendulumState::lateUpdate()
{

}
// 描画
void PlayerPendulumState::draw() const
{
    GSvector2 startPoint = GSvector2::zero();
    GSvector2 endPoint = GSvector2::zero();
    GSvector3 handPosition = position_;
    handPosition.y += 1.5f; //手の位置のオフセット
    GSvector3 centerPosition = wirePosition_;

    //それぞれの座標を画面座標に変換
    gsCalculateScreen(&startPoint, &handPosition);
    gsCalculateScreen(&endPoint, &centerPosition);

    //画面座標がマイナスにならないようにする
    if (startPoint.x > 10000) {
        startPoint.x -= 20000.0f;
    }
    /*if (startPoint.y< 0) {
        startPoint.y += 1080.0f;
    }*/


    gsDrawText("s%f\n", startPoint.x);
    //gsDrawText("\ne%f", endPoint.x);

    //画像の中心決定
    static const GSvector2 center{16.0f,0.0f};
    //色
    GScolor color = { 1.0f, 1.0f, 1.0f, 1.0f };

    float nagasa = sqrtf((endPoint.y - startPoint.y) * (endPoint.y - startPoint.y) + (endPoint.x - startPoint.x) * (endPoint.x - startPoint.x));
    GSvector2 end_scale{ 0.5f, nagasa/32.0f };

    GSvector2 position = GSvector2(endPoint);

    //画像の切り抜き
    static const GSrect Rect{ 0.0f, 0.0f, 32.0f,32.0f };

    //角度決定
    float drawAngle = atan2f(endPoint.y - startPoint.y, endPoint.x - startPoint.x)*(180/math::PI)+90;

    gsDrawSprite2D(Texture_Wire, &position, &Rect, &center,  &color, &end_scale, drawAngle);
    //gsDrawSprite3D(Texture_Wire, &position_, &body, NULL, &color, &scale, 0.0f);
    
}
void PlayerPendulumState::lateDraw() const
{
}

void PlayerPendulumState::changeAngle() {
	//向きの補間
	GSquaternion rotation =
		GSquaternion::rotateTowards(
			parent_->transform().rotation(),
			GSquaternion::lookRotation(my_Input_Direction_), 12.0f);
	parent_->transform().rotation(rotation);
}


