#include <Novice.h>
#include "MyFunction.h"
#include <imgui.h>
#include "Camera.h"
#include<string>
//FPS
#include"FrameTimer.h"

const char kWindowTitle[] = "LE2A_15_ミカミ_ヒロト_MT3_04_03";
// FPS関連
FrameTimer& frameTimer = FrameTimer::GetInstance();

/// <summary>
/// 円錐振り子
/// </summary>
struct ConicalPendulum {
	Vector3 anchor;			//アンカーポイント	
	float length;			//紐の長さ
	float halfApexAngle;	//頂角の半分
	float angle;			//現在の角度
	float angularVelocity;	//角速度
};

/// <summary>
/// 円錐振り子の描画関数
/// </summary>
/// <param name="pendulum"></param>
/// <param name="ball"></param>
/// <param name="viewProjectionMatrix"></param>
/// <param name="viewportMatrix"></param>
/// <param name="color"></param>
void DrawPendulum(ConicalPendulum& pendulum, Ball& ball, const Matrix4x4& viewProjectionMatrix,
	const Matrix4x4& viewportMatrix, uint32_t color) {

	Vector3 pendulumScreenPos = MakeScreenPositionToWorld(pendulum.anchor, viewProjectionMatrix, viewportMatrix);
	Vector3 ballScreenPos = MakeScreenPositionToWorld(ball.position, viewProjectionMatrix, viewportMatrix);
	Novice::DrawLine(static_cast<int>(pendulumScreenPos.x), static_cast<int>(pendulumScreenPos.y),
		static_cast<int>(ballScreenPos.x), static_cast<int>(ballScreenPos.y),
		color);

	DrawSphere({ ball.position,ball.radius }, viewProjectionMatrix, viewportMatrix, ball.color);
}


void UpdatePendulum(ConicalPendulum& pendulum, Ball& ball) {
	//振り子の角度を計算する
	pendulum.angularVelocity = std::sqrt(9.8f / (pendulum.length * std::cos(pendulum.halfApexAngle)));
	pendulum.angle += pendulum.angularVelocity * frameTimer.GetDeltaTime();


	float radius = std::sin(pendulum.halfApexAngle) * pendulum.length;	//円の半径
	float height = std::cos(pendulum.halfApexAngle) * pendulum.length;	//高さ

	ball.position.x = pendulum.anchor.x + std::cos(pendulum.angle) * radius;
	ball.position.y = pendulum.anchor.y - height;	//-をつけるのはY軸が上方向だから
	ball.position.z = pendulum.anchor.z + std::sin(pendulum.angle) * radius;
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




	ConicalPendulum conicalPendulum;

	conicalPendulum.anchor = { 0.0f, 1.0f, 0.0f };	//アンカーポイント
	conicalPendulum.length = 0.8f;					//紐の長さ
	conicalPendulum.halfApexAngle = 0.7f;			//頂角の半分
	conicalPendulum.angle = 0.0f;					//角度
	conicalPendulum.angularVelocity = 0.0f;			//角速度

	Ball ball{};
	ball.position = { 0.0f,0.0f,0.0f };
	ball.mass = 2.0f;
	ball.radius = 0.05f;
	ball.color = BLUE;
	ball.radius = 0.05f;
	ball.color = BLUE;
	//最初の位置にするため一同更新
	UpdatePendulum(conicalPendulum, ball);
	//動くかどうかのフラグ
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
			UpdatePendulum(conicalPendulum, ball);
		};

		///*	ImGUI	 *///

		ImGui::Begin("debug");
		frameTimer.ImGui();
		ImGui::Separator();	//線
		//アクションを起こす
		if (ImGui::Button("isAction")) {
			isMove = !isMove;
		}
		ImGui::Text("isMove : %s", isMove ? "true" : "false");

		ImGui::End();

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(camera->GetViewProjectionMatrix(), camera->GetViewportMatrix());

		DrawPendulum(conicalPendulum, ball, camera->GetViewProjectionMatrix(), camera->GetViewportMatrix(), WHITE);

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
