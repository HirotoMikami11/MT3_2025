#include <Novice.h>
#include "MyFunction.h"
#include <imgui.h>

const char kWindowTitle[] = "LE2A_15_ミカミ_ヒロト_MT3_02_00";

/// <summary>
/// 直線
/// </summary>
struct Line {
	Vector3 origin;		//始点
	Vector3 diff;		//終点への差分ベクトル
};

/// <summary>
/// 半直線
/// </summary>
struct Ray {
	Vector3 origin;		//始点
	Vector3 diff;		//終点への差分ベクトル
};

/// <summary>
/// 線分
/// </summary>
struct Segment {
	Vector3 origin;		//始点
	Vector3 diff;		//終点への差分ベクトル
};

//	正射影ベクトルを求める関数
//	
Vector3 Project(const Vector3& v1, const Vector3& v2) {
	Vector3 project = Vector3Multiply(Vector3Normalize(v2), Vector3Dot(v1, Vector3Normalize(v2)));
	return project;
}
//　最近接点を求める関数
Vector3 ClosestPoint(const Vector3& point, const Segment& segment) {
	Vector3 project = Project(Vector3Subtract(point,segment.origin),segment.diff);
	Vector3 closestPoint =Vector3Add(segment.origin,project);

	return closestPoint;
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	int kWindowWidth = 1280;
	int kWindowHeight = 720;

	Vector3 cameraTranslate{ 0.0f,1.9f,-6.49f };
	Vector3 cameraRotate{ 0.26f,0.0f,0.0f };

	//計算に使用する関数
	Segment segment{ {-2.0f,-1.0f,0.0f},{3.0f,2.0f,2.0f} };
	Vector3 point{ -1.5f,0.6f,0.6f };

	Vector3 project = Project(Vector3Subtract(point, segment.origin), segment.diff);
	Vector3 closestPoint = ClosestPoint(point, segment);

	//描画に使用する関数
	Sphere pointSphere{ point,0.01f };		//1cmの球を描画
	Sphere closestPointSphere{ closestPoint,0.01f };		//1cmの球を描画


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

		Matrix4x4 cameraMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, cameraRotate, cameraTranslate);
		Matrix4x4 viewMatrix = Matrix4x4Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 viewportMatrix = MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);
		Matrix4x4 viewProjectionMatrix = Matrix4x4Multiply(viewMatrix, projectionMatrix);

		
		project = Project(Vector3Subtract(point, segment.origin), segment.diff);
		closestPoint = ClosestPoint(point, segment);
		//描画に使用する関数
		pointSphere={ point,0.01f };		//1cmの球を描画
		closestPointSphere={ closestPoint,0.01f };		//1cmの球を描画

		//線分の両端をそれぞれスクリーン座標に変換
		Vector3 start = Transform(Transform(segment.origin, viewProjectionMatrix), viewportMatrix);
		Vector3 end = Transform(Transform(Vector3Add(segment.origin, segment.diff), viewProjectionMatrix), viewportMatrix);



		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(viewProjectionMatrix, viewportMatrix);

		ImGui::Begin("Window");
		//カメラ
		ImGui::DragFloat3("CameraTranslate", &cameraTranslate.x, 0.01f);
		ImGui::DragFloat3("CameraRotate", &cameraRotate.x, 0.01f);
		
		//カメラを初期状態に戻す
		if (ImGui::Button("Reset Camera")) {
			cameraTranslate={ 0.0f,1.9f,-6.49f };
			cameraRotate={ 0.26f,0.0f,0.0f };
		}
		
		//線分への正射影ベクトルの結果
		ImGui::InputFloat3("Project", &project.x, "%.3f", ImGuiInputTextFlags_ReadOnly);

		ImGui::DragFloat3("point", &point.x, 0.01f);
		ImGui::DragFloat3("segment.origin", &segment.origin.x, 0.01f);
		ImGui::DragFloat3("segment.diff", &segment.diff.x, 0.01f);


		ImGui::End();


		//球の描画
		DrawSphere(pointSphere, viewProjectionMatrix, viewportMatrix, RED);
		DrawSphere(closestPointSphere, viewProjectionMatrix, viewportMatrix, BLACK);

		//線分の描画
		Novice::DrawLine(
			static_cast<int>(start.x),
			static_cast<int>(start.y),
			static_cast<int>(end.x),
			static_cast<int>(end.y),
			WHITE);


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

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
