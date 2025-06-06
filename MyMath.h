#pragma once
#define _USE_MATH_DEFINES
#include<math.h>
#include <cmath>
#include<Novice.h>
#include<assert.h>
#include <format>//imGuiで配列をfor分で回すとき使う
#include<algorithm>











float Lerp(const float& min, const float& max, float t);
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
// 線形補間
Vector2 Lerp(const Vector2& min, const Vector2& max, float t);
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

//距離を求める
float Vector3Distance(const Vector3 v1, const Vector3 v2);
//クロス積
Vector3 Cross(const Vector3& v1, const Vector3& v2);

// 線形補間
Vector3 Lerp(const Vector3& min, const Vector3& max, float t);

/*-----------------------------------------------------------------------*/
//
//						Vector3 演算子オーバーロード
//
/*-----------------------------------------------------------------------*/

// ===== 二項演算子 =====

// 加算 (v1 + v2)
inline  Vector3 operator+(const  Vector3& v1, const  Vector3& v2) { return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z }; }

// 減算 (v1 - v2)
inline  Vector3 operator-(const  Vector3& v1, const  Vector3& v2) { return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z }; }

// スカラー倍 (scalar * v)
inline  Vector3 operator*(float scalar, const  Vector3& v) { return { v.x * scalar, v.y * scalar, v.z * scalar }; }

// スカラー倍 (v * scalar)
inline  Vector3 operator*(const  Vector3& v, float scalar) { return { v.x * scalar, v.y * scalar, v.z * scalar }; }

// スカラー除算 (v / scalar)
inline  Vector3 operator/(const  Vector3& v, float scalar) {
	assert(scalar != 0.0f); // ゼロ除算チェック
	return { v.x / scalar, v.y / scalar, v.z / scalar };
}

// ===== 単項演算子 =====

// 単項マイナス (-v)
inline  Vector3 operator-(const  Vector3& v) { return { -v.x, -v.y, -v.z }; }

// 単項プラス (+v) - 不要だが、対称性のため
inline  Vector3 operator+(const  Vector3& v) { return v; }

// ===== 複合代入演算子 =====

// 加算代入 (v1 += v2)
inline  Vector3& operator+=(Vector3& v1, const  Vector3& v2) {
	v1.x += v2.x;
	v1.y += v2.y;
	v1.z += v2.z;
	return v1;
}

// 減算代入 (v1 -= v2)
inline  Vector3& operator-=(Vector3& v1, const  Vector3& v2) {
	v1.x -= v2.x;
	v1.y -= v2.y;
	v1.z -= v2.z;
	return v1;
}

// スカラー倍代入 (v *= scalar)
inline  Vector3& operator*=(Vector3& v, float scalar) {
	v.x *= scalar;
	v.y *= scalar;
	v.z *= scalar;
	return v;
}

// スカラー除算代入 (v /= scalar)
inline  Vector3& operator/=(Vector3& v, float scalar) {
	assert(scalar != 0.0f); // ゼロ除算チェック
	v.x /= scalar;
	v.y /= scalar;
	v.z /= scalar;
	return v;
}
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
//XYZ
Matrix4x4 MakeRotateXYZMatrix(const Vector3& rotate);

//アフィン返還行列
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

//透視射影行列
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);
//正射影行列
Matrix4x4 MakeOrthograpicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);
//ビューポート変換行列
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);

Matrix4x4 MakeViewProjectionMatrix(const Matrix4x4 cameraMatrix, float aspectRatio);


/*-----------------------------------------------------------------------*/
//
//								4x4		演算子のオーバーロード
//
/*-----------------------------------------------------------------------*/

// ===== 二項演算子 =====

// 加算 (m1 + m2)
inline Matrix4x4 operator+(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result = { 0 };
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.m[i][j] = m1.m[i][j] + m2.m[i][j];
		}
	}
	return result;
}

// 減算 (m1 - m2)
inline Matrix4x4 operator-(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result = { 0 };
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.m[i][j] = m1.m[i][j] - m2.m[i][j];
		}
	}
	return result;
}

// 行列の積 (m1 * m2)
inline Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result = { 0 };
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.m[i][j] =
				(m1.m[i][0] * m2.m[0][j]) +
				(m1.m[i][1] * m2.m[1][j]) +
				(m1.m[i][2] * m2.m[2][j]) +
				(m1.m[i][3] * m2.m[3][j]);
		}
	}
	return result;
}

// スカラー倍 (scalar * m)
inline Matrix4x4 operator*(float scalar, const Matrix4x4& m) {
	Matrix4x4 result = { 0 };
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.m[i][j] = m.m[i][j] * scalar;
		}
	}
	return result;
}

// スカラー倍 (m * scalar)
inline Matrix4x4 operator*(const Matrix4x4& m, float scalar) {
	Matrix4x4 result = { 0 };
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.m[i][j] = m.m[i][j] * scalar;
		}
	}
	return result;
}

// スカラー除算 (m / scalar)
inline Matrix4x4 operator/(const Matrix4x4& m, float scalar) {
	assert(scalar != 0.0f); // ゼロ除算チェック
	Matrix4x4 result = { 0 };
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.m[i][j] = m.m[i][j] / scalar;
		}
	}
	return result;
}

// ===== 単項演算子 =====

// 単項マイナス (-m)
inline Matrix4x4 operator-(const Matrix4x4& m) {
	Matrix4x4 result = { 0 };
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.m[i][j] = -m.m[i][j];
		}
	}
	return result;
}

// 単項プラス (+m) - 対称性のため
inline Matrix4x4 operator+(const Matrix4x4& m) {
	return m;
}

// ===== 複合代入演算子 =====

// 加算代入 (m1 += m2)
inline Matrix4x4& operator+=(Matrix4x4& m1, const Matrix4x4& m2) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m1.m[i][j] += m2.m[i][j];
		}
	}
	return m1;
}

// 減算代入 (m1 -= m2)
inline Matrix4x4& operator-=(Matrix4x4& m1, const Matrix4x4& m2) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m1.m[i][j] -= m2.m[i][j];
		}
	}
	return m1;
}

// 乗算代入 (m1 *= m2) - 行列の積
inline Matrix4x4& operator*=(Matrix4x4& m1, const Matrix4x4& m2) {
	m1 = m1 * m2;  // 上で定義した operator* を使用
	return m1;
}

// スカラー倍代入 (m *= scalar)
inline Matrix4x4& operator*=(Matrix4x4& m, float scalar) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m.m[i][j] *= scalar;
		}
	}
	return m;
}

// スカラー除算代入 (m /= scalar)
inline Matrix4x4& operator/=(Matrix4x4& m, float scalar) {
	assert(scalar != 0.0f); // ゼロ除算チェック
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m.m[i][j] /= scalar;
		}
	}
	return m;
}