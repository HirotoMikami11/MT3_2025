#pragma once
#include "MyMath.h"
#include<algorithm>

/// <summary>
/// 球体
/// </summary>
struct Sphere {
	Vector3 center;	///中心点
	float radius;	///半径
};

/// <summary>
/// 直線
/// </summary>
struct Line {
	Vector3 origin;		//始点
	Vector3 diff;		//終点への差分ベクトル
};

/// <summary>
/// 半直線
/// </summary>
struct Ray {
	Vector3 origin;		//始点
	Vector3 diff;		//終点への差分ベクトル
};

/// <summary>
/// 線分
/// </summary>
struct Segment {
	Vector3 origin;		//始点
	Vector3 diff;		//終点への差分ベクトル
};

/// <summary>
/// 平面
/// </summary>
struct Plane {
	Vector3 normal;	//法線
	float distance;//距離

};


/// <summary>
/// 三角形
/// </summary>
struct Triangle {
	Vector3 vertices[3];//頂点
};

/// <summary>
/// AABB(軸並行境界箱)
/// </summary>
struct AABB {
	Vector3 min;	//最小点
	Vector3 max;	//最大点

};

/*-----------------------------------------------------------------------*/
//
//								計算関数
//
/*-----------------------------------------------------------------------*/

/// <summary>
/// 正射影ベクトルを求める関数
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns></returns>
Vector3 Project(const Vector3& v1, const Vector3& v2);

/// <summary>
/// 最近接点を求める関数
/// </summary>
/// <param name="point"></param>
/// <param name="segment"></param>
/// <returns></returns>
Vector3 ClosestPoint(const Vector3& point, const Segment& segment);

/// <summary>
/// 球と球の衝突判定
/// </summary>
/// <param name="sphere1"></param>
/// <param name="sphere2"></param>
/// <returns></returns>
bool IsCollision(const Sphere& sphere1, const Sphere& sphere2);

/// <summary>
/// 球と平面の衝突判定
/// </summary>
/// <param name="sphere"> 球体/param>
/// <param name="plane"> 平面</param>
/// <returns></returns>
bool IsCollision(const Sphere& sphere, const Plane& plane);


/// <summary>
/// 線分と平面の衝突判定
/// </summary>
/// <param name="segment">線分</param>
/// <param name="plane">平面</param>
/// <returns></returns>
bool IsCollision(const Segment& segment, const Plane& plane);



/// <summary>
/// 線分と平面の衝突点の座標を求める
/// </summary>
/// <param name="segment"></param>
/// <param name="normal"></param>
/// <param name="distance"></param>
/// <returns></returns>
Vector3 MakeCollisionPoint(const Segment& segment, const Plane& plane);
/// <summary>
/// 三角形と線分の衝突判定
/// </summary>
/// <param name="triangle"></param>
/// <param name="segnent"></param>
/// <returns></returns>
bool IsCollision(const Triangle& triangle, const Segment& segment);

/// <summary>
/// AABBとAABBの衝突判定
/// </summary>
/// <param name="aabb1"></param>
/// <param name="aabb2"></param>
/// <returns></returns>
bool IsCollision(const AABB& aabb1, const AABB& aabb2);


/// <summary>
/// AABBと球の衝突判定
/// </summary>
/// <param name="aabb"></param>
/// <param name="sphere"></param>
/// <returns></returns>
bool IsCollision(const AABB& aabb, Sphere& sphere);

/// <summary>
/// 最大最小を正しくする関数
/// </summary>
/// <param name="aabb"></param>
void FixAABBMinMax(AABB& aabb);



/// <summary>
/// 垂直なベクトルを求める
/// </summary>
/// <param name="vector"></param>
/// <returns></returns>
Vector3 Perpendicular(const Vector3& vector);


/*-----------------------------------------------------------------------*/
//
//								描画関数
//
/*-----------------------------------------------------------------------*/

/// <summary>
/// グリッド線を描画する関数
/// </summary>
/// <param name="viewProjectionMatrix"></param>
/// <param name="viewportMatrix"></param>
void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix);

/// <summary>
/// 球体を表示する関数
/// </summary>
/// <param name="sphere"></param>
/// <param name="viewProjectionMatrix"></param>
/// <param name="viewportMatrix"></param>
/// <param name="color"></param>
void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);

/// <summary>
/// 線分を描画する関数
/// </summary>
/// <param name="segment"></param>
/// <param name="viewProjectionMatrix"></param>
/// <param name="viewportMatrix"></param>
/// <param name="color"></param>
void DrawLine(const Segment& segment, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);


/// <summary>
/// 平面の描画
/// </summary>
/// <param name="plane"></param>
/// <param name="viewProjectionMatrix"></param>
/// <param name="viewportMatrix"></param>
/// <param name="color"></param>
void DrawPlane(const Plane& plane, const Matrix4x4 viewProjectionMatrix, const Matrix4x4 viewportMatrix, uint32_t color);





/// <summary>
/// 三角形を描画
/// </summary>
/// <param name="triangle"></param>
/// <param name="viewProjectionMatrix"></param>
/// <param name="viewportMatrix"></param>
/// <param name="color"></param>
void DrawTriangle(const Triangle& triangle, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);


/// <summary>
/// AABBの描画
/// </summary>
/// <param name="aabb"></param>
/// <param name="viewPeojectionMatrix"></param>
/// <param name="viewportMatrix"></param>
/// <param name="color"></param>
void DrawAABB(const AABB& aabb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);




