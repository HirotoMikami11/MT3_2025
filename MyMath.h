﻿#pragma once
#define _USE_MATH_DEFINES
#include<math.h>
#include <cmath>
#include<Novice.h>
#include <format>//imGuiで配列をfor分で回すとき使う

/*-----------------------------------------------------------------------*/
//
//								2次元ベクトル
//
/*-----------------------------------------------------------------------*/

/// <summary>
/// 2次元ベクトル
/// </summary>
struct Vector2 final {
	float x;
	float y;
};

/*-----------------------------------------------------------------------*/
//
//								3次元ベクトル
//
/*-----------------------------------------------------------------------*/
/// <summary>
/// 3次元ベクトル
/// </summary>
struct Vector3 final {
	float x;
	float y;
	float z;
};

//３次元ベクトルの値を表示する
void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label);
//加算
Vector3 Vector3Add(const Vector3& v1, const Vector3& v2);
//減算
Vector3 Vector3Subtract(const Vector3& v1, const Vector3& v2);
//スカラー倍
Vector3 Vector3Multiply(const Vector3& v, float scalar);
Vector3 Vector3Multiply(float scalar, const Vector3& v);
//内積
float Vector3Dot(const Vector3& v1, const Vector3& v2);
//長さ
float Vector3Length(const Vector3& v);
//正規化
Vector3 Vector3Normalize(const Vector3& v);

//クロス積
Vector3 Cross(const Vector3& v1, const Vector3& v2);

/*-----------------------------------------------------------------------*/
//
//								4x4
//
/*-----------------------------------------------------------------------*/
/// <summary>
/// 4x4行列
/// </summary>
struct Matrix4x4 final {
	float m[4][4];
};


//4x4の行列の数値を表示する
void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label);
//4x4行列の加算
Matrix4x4 Matrix4x4Add(const Matrix4x4& m1, const Matrix4x4& m2);
//4x4行列の減算
Matrix4x4 Matrix4x4Subtract(const Matrix4x4& m1, const Matrix4x4& m2);
//4x4行列の積
Matrix4x4 Matrix4x4Multiply(const Matrix4x4& m1, const Matrix4x4& m2);
//4x4行列の逆行列
Matrix4x4 Matrix4x4Inverse(const Matrix4x4& m);
//4x4行列の転置
Matrix4x4 Matrix4x4Transpose(const Matrix4x4& m);
//4x4行列の単位行列の生成
Matrix4x4 MakeIdentity4x4();


//4x4行列の平行移動行列
Matrix4x4 MakeTranslateMatrix(const Vector3& translate);
//4x4行列の拡大縮小行列
Matrix4x4 MakeScaleMatrix(const Vector3& Scale);
//4x4行列の座標変換
Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);


//X軸回転行列
Matrix4x4 MakeRotateXMatrix(float radian);
//Y軸回転行列
Matrix4x4 MakeRotateYMatrix(float radian);
//Z軸回転行列
Matrix4x4 MakeRotateZMatrix(float radian);
//アフィン返還行列
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

//透視射影行列
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);
//正射影行列
Matrix4x4 MakeOrthograpicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);
//ビューポート変換行列
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);

Matrix4x4 MakeViewProjectionMatrix(const Matrix4x4 cameraMatrix, float aspectRatio);
