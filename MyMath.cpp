#include "MyMath.h"


/*-----------------------------------------------------------------------*/
//
//								2次元ベクトル
//
/*-----------------------------------------------------------------------*/








/*-----------------------------------------------------------------------*/
//
//								3次元ベクトル
//
/*-----------------------------------------------------------------------*/


void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label) {
	static const int kColumnWidthVec3 = 60;
	static const int kRowHeightVec3 = 20;
	Novice::ScreenPrintf(x, y, "%.02f", vector.x);
	Novice::ScreenPrintf(x + kColumnWidthVec3, y, "%.02f", vector.y);
	Novice::ScreenPrintf(x + kColumnWidthVec3 * 2, y, "%.02f", vector.z);
	Novice::ScreenPrintf(x + kColumnWidthVec3 * 3, y, "%s", label);
};

Vector3 Vector3Add(const Vector3& v1, const Vector3& v2) {
	Vector3 result = {
		v1.x + v2.x,
		v1.y + v2.y,
		v1.z + v2.z
	};
	return result;
}

Vector3 Vector3Subtract(const Vector3& v1, const Vector3& v2) {
	Vector3 result = {
	v1.x - v2.x,
	v1.y - v2.y,
	v1.z - v2.z
	};
	return result;
}

Vector3 Vector3Multiply(const Vector3& v,float scalar ) {
	Vector3 result = {
	v.x * scalar,
	v.y * scalar,
	v.z * scalar
	};
	return result;
}

float Vector3Dot(const Vector3& v1, const Vector3& v2) {
	float result =
		v1.x * v2.x +
		v1.y * v2.y +
		v1.z * v2.z;

	return result;
}

float Vector3Length(const Vector3& v) {
	float result = sqrtf(
		v.x * v.x +
		v.y * v.y +
		v.z * v.z
	);
	return result;
}

Vector3 Vector3Normalize(const Vector3& v) {
	Vector3 result = { 0,0,0 };
	float length = Vector3Length(v);

	if (length != 0) {
		result.x = v.x / length;
		result.y = v.y / length;
		result.z = v.z / length;
	}

	return result;
}

