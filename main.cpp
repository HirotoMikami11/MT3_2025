#include <Novice.h>
#include "MyFunction.h"
#include <imgui.h>
#include "Camera.h"
#include<string>
//FPS
#include"FrameTimer.h"

const char kWindowTitle[] = "LE2A_15_ミカミ_ヒロト_MT3_04_03";






// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	// FPS関連
	FrameTimer& frameTimer = FrameTimer::GetInstance();

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
