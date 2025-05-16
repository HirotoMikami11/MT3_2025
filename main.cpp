#include <Novice.h>
#include "MyFunction.h"
#include <imgui.h>
#include "Camera.h"

const char kWindowTitle[] = "LE2A_15_ミカミ_ヒロト_MT3_02_02";

/// <summary>
/// 平面
/// </summary>
struct Plane {
	Vector3 normal;	//法線
	float distance;//距離

};


// 垂直なベクトルを求める
Vector3 Perpendicular(const Vector3& vector) {
	if (vector.x != 0.0f || vector.y != 0.0f) {
		return { -vector.y, vector.x, 0.0f };
	}
	return { 0.0f, -vector.z, vector.y };
}


//平面の描画
void DrawPlane(const Plane& plane, const Matrix4x4 viewProjectionMatrix, const Matrix4x4 viewportMatrix, uint32_t color) {
	//1.平面の中心点を決める
	Vector3 center = Vector3Multiply(plane.normal, plane.distance);

	Vector3 perpendiculars[4];
	//2.法線と垂直なベクトルを求める
	perpendiculars[0] = Vector3Normalize(Perpendicular(plane.normal));

	//3.2の逆ベクトルを求める
	perpendiculars[1] = { -perpendiculars[0].x,-perpendiculars[0].y,-perpendiculars[0].z };

	//4.2の法線とのクロス積を求める
	perpendiculars[2] = Cross(plane.normal, perpendiculars[0]);

	//5.4の逆ベクトルを求める
	perpendiculars[3] = { -perpendiculars[2].x,-perpendiculars[2].y,-perpendiculars[2].z };

	//6.2~5のベクトルを中心点に、定数倍して足して4頂点を出す
	Vector3 points[4];

	for (int32_t index = 0; index < 4; ++index) {
		Vector3 extend = Vector3Multiply(perpendiculars[index], 2.0f);
		Vector3 point = Vector3Add(center, extend);
		points[index] = Transform(Transform(point, viewProjectionMatrix), viewportMatrix);
	}
	//描画
	Novice::DrawLine(static_cast<int>(points[0].x), static_cast<int>(points[0].y),
		static_cast<int>(points[3].x), static_cast<int>(points[3].y), color);
	Novice::DrawLine(static_cast<int>(points[0].x), static_cast<int>(points[0].y),
		static_cast<int>(points[2].x), static_cast<int>(points[2].y), color);
	Novice::DrawLine(static_cast<int>(points[1].x), static_cast<int>(points[1].y),
		static_cast<int>(points[2].x), static_cast<int>(points[2].y), color);
	Novice::DrawLine(static_cast<int>(points[1].x), static_cast<int>(points[1].y),
		static_cast<int>(points[3].x), static_cast<int>(points[3].y), color);

}

//球と平面の衝突判定
bool IsCollision(const Sphere& sphere, const Plane& plane) {
	
	//1.点と平面との距離
	//そのままだと符号付き距離になってしまうので、絶対値(abs)を取る
	float distance = std::abs(Vector3Dot(plane.normal, sphere.center) - plane.distance);
	
	//2.1の距離<=球の半径なら衝突
	if (distance <= sphere.radius) {
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

	Sphere sphere_ = { {0.12f,0,0},0.6f };
	Plane plane_ = { {0,1,0},1.0f };

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
		ImGui::SliderFloat3("Sphere Center", &sphere_.center.x, -1.0f, 1.0f);
		ImGui::SliderFloat3("Plane Center", &plane_.normal.x, -1.0f, 1.0f);
		ImGui::SliderFloat("Sphere Radius", &sphere_.radius, 0.0f, 1.0f);
		ImGui::SliderFloat("Plane Radius", &plane_.distance, 0.0f, 1.0f);


		ImGui::End();

		//グリッド線を描画
		DrawGrid(camera->GetViewProjectionMatrix(), camera->GetViewportMatrix());


		DrawSphere(sphere_, camera->GetViewProjectionMatrix(), camera->GetViewportMatrix(), IsCollision(sphere_, plane_) ? RED : WHITE);
		DrawPlane(plane_, camera->GetViewProjectionMatrix(), camera->GetViewportMatrix(), WHITE);

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
