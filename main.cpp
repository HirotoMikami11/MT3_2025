#include <Novice.h>
#include "MyFunction.h"
#include <imgui.h>
#include "Camera.h"
#include<string>
#include<algorithm>

const char kWindowTitle[] = "LE2A_15_ミカミ_ヒロト_MT3_02_06";

bool IsCollision(const AABB& aabb, Sphere& sphere) {
	bool isCollision = false;

	//球の中心座標がAABBの[min,max]内にclampすれば、それが最近接点になる
	Vector3 closestPoint{
		std::clamp(sphere.center.x,aabb.min.x,aabb.max.x),
		std::clamp(sphere.center.y,aabb.min.y,aabb.max.y),
		std::clamp(sphere.center.z,aabb.min.z,aabb.max.z),
	};

	//最近接点と球の中心との距離を求める
	float distance = Vector3Length(Vector3Subtract(closestPoint, sphere.center));
	//距離が半径より小さければ衝突
	if (distance <= sphere.radius) {
		isCollision = true;
	}
	return isCollision;

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

	AABB aabb1{
	.min{-0.5f,-0.5f,-0.5f},
	.max{0.0f,0.0f,0.0f}
	};

	Sphere sphere{
		.center{1.0f,1.0f,1.0f},
		.radius {1.0f}
	};

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
		camera->Update(keys, preKeys);



		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		ImGui::Begin("Window");

		ImGui::DragFloat3("aabb1.max", &aabb1.max.x, 0.05f);
		ImGui::DragFloat3("aabb1.min", &aabb1.min.x, 0.05f);
		ImGui::DragFloat3("sphere.center", &sphere.center.x, 0.05f);
		ImGui::DragFloat("sphere.radius", &sphere.radius, 0.05f);

		//AABBのminmaxを正常値に維持する
		FixAABBMinMax(aabb1);

		ImGui::End();






		//グリッド線を描画
		DrawGrid(camera->GetViewProjectionMatrix(), camera->GetViewportMatrix());
		DrawAABB(aabb1, camera->GetViewProjectionMatrix(), camera->GetViewportMatrix(), IsCollision(aabb1, sphere) ? RED : WHITE);
		DrawSphere(sphere, camera->GetViewProjectionMatrix(), camera->GetViewportMatrix(), IsCollision(aabb1, sphere) ? RED : WHITE);

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
