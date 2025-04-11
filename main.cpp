#include <Novice.h>
#include "MyMath.h"
#include <cmath>

const char kWindowTitle[] = "LE2A_15_ミカミ_ヒロト_MT3_00_04";

//1.X軸回転行列
Matrix4x4 MakeRotateXMatrix(float radian) {
	Matrix4x4 RotateXMatrix = { 0 };
	RotateXMatrix.m[0][0] = 1;
	RotateXMatrix.m[0][1] = 0;
	RotateXMatrix.m[0][2] = 0;
	RotateXMatrix.m[0][3] = 0;

	RotateXMatrix.m[1][0] = 0;
	RotateXMatrix.m[1][1] = std::cos(radian);
	RotateXMatrix.m[1][2] = std::sin(radian);
	RotateXMatrix.m[1][3] = 0;

	RotateXMatrix.m[2][0] = 0;
	RotateXMatrix.m[2][1] = -std::sin(radian);
	RotateXMatrix.m[2][2] = std::cos(radian);
	RotateXMatrix.m[2][3] = 0;

	RotateXMatrix.m[3][0] = 0;
	RotateXMatrix.m[3][1] = 0;
	RotateXMatrix.m[3][2] = 0;
	RotateXMatrix.m[3][3] = 1;
	return RotateXMatrix;
}
//2.Y軸回転行列
Matrix4x4 MakeRotateYMatrix(float radian) {
	Matrix4x4 RotateYMatrix = { 0 };
	RotateYMatrix.m[0][0] = std::cos(radian);
	RotateYMatrix.m[0][1] = 0;
	RotateYMatrix.m[0][2] = -std::sin(radian);
	RotateYMatrix.m[0][3] = 0;

	RotateYMatrix.m[1][0] = 0;
	RotateYMatrix.m[1][1] = 1;
	RotateYMatrix.m[1][2] = 0;
	RotateYMatrix.m[1][3] = 0;

	RotateYMatrix.m[2][0] = std::sin(radian);
	RotateYMatrix.m[2][1] = 0;
	RotateYMatrix.m[2][2] = std::cos(radian);
	RotateYMatrix.m[2][3] = 0;

	RotateYMatrix.m[3][0] = 0;
	RotateYMatrix.m[3][1] = 0;
	RotateYMatrix.m[3][2] = 0;
	RotateYMatrix.m[3][3] = 1;
	return RotateYMatrix;
}

//3.Z軸回転行列
Matrix4x4 MakeRotateZMatrix(float radian) {
	Matrix4x4 RotateZMatrix = { 0 };
	RotateZMatrix.m[0][0] = std::cos(radian);
	RotateZMatrix.m[0][1] = std::sin(radian);
	RotateZMatrix.m[0][2] = 0;
	RotateZMatrix.m[0][3] = 0;

	RotateZMatrix.m[1][0] = -std::sin(radian);
	RotateZMatrix.m[1][1] = std::cos(radian);
	RotateZMatrix.m[1][2] = 0;
	RotateZMatrix.m[1][3] = 0;

	RotateZMatrix.m[2][0] = 0;
	RotateZMatrix.m[2][1] = 0;
	RotateZMatrix.m[2][2] = 1;
	RotateZMatrix.m[2][3] = 0;

	RotateZMatrix.m[3][0] = 0;
	RotateZMatrix.m[3][1] = 0;
	RotateZMatrix.m[3][2] = 0;
	RotateZMatrix.m[3][3] = 1;
	return RotateZMatrix;
}



// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };
	
	int kRowHeight = 20;

	Vector3 rotate{ 0.4f,1.43f,-0.8f };
	Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotate.x);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotate.y);
	Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotate.z);
	Matrix4x4 rotateXYZMatrix = Matrix4x4Multiply(rotateXMatrix, Matrix4x4Multiply(rotateYMatrix, rotateZMatrix));


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

		MatrixScreenPrintf(0, 0, rotateXMatrix, "rotateXMatrix");
		MatrixScreenPrintf(0, kRowHeight*5, rotateYMatrix, "rotateYMatrix");
		MatrixScreenPrintf(0, kRowHeight*5*2, rotateZMatrix, "rotateZMatrix");
		MatrixScreenPrintf(0, kRowHeight*5*3, rotateXYZMatrix, "rotateXYZMatrix");

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
