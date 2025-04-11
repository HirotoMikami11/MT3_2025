#pragma once
#define _USE_MATH_DEFINES
#include<math.h>
#include<Novice.h>


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
//内積
float Vector3Dot(const Vector3& v1, const Vector3& v2);
//長さ
float Vector3Length(const Vector3& v);
//正規化
Vector3 Vector3Normalize(const Vector3& v);

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
Matrix4x4 Matrix4x4Add(const Matrix4x4& m1, const Matrix4x4& m2);
Matrix4x4 Matrix4x4Subtract(const Matrix4x4& m1, const Matrix4x4& m2);
Matrix4x4 Matrix4x4Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

Matrix4x4 Matrix4x4Inverse(const Matrix4x4& m);
Matrix4x4 Matrix4x4Transpose(const Matrix4x4& m);
Matrix4x4 MakeIdentity4x4();
