#include <Novice.h>
#include "MyFunction.h"
#include <imgui.h>
#include "Camera.h"
#include<string>

#include"FrameTimer.h"

const char kWindowTitle[] = "LE2A_15_ミカミ_ヒロト_MT3_04_00";



struct Spring {
	//アンカー固定された橋の位置
	Vector3 anchor;
	float naturalLength;		//自然長
	float stiffness;			//剛性,バネ定数k
	float dampingCoefficient;	//減衰係数
};

struct Ball {
	Vector3 position;			//ボールの位置
	Vector3 velocity;			//ボールの速度
	Vector3 acceleration;		//ボールの加速度
	float mass;					//ボールの質量
	float radius;				//ボールの半径
	unsigned int color;			//ボールの色
};


void UpdateSpring(Spring& spring, Ball& ball) {

	Vector3 diff = ball.position - spring.anchor;
	float length = Vector3Length(diff);

	if (length != 0.0f) {

		Vector3 direction = Vector3Normalize(diff);

		Vector3 restPosition = spring.anchor + direction * spring.naturalLength;
		Vector3 displacement = length * (ball.position - restPosition);
		Vector3 restoringForce = -spring.stiffness * displacement;

		///減衰抵抗を計算
		Vector3 dampingForce = -spring.dampingCoefficient * ball.velocity;
		//減衰抵抗も加味して、物体にかかる力を決定
		Vector3 force = restoringForce + dampingForce;

		ball.acceleration = force / ball.mass;
	}
	//加速度も速度も秒を基準とする(DeltaTimeを適応する)
	ball.velocity += ball.acceleration * FrameTimer::GetInstance().GetDeltaTime();
	ball.position += ball.velocity * FrameTimer::GetInstance().GetDeltaTime();

}


void DrawSpring(Spring& spring, Ball& ball, const Matrix4x4& viewProjectionMatrix,
	const Matrix4x4& viewportMatrix, uint32_t color) {

	Vector3 springScreenPos = MakeScreenPositionToWorld(spring.anchor, viewProjectionMatrix, viewportMatrix);
	Vector3 ballScreenPos = MakeScreenPositionToWorld(ball.position, viewProjectionMatrix, viewportMatrix);
	Novice::DrawLine(static_cast<int>(springScreenPos.x), static_cast<int>(springScreenPos.y),
		static_cast<int>(ballScreenPos.x), static_cast<int>(ballScreenPos.y),
		WHITE);

	DrawSphere({ ball.position,ball.radius }, viewProjectionMatrix, viewportMatrix, color);
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

	Spring spring{};
	spring.anchor = { 0.0f,0.0f,0.0f };
	spring.naturalLength = 1.0f;
	spring.stiffness = 100.0f;
	spring.dampingCoefficient = 2.0f;

	Ball ball{};
	ball.position = { 1.2f,0.0f,0.0f };
	ball.mass = 2.0f;
	ball.radius = 0.05f;
	ball.color = BLUE;




	bool moveSpring=false;


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


		if (moveSpring) {
			UpdateSpring(spring, ball);
		}


		ImGui::Begin("debug");
		frameTimer.ImGui();
		ImGui::Separator();	//線
		//バネを動かすかどうか
		if (ImGui::Button("Move Spring")) {
			moveSpring = !moveSpring;
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
		//バネを描画
		DrawSpring(spring, ball, camera->GetViewProjectionMatrix(), camera->GetViewportMatrix(), BLUE);

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
