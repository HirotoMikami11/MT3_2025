#include <Novice.h>
#include "MyFunction.h"
#include <imgui.h>
#include "Camera.h"

const char kWindowTitle[] = "LE2A_15_ミカミ_ヒロト_MT3_02_03";


bool IsCollision(const Segment& segment, const Plane& plane) {
	//衝突しているかどうか
	bool isCollision = false;

	//垂直判定を行うため、法線と線の内積を求める
	float dot = Vector3Dot(plane.normal, segment.diff);
	//垂直=平行であるので、衝突していない
	if (dot == 0.0f) {
		return false;
	}

	//tを求める
	float t = (plane.distance - Vector3Dot(segment.origin, plane.normal)) / dot;

	//tの値と線の種類によって衝突しているかを判定する
	//線分なので0~1
	if (t >= 0 && t <= 1) {
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


	Plane plane_ = { {0,1,0},1.0f };
	Segment segment_ = { 
		{-0.45f,0.7f,0.0f},	//始点
		{1.0f,0.5f,0.0f}	//終点
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
		ImGui::SliderFloat3("plane_Normal", &plane_.normal.x, -1.0f, 1.0f);
		plane_.normal = Vector3Normalize(plane_.normal);
		ImGui::SliderFloat("plane_Distance", &plane_.distance, 0.0f, 1.0f);

		ImGui::SliderFloat3("segment_Origin", &segment_.origin.x, 0.0f, 1.0f);
		ImGui::SliderFloat3("segment_Diff", &segment_.diff.x, 0.0f, 1.0f);
		ImGui::End();

		//グリッド線を描画
		DrawGrid(camera->GetViewProjectionMatrix(), camera->GetViewportMatrix());
		DrawPlane(plane_, camera->GetViewProjectionMatrix(), camera->GetViewportMatrix(), WHITE);
		//ライン
		DrawLine(segment_, camera->GetViewProjectionMatrix(), camera->GetViewportMatrix(), IsCollision(segment_, plane_) ? RED : WHITE);

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
