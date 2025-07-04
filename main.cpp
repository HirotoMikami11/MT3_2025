#include <Novice.h>
#include "MyFunction.h"
#include <imgui.h>
#include "Camera.h"
#include<string>
//FPS
#include"FrameTimer.h"

const char kWindowTitle[] = "LE2A_15_ミカミ_ヒロト_MT3_04_04";

Vector3 Reflect(const Vector3& Input, const Vector3& normal) {
	// 法線ベクトルに対する反射ベクトルを計算
	return Input - 2.0f * Vector3Dot(Input, normal) * normal;
}



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


	Plane plane;
	plane.normal = Vector3Normalize({ -0.2f,0.9f,-0.3f });
	plane.distance = 0.0f;

	Ball ball{};
	ball.position = { 0.8f,1.2f,0.3f };
	ball.mass = 2.0f;
	ball.radius = 0.05f;
	ball.acceleration = { 0.0f, -9.8f, 0.0f }; // 重力加速度を設定
	ball.color = BLUE;

	float e = 0.5f; // 反発係数

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


			ball.velocity += ball.acceleration * frameTimer.GetDeltaTime(); // 加速度を速度に加える
			ball.position += ball.velocity * frameTimer.GetDeltaTime(); // 速度を位置に加える

			if (IsCollision(Sphere{ ball.position,ball.radius }, plane)) {
				Vector3 reflected = Reflect(ball.velocity, plane.normal); // 反射ベクトルを計算
				Vector3 projectToNormal = Project(reflected, plane.normal); // 法線方向への射影
				Vector3 moovingDirection = reflected - projectToNormal; // 法線方向を除いた移動方向
				ball.velocity = projectToNormal * e + moovingDirection; // 反射ベクトルに法線方向を加える
			}


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

		//リセットする
		if (ImGui::Button("isReset")) {

			ball.position = { 0.8f,1.2f,0.3f };
			ball.mass = 2.0f;
			ball.radius = 0.05f;
			ball.acceleration = { 0.0f, -9.8f, 0.0f }; // 重力加速度を設定
			ball.velocity = { 0.0f, 0.0f, 0.0f }; // 重力加速度を設定
			ball.color = BLUE;
		}



		ImGui::SliderFloat3("Plane Center", &plane.normal.x, -1.0f, 1.0f);
		ImGui::SliderFloat("Plane Radius", &plane.distance, 0.0f, 1.0f);

		ImGui::End();

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(camera->GetViewProjectionMatrix(), camera->GetViewportMatrix());
		DrawSphere({ ball.position,ball.radius }, camera->GetViewProjectionMatrix(), camera->GetViewportMatrix(), WHITE);
		DrawPlane(plane, camera->GetViewProjectionMatrix(), camera->GetViewportMatrix(), WHITE);

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
