#include <Novice.h>
#include "MyFunction.h"
#include <imgui.h>
#include "Camera.h"
#include<string>


const char kWindowTitle[] = "LE2A_15_ミカミ_ヒロト_MT3_03_00";


//3次元空間上で２次ベジェ曲線上の点を求める関数
Vector3 Bezier(const Vector3& p0, const Vector3& p1, const Vector3& p2, float t) {

	//制御点p0,p1を線形補間
	Vector3 p01 = Lerp(p0, p1, t);
	//制御点p1,p2を線形補間
	Vector3 p12 = Lerp(p1, p2, t);
	//制御点p0p1,p1p2をさらに線形補間
	Vector3 result = Lerp(p01, p12, t);

	return result;
}


/// <summary>
/// ベジエ曲線を描画する関数
/// </summary>
/// <param name="controlPoint0">制御点0</param>
/// <param name="controlPoint1">制御点1</param>
/// <param name="controlPoint2">制御点2</param>
/// <param name="subdivision">分割数</param>
/// <param name="viewProjectionMatrix">ビュープロジェクション</param>
/// <param name="viewportMatrix">ビューポート</param>
/// <param name="color">色</param>
void DrawBezier(
	const Vector3& controlPoint0,
	const Vector3& controlPoint1,
	const Vector3& controlPoint2,
	const int subdivision,
	const Matrix4x4& viewProjectionMatrix,
	const Matrix4x4& viewportMatrix,
	uint32_t color)
{
	// 前の点を保持するための変数
	Vector3 previousScreenPos = { 0,0,0 };
	bool hasPreviousPoint = false;
	for (int i = 0; i <= subdivision; ++i) {
		//tの値を求める
		float t = static_cast<float>(i) / static_cast<float>(subdivision);
		//ベジエ曲線上の点を求める
		Vector3 point = Bezier(controlPoint0, controlPoint1, controlPoint2, t);
		//スクリーン座標系に変換
		Vector3 ndcPos = Transform(point, viewProjectionMatrix);
		Vector3 screenPos = Transform(ndcPos, viewportMatrix);
		//描画
		if (hasPreviousPoint) {
			Novice::DrawLine(
				static_cast<int>(previousScreenPos.x),
				static_cast<int>(previousScreenPos.y),
				static_cast<int>(screenPos.x),
				static_cast<int>(screenPos.y),
				color);
		}
		// 現在の点を次のループの前の点として保存
		previousScreenPos = screenPos;
		hasPreviousPoint = true;
	}
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

	///ベジエ曲線の制御点
	Vector3 controlPoints[3] = {
		{ -0.8f, 0.58f, 1.0f }, // 制御点1
		{ 1.76f, 1.0f, -0.3f }, // 制御点2
		{ 0.94f, -0.7f, 2.3f }  // 制御点3
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

		ImGui::Begin("debug");
		for (int i = 0; i < 3; i++)
		{
			// 制御点のラベルを動的に生成
			std::string label = "Control Point " + std::to_string(i);
			ImGui::DragFloat3(label.c_str(), &controlPoints[i].x, 0.05f);

		}

		ImGui::End();

		//グリッド線を描画
		DrawGrid(camera->GetViewProjectionMatrix(), camera->GetViewportMatrix());


		///各制御点の描画

		for (int i = 0; i < 3; i++)
		{
			uint32_t color = 0xFF0000FF; // 赤色
			if (i == 1) {
				color = 0x00FF00FF; // 緑色
			} else if (i == 2) {
				color = 0x0000FFFF; // 青色
			}

			DrawSphere(
				{ controlPoints[i], 0.01f },
				camera->GetViewProjectionMatrix(),
				camera->GetViewportMatrix(),
				color); // 赤色
		}

		DrawBezier(
			controlPoints[0],
			controlPoints[1],
			controlPoints[2],
			32, // 分割数
			camera->GetViewProjectionMatrix(),
			camera->GetViewportMatrix(),
			0xFFFFFFFF); // 白色

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
