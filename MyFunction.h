#pragma once
#include "MyMath.h"

struct Sphere {
	Vector3 center;	///中心点
	float radius;	///半径
};


//グリッド線を描画する関数
void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix);

//球体を表示する関数
void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);



