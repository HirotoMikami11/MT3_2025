#include <Novice.h>
#include "MyFunction.h"
#include <imgui.h>
#include "Camera.h"
#include<string>
//FPS
#include"FrameTimer.h"

const char kWindowTitle[] = "LE2A_15_ミカミ_ヒロト_MT3_04_01";

/// <summary>
/// 振り子
/// </summary>
struct Pendulum {
	Vector3 anchor;				//アンカーポイント
	float length;				//紐の長さ
	float angle;				//現在の角度
	float angularVelocity;		//角速度
	float angularAcceleration;	//各加速度
};

void DrawPendulum(Pendulum& pendulum, Ball& ball, const Matrix4x4& viewProjectionMatrix,
	const Matrix4x4& viewportMatrix, uint32_t color) {

	Vector3 springScreenPos = MakeScreenPositionToWorld(pendulum.anchor, viewProjectionMatrix, viewportMatrix);
	Vector3 ballScreenPos = MakeScreenPositionToWorld(ball.position, viewProjectionMatrix, viewportMatrix);
	Novice::DrawLine(static_cast<int>(springScreenPos.x), static_cast<int>(springScreenPos.y),
		static_cast<int>(ballScreenPos.x), static_cast<int>(ballScreenPos.y),
		color);

	DrawSphere({ ball.position,ball.radius }, viewProjectionMatrix, viewportMatrix, ball.color);
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };


	Camera* camera = new Camera();
	camera->Initialize();

	// FPS関連
	FrameTimer& frameTimer = FrameTimer::GetInstance();

	///振り子
	Pendulum pendulum;
	pendulum.anchor = { 0.0f,1.0f,0.0f };
	pendulum.length = 0.8f;
	pendulum.angle = 0.7f;
	pendulum.angularVelocity = 0.0f;
	pendulum.angularAcceleration = 0.0f;

	Ball ball{};
	ball.position.x = pendulum.anchor.x + std::sin(pendulum.angle) * pendulum.length;
	ball.position.y = pendulum.anchor.y - std::cos(pendulum.angle) * pendulum.length;
	ball.position.z = pendulum.anchor.z;
	ball.mass = 2.0f;
	ball.radius = 0.05f;
	ball.color = BLUE;

	//移動させるフラグ
	bool isMove = false;


	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		frameTimer.BeginFrame();

		///
		/// ↓更新処理ここから
		///





		//カメラの更新
		camera->Update(keys, preKeys);

		if (isMove) {
		//振り子の角度を計算する
		pendulum.angularAcceleration = -(9.0f / pendulum.length) * std::sinf(pendulum.angle);
		pendulum.angularVelocity += pendulum.angularAcceleration * frameTimer.GetDeltaTime();
		pendulum.angle += pendulum.angularVelocity * frameTimer.GetDeltaTime();

		//振り子の先端の位置にボールの座標を置く
		ball.position.x = pendulum.anchor.x + std::sin(pendulum.angle) * pendulum.length;
		ball.position.y = pendulum.anchor.y -std::cos(pendulum.angle) * pendulum.length;
		ball.position.z = pendulum.anchor.z;
		}







		///*	ImGUI	 *///

		ImGui::Begin("debug");
		frameTimer.ImGui();
		ImGui::Separator();	//線
		//アクションを起こす
		if (ImGui::Button("isAction")) {
			isMove = !isMove;
		}

		ImGui::End();

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		//グリッド線を描画
		DrawGrid(camera->GetViewProjectionMatrix(), camera->GetViewportMatrix());
		DrawPendulum(pendulum, ball, camera->GetViewProjectionMatrix(), camera->GetViewportMatrix(),WHITE);

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}


	delete	camera;

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
