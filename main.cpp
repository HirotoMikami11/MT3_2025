#include <Novice.h>
#include "MyMath.h"

const char kWindowTitle[] = "LE2A_15_ミカミ_ヒロト_MT3_01_01";

//クロス積
Vector3 Cross(const Vector3& v1, const Vector3& v2) {
	Vector3 result = {
	(v1.y * v2.z) - (v1.z * v2.y),
	(v1.z * v2.x) - (v1.x * v2.z),
	(v1.x * v2.y) - (v1.y * v2.x) };
	return result;
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

	///クロス積確認用の変数
	Vector3 v1 = { 1.2f,-3.9f,2.5f };
	Vector3 v2 = { 2.8f,0.4f,-1.3f };
	Vector3 cross = Cross(v1, v2);

	///三角形用の変数
	Vector3 rotate = { 0 };
	Vector3 translate = { 0 };
	//カメラが三角形より後ろにいるようにz-50
	Vector3 cameraPosition = { 0,0,-50.0f };
	Vector3 kLocalVertices[3] = {
		0.0f,5.0f,0.0f,
		5.0f,-5.0f,0.0f,
		-5.0f,-5.0f,0.0f,

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

		//WSキーで前後、ADキーで左右に三角形を動かす。
		if ( keys[DIK_W]) {
			translate.z ++;
		}
		if ( keys[DIK_S]) {
			translate.z --;
		}

		if (keys[DIK_A]) {
			translate.x --;
		}
		if (keys[DIK_D]) {
			translate.x ++;
		}
		//Y軸回転させる
		rotate.y += 0.05f;

		///各行列の計算
		Matrix4x4 worldMatrix = MakeAffineMatrix({ 1.0f,1.0f, 1.0f }, rotate, translate);
		Matrix4x4 cameraMatrix = MakeAffineMatrix({ 1.0f,1.0f, 1.0f }, { 0.0f,0.0f,0.0f }, cameraPosition);
		Matrix4x4 viewMatrix = Matrix4x4Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 worldViewProjectionMatrix = Matrix4x4Multiply(worldMatrix, Matrix4x4Multiply(viewMatrix, projectionMatrix));
		Matrix4x4 viewportMatrix = MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);

		///スクリーン上の３点を計算
		Vector3 screenVertices[3];
		for (uint32_t i = 0; i < 3; ++i) {
			Vector3 ndcVertex = Transform(kLocalVertices[i], worldViewProjectionMatrix);
			screenVertices[i] = Transform(ndcVertex, viewportMatrix);
		}


		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///


		VectorScreenPrintf(0, 0, cross, "Cross");
		
		Novice::DrawTriangle(
			static_cast<int>(screenVertices[0].x), static_cast<int>(screenVertices[0].y),
			static_cast<int>(screenVertices[1].x), static_cast<int>(screenVertices[1].y),
			static_cast<int>(screenVertices[2].x), static_cast<int>(screenVertices[2].y),
			RED, kFillModeSolid
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

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
