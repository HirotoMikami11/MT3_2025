﻿#include "MyFunction.h"


//グリッド線を描画する関数
void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix) {
	const float kGridHalfWidth = 2.0f;									//Gridの半分の幅
	const uint32_t kSubdivision = 10;									//分割数
	const float kGridEvery = (kGridHalfWidth * 2.0f) / float(kSubdivision);	//1つ分の長さ

	//奥から手前に線を順々（左→右）に引いていく
	for (uint32_t xIndex = 0; xIndex <= kSubdivision; xIndex++) {
		//ワールド座標上の始点と終点を求める
		//左から一本ずつ引いていく
		float pos_x = -kGridHalfWidth + (xIndex * kGridEvery);
		Vector3 startPos = { pos_x,0,-kGridHalfWidth };
		Vector3 endPos = { pos_x,0,kGridHalfWidth };
		//スクリーン座標系まで変換する
		Matrix4x4 startWorldMatrix = MakeAffineMatrix({ 1,1,1 }, { 0,0,0 }, { 0,0,0 });
		Matrix4x4 startWorldViewProjectionMatrix = Matrix4x4Multiply(startWorldMatrix, viewProjectionMatrix);
		Vector3 startNdcPos = Transform(startPos, startWorldViewProjectionMatrix);
		Vector3 startScreenPos = Transform(startNdcPos, viewportMatrix);

		Matrix4x4 endWorldMatrix = MakeAffineMatrix({ 1,1,1 }, { 0,0,0 }, { 0,0,0 });
		Matrix4x4 endWorldViewProjectionMatrix = Matrix4x4Multiply(endWorldMatrix, viewProjectionMatrix);
		Vector3 endNdcPos = Transform(endPos, endWorldViewProjectionMatrix);
		Vector3 endScreenPos = Transform(endNdcPos, viewportMatrix);

		//変換した座標を使って表示
		Novice::DrawLine(
			static_cast<int>(startScreenPos.x),
			static_cast<int>(startScreenPos.y),
			static_cast<int>(endScreenPos.x),
			static_cast<int>(endScreenPos.y),
			(xIndex == kSubdivision / 2) ? 0x000000FF : 0xAAAAAAFF);//原点だけ黒、それ以外は灰色
	}


	//左から右に線を順々（奥→手前）に引いていく
	for (uint32_t zIndex = 0; zIndex <= kSubdivision; zIndex++) {
		//ワールド座標上の始点と終点を求める
		//左から一本ずつ引いていく
		float pos_z = -kGridHalfWidth + (zIndex * kGridEvery);
		Vector3 startPos = { -kGridHalfWidth,0,pos_z };
		Vector3 endPos = { kGridHalfWidth,0,pos_z };
		//スクリーン座標系まで変換する
		Matrix4x4 startWorldMatrix = MakeAffineMatrix({ 1,1,1 }, { 0,0,0 }, { 0,0,0 });
		Matrix4x4 startWorldViewProjectionMatrix = Matrix4x4Multiply(startWorldMatrix, viewProjectionMatrix);
		Vector3 startNdcPos = Transform(startPos, startWorldViewProjectionMatrix);
		Vector3 startScreenPos = Transform(startNdcPos, viewportMatrix);

		Matrix4x4 endWorldMatrix = MakeAffineMatrix({ 1,1,1 }, { 0,0,0 }, { 0,0,0 });
		Matrix4x4 endWorldViewProjectionMatrix = Matrix4x4Multiply(endWorldMatrix, viewProjectionMatrix);
		Vector3 endNdcPos = Transform(endPos, endWorldViewProjectionMatrix);
		Vector3 endScreenPos = Transform(endNdcPos, viewportMatrix);

		//変換した座標を使って表示
		Novice::DrawLine(
			static_cast<int>(startScreenPos.x),
			static_cast<int>(startScreenPos.y),
			static_cast<int>(endScreenPos.x),
			static_cast<int>(endScreenPos.y),
			(zIndex == kSubdivision / 2) ? 0x000000FF : 0xAAAAAAFF);//原点だけ黒、それ以外は灰色
	}

}





//球体を表示する関数
void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	const uint32_t kSubdivision = 10;//分割数
	const float kLonEvery = (2 * float(M_PI)) / kSubdivision;		//経度分割1つ分の角度
	const float kLatEvery = float(M_PI) / kSubdivision;				//緯度分割1つ分の角度

	//緯度の方向に分割　-π/2　~π/2
	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		float lat = (-float(M_PI) / 2.0f) + kLatEvery * latIndex;	//現在の緯度(θ)
		//経度の方向に分割 0 ~ 2π
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			float lon = lonIndex * kLonEvery;						//現在の経度（φ）
			//world座標系でのa,b,cを求める
			Vector3 a = {
			cos(lat) * cos(lon),
			sin(lat),
			cos(lat) * sin(lon)
			};
			Vector3 b = {
			cos(lat + kLatEvery) * cos(lon),
			sin(lat + kLatEvery),
			cos(lat + kLatEvery) * sin(lon)
			};
			Vector3 c = {
			cos(lat) * cos(lon + kLonEvery),
			sin(lat),
			cos(lat) * sin(lon + kLonEvery)
			};

			//球体の中心座標と半径を加味して、a,b,cをワールド座標に変換
			a = Vector3Add(sphere.center, Vector3Multiply(a, sphere.radius));
			b = Vector3Add(sphere.center, Vector3Multiply(b, sphere.radius));
			c = Vector3Add(sphere.center, Vector3Multiply(c, sphere.radius));

			//a,b,cをスクリーン座標に変換
			Vector3 aNdcPos = Transform(a, viewProjectionMatrix);
			Vector3 aScreenPos = Transform(aNdcPos, viewportMatrix);

			Vector3 bNdcPos = Transform(b, viewProjectionMatrix);
			Vector3 bScreenPos = Transform(bNdcPos, viewportMatrix);

			Vector3 cNdcPos = Transform(c, viewProjectionMatrix);
			Vector3 cScreenPos = Transform(cNdcPos, viewportMatrix);

			//ab,bcで線を引く
			Novice::DrawLine(
				static_cast<int>(aScreenPos.x),
				static_cast<int>(aScreenPos.y),
				static_cast<int>(bScreenPos.x),
				static_cast<int>(bScreenPos.y),
				color);
			Novice::DrawLine(
				static_cast<int>(aScreenPos.x),
				static_cast<int>(aScreenPos.y),
				static_cast<int>(cScreenPos.x),
				static_cast<int>(cScreenPos.y),
				color);

		}

	}

}
