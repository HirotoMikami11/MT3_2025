#include <Novice.h>
#include "MyMath.h"

const char kWindowTitle[] = "LE2A_15_ミカミ_ヒロト_MT3_00_00";

//1.透視射影行列
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip) {
	Matrix4x4 perspectiveFovMatrix = { 0 };

	perspectiveFovMatrix.m[0][0] = (1 / aspectRatio) * (1 / std::tan(fovY / 2));
	perspectiveFovMatrix.m[0][1] = 0;
	perspectiveFovMatrix.m[0][2] = 0;
	perspectiveFovMatrix.m[0][3] = 0;

	perspectiveFovMatrix.m[1][0] = 0;
	perspectiveFovMatrix.m[1][1] = (1 / (std::tan(fovY / 2)));
	perspectiveFovMatrix.m[1][2] = 0;
	perspectiveFovMatrix.m[1][3] = 0;

	perspectiveFovMatrix.m[2][0] = 0;
	perspectiveFovMatrix.m[2][1] = 0;
	perspectiveFovMatrix.m[2][2] = (farClip / (farClip - nearClip));
	perspectiveFovMatrix.m[2][3] = 1;

	perspectiveFovMatrix.m[3][0] = 0;
	perspectiveFovMatrix.m[3][1] = 0;
	perspectiveFovMatrix.m[3][2] = (-nearClip * farClip) / (farClip - nearClip);
	perspectiveFovMatrix.m[3][3] = 0;

	return perspectiveFovMatrix;
}


//2.正射影行列
Matrix4x4 MakeOrthograpicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip) {
	Matrix4x4 orthpgrapicMatrix = { 0 };

	orthpgrapicMatrix.m[0][0] = 2 / (right - left);
	orthpgrapicMatrix.m[0][1] = 0;
	orthpgrapicMatrix.m[0][2] = 0;
	orthpgrapicMatrix.m[0][3] = 0;

	orthpgrapicMatrix.m[1][0] = 0;
	orthpgrapicMatrix.m[1][1] = 2 / (top - bottom);
	orthpgrapicMatrix.m[1][2] = 0;
	orthpgrapicMatrix.m[1][3] = 0;

	orthpgrapicMatrix.m[2][0] = 0;
	orthpgrapicMatrix.m[2][1] = 0;
	orthpgrapicMatrix.m[2][2] = 1 / (farClip - nearClip);
	orthpgrapicMatrix.m[2][3] = 0;

	orthpgrapicMatrix.m[3][0] = (left + right) / (left - right);
	orthpgrapicMatrix.m[3][1] = (top + bottom) / (bottom - top);
	orthpgrapicMatrix.m[3][2] = nearClip / (nearClip - farClip);
	orthpgrapicMatrix.m[3][3] = 1;

	return orthpgrapicMatrix;

}
//3.ビューポート変換行列
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth) {
	Matrix4x4 viewportMatrix = { 0 };

	viewportMatrix.m[0][0] = width / 2;
	viewportMatrix.m[0][1] = 0;
	viewportMatrix.m[0][2] = 0;
	viewportMatrix.m[0][3] = 0;

	viewportMatrix.m[1][0] = 0;
	viewportMatrix.m[1][1] = -(height / 2);
	viewportMatrix.m[1][2] = 0;
	viewportMatrix.m[1][3] = 0;

	viewportMatrix.m[2][0] = 0;
	viewportMatrix.m[2][1] = 0;
	viewportMatrix.m[2][2] = maxDepth - minDepth;
	viewportMatrix.m[2][3] = 0;

	viewportMatrix.m[3][0] = left + (width / 2);
	viewportMatrix.m[3][1] = top + (height / 2);
	viewportMatrix.m[3][2] = minDepth;
	viewportMatrix.m[3][3] = 1;

	return viewportMatrix;
}
// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	int kRowHeight = 20;
	Matrix4x4 orthographicMatrix =
		MakeOrthograpicMatrix(-160.0f, 160.0f, 200.0f, 300.0f, 0.0f, 1000.0f);

	Matrix4x4 perspectiveFovMatrix =
		MakePerspectiveFovMatrix(0.63f, 1.33f, 0.1f, 1000.0f);

	Matrix4x4 viewportMatrix =
		MakeViewportMatrix(100.0f, 200.0f, 600.0f, 300.0f, 0.0f, 1.0f);


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


		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		
		MatrixScreenPrintf(0, 0, orthographicMatrix, "orthographicMatrix");
		MatrixScreenPrintf(0, kRowHeight * 5, perspectiveFovMatrix, "perspectiveFovMatrix");
		MatrixScreenPrintf(0, kRowHeight * 5 * 2, viewportMatrix, "viewportMatrix");

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
