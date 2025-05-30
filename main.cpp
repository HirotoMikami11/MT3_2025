#include <Novice.h>
#include "MyFunction.h"
#include <imgui.h>
#include "Camera.h"
#include<string>
#include<algorithm>

const char kWindowTitle[] = "LE2A_15_ミカミ_ヒロト_MT3_02_07";

/// <summary>
/// AABBと線分の衝突判定
/// </summary>
/// <param name="aabb"></param>
/// <param name="segment"></param>
/// <returns></returns>
bool IsCollision(const AABB& aabb, const Segment& segment) {


	// 0除算にならないように、diffが0の時はそれが中にあるかどうか確認する
	if (segment.diff.x == 0.0f && segment.diff.y == 0.0f && segment.diff.z == 0.0f) {
		// 点の場合：始点がAABB内にあれば衝突、なければ衝突してない
		return (segment.origin.x >= aabb.min.x && segment.origin.x <= aabb.max.x &&
			segment.origin.y >= aabb.min.y && segment.origin.y <= aabb.max.y &&
			segment.origin.z >= aabb.min.z && segment.origin.z <= aabb.max.z);
	}


	//線分なので、0~1の範囲でtを求める
	float tmin = 0.0f;  // 線分の開始
	float tmax = 1.0f;  // 線分の終了



	//それぞれの軸の判定を行う

	/// X軸の処理

	// diffが0でない場合、AABBとの交差を計算
	if (segment.diff.x != 0.0f) {
		float txmin = (aabb.min.x - segment.origin.x) / segment.diff.x;
		float txmax = (aabb.max.x - segment.origin.x) / segment.diff.x;

		// Near/Farを正しい順序にする
		float tNearX = min(txmin, txmax);
		float tFarX = max(txmin, txmax);

		// 全体の範囲を更新
		tmin = max(tmin, tNearX);
		tmax = min(tmax, tFarX);

		if (tmin > tmax) {
			return false;// tminがtmaxより大きい場合、衝突していない
		}
	} else {
		//diffが0の場合、線分がX軸に平行
		// 始点のX座標がAABBの範囲内なら衝突
		if (segment.origin.x < aabb.min.x || segment.origin.x > aabb.max.x) {
			return false;
		}
	}


	/// Y軸の処理

	// diffが0でない場合、AABBとの交差を計算
	if (segment.diff.y != 0.0f) {
		float tymin = (aabb.min.y - segment.origin.y) / segment.diff.y;
		float tymax = (aabb.max.y - segment.origin.y) / segment.diff.y;

		// Near/Farを正しい順序にする
		float tNearY = min(tymin, tymax);
		float tFarY = max(tymin, tymax);
		// 全体の範囲を更新
		tmin = max(tmin, tNearY);
		tmax = min(tmax, tFarY);

		if (tmin > tmax) {
			return false;// tminがtmaxより大きい場合、衝突していない
		}
	} else {
		//diffが0の場合、線分がY軸に平行
		// 始点のY座標がAABBの範囲内なら衝突
		if (segment.origin.y < aabb.min.y || segment.origin.y > aabb.max.y) {
			return false;
		}
	}


	/// Z軸の処理

	// diffが0でない場合、AABBとの交差を計算
	if (segment.diff.z != 0.0f) {
		float tzmin = (aabb.min.z - segment.origin.z) / segment.diff.z;
		float tzmax = (aabb.max.z - segment.origin.z) / segment.diff.z;

		// Near/Farを正しい順序にする
		float tNearZ = min(tzmin, tzmax);
		float tFarZ = max(tzmin, tzmax);

		// 全体の範囲を更新
		tmin = max(tmin, tNearZ);
		tmax = min(tmax, tFarZ);

		if (tmin > tmax) {
			return false;	// tminがtmaxより大きい場合、衝突していない
		}
	} else {
		//diffが0の場合、線分がZ軸に平行
		// 始点のZ座標がAABBの範囲内なら衝突
		if (segment.origin.z < aabb.min.z || segment.origin.z > aabb.max.z) {
			return false;
		}
	}

	// tmin <= tmax かつ 0 <= tmax かつ tmin <= 1 なら衝突
	if (tmin <= tmax && tmax >= 0.0f && tmin <= 1.0f) {
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

	AABB aabb1{
	.min{-0.5f,-0.5f,-0.5f},
	.max{0.0f,0.0f,0.0f}
	};

	Segment segment{
		.origin{-0.7f,0.3f,0.0f},
		.diff{2.0f,-0.5f,0.0f}
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

		ImGui::DragFloat3("aabb1.max", &aabb1.max.x, 0.05f);
		ImGui::DragFloat3("aabb1.min", &aabb1.min.x, 0.05f);
		//AABBのminmaxを正常値に維持する
		FixAABBMinMax(aabb1);

		ImGui::DragFloat3("segment_Origin", &segment.origin.x, 0.05f);
		ImGui::DragFloat3("segment_Diff", &segment.diff.x, 0.05f);

		ImGui::End();






		//グリッド線を描画
		DrawGrid(camera->GetViewProjectionMatrix(), camera->GetViewportMatrix());

		DrawAABB(aabb1, camera->GetViewProjectionMatrix(), camera->GetViewportMatrix(), IsCollision(aabb1, segment) ? RED : WHITE);
		//ライン
		DrawLine(segment, camera->GetViewProjectionMatrix(), camera->GetViewportMatrix(), IsCollision(aabb1, segment) ? RED : WHITE);

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
