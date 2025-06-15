#include <Novice.h>
#include "MyFunction.h"
#include <imgui.h>
#include "Camera.h"
#include<string>
//FPS
#include"FrameTimer.h"

const char kWindowTitle[] = "LE2A_15_ミカミ_ヒロト_MT3_04_01";



/// <summary>
/// 円運動を行う関数
/// </summary>
/// <param name="movePosition">動く座標</param>
/// <param name="originPostion">円運動の中心になる座標</param>
/// <param name="anglarVelocity">角速度(1秒間にどの程度角度が進むか)</param>
/// <param name="radius">円運動をする半径</param>
/// <param name="angle">現在の角度</param>
void circularMotion(Vector3& movePosition, Vector3 originPostion, float& anglarVelocity, float& circleRadius, float& angle) {

	angle += anglarVelocity * FrameTimer::GetInstance().GetDeltaTime();

	//円運動の計算
	movePosition.x = originPostion.x + std::cosf(angle) * circleRadius;
	movePosition.y = originPostion.y + std::sinf(angle) * circleRadius;
	movePosition.z = originPostion.z;

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

	// FPS関連
	FrameTimer& frameTimer = FrameTimer::GetInstance();

	Sphere sphere;
	sphere.center = { 0,0,0 };
	sphere.radius = 0.1f;

	bool isMove = false;

	//角速度
	float anglarVelocity = std::numbers::pi_v<float>;
	float angle = 0.0f;
	//円運動するの半径(r)
	float circleRadius = 0.8f;

	//最初に円運動の初期位置に置いておく。
	sphere.center.x = 0.0f + std::cosf(angle) * circleRadius;
	sphere.center.y = 0.0f + std::sinf(angle) * circleRadius;
	sphere.center.z = 0.0f;


	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		frameTimer.BeginFrame();

		///
		/// ↓更新処理ここから
		///

		//カメラの更新
		camera->Update(keys, preKeys);

		if (isMove) {
			////円運動
			circularMotion(sphere.center,{0.0f,0.0f,0.0f},anglarVelocity,circleRadius,angle);
		}







		///*	ImGUI	 *///

		ImGui::Begin("debug");
		frameTimer.ImGui();
		ImGui::Separator();	//線
		//アクションを起こす
		if (ImGui::Button("isAction")) {
			isMove = !isMove;
		}

		ImGui::End();

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		//グリッド線を描画
		DrawGrid(camera->GetViewProjectionMatrix(), camera->GetViewportMatrix());

		//球体を表示
		DrawSphere(sphere, camera->GetViewProjectionMatrix(), camera->GetViewportMatrix(), WHITE);

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
