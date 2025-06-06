#include <Novice.h>
#include "MyFunction.h"
#include <imgui.h>
#include "Camera.h"
#include<string>


const char kWindowTitle[] = "LE2A_15_ミカミ_ヒロト_MT3_03_01";

/// <summary>
/// parts
/// </summary>
enum class parts {
	Shoulder,		//肩
	Elbow,			//肘
	Hands			//手
};


/// <summary>
/// 階層構造の変換行列を計算
/// </summary>
void UpdateHierarchy(
	const Vector3 translates[],
	const Vector3 rotates[],
	const Vector3 scales[],
	Matrix4x4 worldMatrices[],
	int partCount
) {

	//partsの数だけ回す
	for (int i = 0; i < partCount; i++) {
		
		// ローカル変換行列を作成
		Matrix4x4 scaleMatrix = MakeScaleMatrix(scales[i]);
		Matrix4x4 rotateMatrix = MakeRotateXYZMatrix(rotates[i]);
		Matrix4x4 translateMatrix = MakeTranslateMatrix(translates[i]);

		// SRT順でローカルのアフィン行列
		Matrix4x4 localMatrix = Matrix4x4Multiply(scaleMatrix, Matrix4x4Multiply(rotateMatrix, translateMatrix));

		///iでそれが根元(親)かどうか判定する
		if (i == 0) {
			// 親がいない場合はローカル変換がそのままワールド変換
			worldMatrices[i] = localMatrix;
		} else {
			// 親がいる場合は親のワールド変換を適用
			worldMatrices[i] = Matrix4x4Multiply(localMatrix, worldMatrices[i-1]);
		}
	}
}

/// <summary>
/// 階層構造を描画
/// </summary>
/// <param name="worldMatrices">ワールド変換行列配列</param>
/// <param name="colors">各パーツの色配列</param>
/// <param name="partCount">パーツ数</param>
/// <param name="viewProjectionMatrix">ビュープロジェクション行列</param>
/// <param name="viewportMatrix">ビューポート行列</param>
/// <param name="sphereRadius">球体の半径</param>
void DrawHierarchy(
	const Matrix4x4 worldMatrices[],
	const uint32_t colors[],
	int partCount,
	const Matrix4x4& viewProjectionMatrix,
	const Matrix4x4& viewportMatrix,
	float sphereRadius = 0.05f
) {

	for (int i = 0; i < partCount; i++) {
		// 各パーツの位置を取得（ワールド変換行列の平行移動成分）
		Vector3 worldPosition = {
			worldMatrices[i].m[3][0],
			worldMatrices[i].m[3][1],
			worldMatrices[i].m[3][2]
		};

		// 球体描画
		DrawSphere(
			{ worldPosition, sphereRadius },
			viewProjectionMatrix,
			viewportMatrix,
			colors[i]);

		// 親との線を描画（階層構造の可視化）
		if (i != 0) {
			//親の座標を求める
			Vector3 parentPosition = {
				worldMatrices[i-1].m[3][0],
				worldMatrices[i-1].m[3][1],
				worldMatrices[i-1].m[3][2]
			};

			// スクリーン座標に変換
			Vector3 ndcPos1 = Transform(worldPosition, viewProjectionMatrix);
			Vector3 ndcPos2 = Transform(parentPosition, viewProjectionMatrix);

			Vector3 screenPos1 = Transform(ndcPos1, viewportMatrix);
			Vector3 screenPos2 = Transform(ndcPos2, viewportMatrix);

			// 線を描画
			Novice::DrawLine(
				static_cast<int>(screenPos1.x),
				static_cast<int>(screenPos1.y),
				static_cast<int>(screenPos2.x),
				static_cast<int>(screenPos2.y),
				0xFFFFFFFF  // 白色
			);
		}
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


	// 各パーツのローカル変換
	Vector3 translates[3] = {
		{0.2f, 1.0f, 0.0f},		// Shoulder
		{0.4f, 0.0f, 0.0f},		// Elbow
		{0.3f, 0.0f, 0.0f}		// Hands
	};

	Vector3 rotates[3] = {
		{0.0f, 0.0f, -6.8f},	// Shoulder
		{0.0f, 0.0f, -1.4f},	// Elbow
		{0.0f, 0.0f, 0.0f}		// Hands
	};

	Vector3 scales[3] = {
		{1.0f, 1.0f, 1.0f},		// Shoulder
		{1.0f, 1.0f, 1.0f},		// Elbow
		{1.0f, 1.0f, 1.0f}		// Hands
	};

	//ワールド座標
	Matrix4x4 worldMatrices[3];
	//それぞれのパーツの色
	uint32_t colors[3] = {
		0xFF0000FF,  // Shoulder: 赤
		0x00FF00FF,  // Elbow: 緑
		0x0000FFFF   // Hands: 青
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
		//階層構造の更新
		UpdateHierarchy(translates, rotates, scales, worldMatrices, 3);


		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		ImGui::Begin("debug");

		for (int i = 0; i < 3; i++) {
			ImGui::PushID(i);  // ID衝突を避ける?
			std::string label = "Control Point " + std::to_string(i);
			ImGui::Text(label.c_str());
			// 座標
			ImGui::DragFloat3("Position", &translates[i].x, 0.01f);
			//回転
			ImGui::DragFloat3("Rotation", &rotates[i].x, 0.01f);
			// スケール
			ImGui::DragFloat3("Scale", &scales[i].x, 0.01f, 0.1f, 5.0f);

		}

		ImGui::End();

		//グリッド線を描画
		DrawGrid(camera->GetViewProjectionMatrix(), camera->GetViewportMatrix());


		//階層構造の描画
		DrawHierarchy(worldMatrices, colors, 3,
			camera->GetViewProjectionMatrix(),
			camera->GetViewportMatrix()
		);


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
