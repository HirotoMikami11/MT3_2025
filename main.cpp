#include <Novice.h>
#include "MyFunction.h"
#include <imgui.h>
#include "Camera.h"
#include<string>
const char kWindowTitle[] = "LE2A_15_ミカミ_ヒロト_MT3_02_04";

/// <summary>
/// 三角形
/// </summary>
struct Triangle {
	Vector3 vertices[3];//頂点
};

/// <summary>
/// 線分と平面の衝突点の座標を求める
/// </summary>
/// <param name="segment"></param>
/// <param name="normal"></param>
/// <param name="distance"></param>
/// <returns></returns>
Vector3 MakeCollisionPoint(const Segment& segment, const Plane& plane) {
	///衝突点
	Vector3 CollsionPoint;

	//平面と線の衝突判定と同様
	float dot = Vector3Dot(segment.diff, plane.normal);
	assert(dot != 0.0f);
	float t = (plane.distance - (Vector3Dot(segment.origin, plane.normal))) / dot;

	//衝突点を求める
	//p=origin+tb
	CollsionPoint = Vector3Add(segment.origin, Vector3Multiply(t, segment.diff));
	return CollsionPoint;
}



/// <summary>
/// 三角形と線分の衝突判定
/// </summary>
/// <param name="triangle"></param>
/// <param name="segnent"></param>
/// <returns></returns>
bool IsCollision(const Triangle& triangle, const Segment& segment) {
	//衝突しているかどうか
	bool isCollision = false;

	///1.線と三角形の存在する平面との衝突判定を行う
	//三角形の中心座標求める
	Vector3 TriangleCenter = {
		(triangle.vertices[0].x + triangle.vertices[1].x + triangle.vertices[2].x) / 3,
		(triangle.vertices[0].y + triangle.vertices[1].y + triangle.vertices[2].y) / 3,
		(triangle.vertices[0].z + triangle.vertices[1].z + triangle.vertices[2].z) / 3
	};

	//平面を作成する
	Plane plane;
	//距離
	plane.distance = Vector3Distance(TriangleCenter, { 0,0,0 });
	//法線
	plane.normal = Cross(Vector3Subtract(triangle.vertices[1], triangle.vertices[0]), Vector3Subtract(triangle.vertices[2], triangle.vertices[1]));


	if (IsCollision(segment, plane)) {


		///2.衝突していたら、衝突点が三角形の内側にあるのかを調べる
		//衝突点pを作成
		Vector3 p = MakeCollisionPoint(segment, plane);
		//衝突点と、三角形それぞれの辺で新たな三角形を作成する。(衝突点が[2]になるように)
		//a.各辺を結んだベクトル
		Vector3 v01 = Vector3Subtract(triangle.vertices[1], triangle.vertices[0]);
		Vector3 v12 = Vector3Subtract(triangle.vertices[2], triangle.vertices[1]);
		Vector3 v20 = Vector3Subtract(triangle.vertices[0], triangle.vertices[2]);
		//b.頂点と衝突点pを結んだベクトル
		Vector3 v1p = Vector3Subtract(p, triangle.vertices[1]);
		Vector3 v2p = Vector3Subtract(p, triangle.vertices[2]);
		Vector3 v0p = Vector3Subtract(p, triangle.vertices[0]);

		///法線ベクトルと同じ方向を向いているか見るため、aとbで外積を行う
		Vector3 cross01 = Cross(v01, v1p);
		Vector3 cross12 = Cross(v12, v2p);
		Vector3 cross20 = Cross(v20, v0p);

		///全ての小さな三角形の外積と法線が同じ方向を向いていたら、衝突している
			//全ての小さい三角形のクロス積と法線が同じ方法を向いていたら衝突
		if (
			Vector3Dot(cross01, plane.normal) >= 0.0f &&
			Vector3Dot(cross12, plane.normal) >= 0.0f &&
			Vector3Dot(cross20, plane.normal) >= 0.0f
			) {
			isCollision = true;

		}
	}

	return isCollision;

}


/// <summary>
/// 三角形を描画
/// </summary>
/// <param name="triangle"></param>
/// <param name="viewProjectionMatrix"></param>
/// <param name="viewportMatrix"></param>
/// <param name="color"></param>
void DrawTriangle(const Triangle& triangle, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {

	Vector3 ndcPos[3];
	Vector3 screenPos[3];

	//スクリーン座標系まで変換をかける
	for (int i = 0; i < 3; i++) {
		ndcPos[i] = Transform(triangle.vertices[i], viewProjectionMatrix);
		screenPos[i] = Transform(ndcPos[i], viewportMatrix);
	}

	Novice::DrawTriangle(
		static_cast<int>(screenPos[0].x),
		static_cast<int>(screenPos[0].y),
		static_cast<int>(screenPos[1].x),
		static_cast<int>(screenPos[1].y),
		static_cast<int>(screenPos[2].x),
		static_cast<int>(screenPos[2].y),
		color, kFillModeWireFrame);

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


	Segment segment_ = {
		{0.0f,0.5f,0.0f},
		{0.0f,0.5f,2.0f}
	};

	Triangle triangle_ = {
		{{-1,0,0},		//
		{0,1,0},		//
		{1,0,0}}		//
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


		ImGui::DragFloat3("segment_Origin", &segment_.origin.x, 0.1f);
		ImGui::DragFloat3("segment_Diff", &segment_.diff.x, 0.1f);
		for (int i = 0; i < 3; i++)
		{
			std::string label_Triangle = "Triangle_Vertices[" + std::to_string(i) + "] ";
			ImGui::DragFloat3(label_Triangle.c_str(), &triangle_.vertices[i].x, 0.1f);

		}







		//グリッド線を描画
		DrawGrid(camera->GetViewProjectionMatrix(), camera->GetViewportMatrix());
		//ライン
		DrawLine(segment_, camera->GetViewProjectionMatrix(), camera->GetViewportMatrix(), IsCollision(triangle_, segment_) ? RED : WHITE);
		//三角形
		DrawTriangle(triangle_, camera->GetViewProjectionMatrix(), camera->GetViewportMatrix(), WHITE);


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
