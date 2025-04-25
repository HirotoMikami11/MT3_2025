#include <Novice.h>
#include "MyFunction.h"
#include <imgui.h>
#include "Camera.h"

const char kWindowTitle[] = "LE2A_15_ミカミ_ヒロト_MT3_02_01";

//　球と球の衝突判定
bool IsCollision(const Sphere& sphere1, const Sphere& sphere2) {

	//2つの球の中心転換の距離を求める
	float distance = Vector3Length(Vector3Subtract(sphere1.center, sphere2.center));

	//半径の合計より短ければ衝突
	if (distance <= sphere1.radius + sphere2.radius) {
		return true;
	}

	return false;
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

	Sphere sphere[2];
	sphere[0] = { {0.0f, 0.0f, 0.0f},0.6f };
	sphere[1] = { {0.8f, 0.0f, 1.0f},0.4f };


	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);



		///
		/// ↓更新処理ここから
		///

		//カメラの更新
		camera->Update(keys,preKeys);



		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		ImGui::Begin("Window");
		for (int i = 0; i < 2; i++){
			std::string labelCenter = std::format("sphere[{}].center", i);
			std::string labelRadius = std::format("sphere[{}].radius", i);
			ImGui::DragFloat3(labelCenter.c_str(), &sphere[i].center.x, 0.01f);
			ImGui::DragFloat(labelRadius.c_str(), &sphere[i].radius, 0.01f);
		}
		ImGui::End();

		//グリッド線を描画
		DrawGrid(camera->GetViewProjectionMatrix(), camera->GetViewportMatrix());

		for (int i = 0; i < 2; i++)
		{
			//球体を描画
			DrawSphere(sphere[i], camera->GetViewProjectionMatrix(), camera->GetViewportMatrix(), IsCollision(sphere[0], sphere[1]) ? RED : WHITE);

		}

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
