#pragma once
#include "MyMath.h"
#include "FrameTimer.h"
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


/// <summary>
/// 振り子
/// </summary>
struct Pendulum {
	Vector3 anchor;				//アンカーポイント
	float length;				//紐の長さ
	float angle;				//現在の角度
	float angularVelocity;		//角速度
	float angularAcceleration;	//各加速度
};


/// <summary>
/// 円錐振り子
/// </summary>
struct ConicalPendulum {
	Vector3 anchor;			//アンカーポイント	
	float length;			//紐の長さ
	float halfApexAngle;	//頂角の半分
	float angle;			//現在の角度
	float angularVelocity;	//角速度
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
/// AABBと線分の衝突判定
/// </summary>
/// <param name="aabb"></param>
/// <param name="segment"></param>
/// <returns></returns>
bool IsCollision(const AABB& aabb, const Segment& segment);

/// <summary>
/// 垂直なベクトルを求める
/// </summary>
/// <param name="vector"></param>
/// <returns></returns>
Vector3 Perpendicular(const Vector3& vector);


/// <summary>
/// 3次元空間上で２次ベジェ曲線上の点を求める関数
/// </summary>
/// <param name="p0">制御点</param>
/// <param name="p1">制御点</param>
/// <param name="p2">制御点</param>
/// <param name="t">補間の割合</param>
/// <returns></returns>
Vector3 Bezier(const Vector3& p0, const Vector3& p1, const Vector3& p2, float t);


/// <summary>
/// ワールド座標からスクリーン座標を計算する関数
/// </summary>
/// <param name="v">ワールド座標</param>
/// <param name="viewProjectionMatrix">ビュープロジェクション</param>
/// <param name="viewportMatrix">ビューポート</param>
/// <returns></returns>
Vector3 MakeScreenPositionToWorld(const Vector3& v, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix);



/// <summary>
/// 
/// </summary>
/// <param name="spring"></param>
/// <param name="ball"></param>
void UpdateSpring(Spring& spring, Ball& ball);



/// <summary>
/// 
/// </summary>
/// <param name="pendulum"></param>
/// <param name="ball"></param>
void UpdatePendulum(Pendulum& pendulum, Ball& ball);


/// <summary>
/// 円錐振り子の更新
/// </summary>
/// <param name="pendulum"></param>
/// <param name="ball"></param>
void UpdatePendulum(ConicalPendulum& pendulum, Ball& ball);

/*-----------------------------------------------------------------------*/
//
//								描画関数
//
/*-----------------------------------------------------------------------*/

/// <summary>
/// グリッド線を描画する関数
/// </summary>
/// <param name="viewProjectionMatrix">ビュープロジェクション</param>
/// <param name="viewportMatrix">ビューポート</param>
void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix);

/// <summary>
/// 球体を表示する関数
/// </summary>
/// <param name="sphere"></param>
/// <param name="viewProjectionMatrix">ビュープロジェクション</param>
/// <param name="viewportMatrix">ビューポート</param>
/// <param name="color"></param>
void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);

/// <summary>
/// 線分を描画する関数
/// </summary>
/// <param name="segment"></param>
/// <param name="viewProjectionMatrix">ビュープロジェクション</param>
/// <param name="viewportMatrix">ビューポート</param>
/// <param name="color"></param>
void DrawLine(const Segment& segment, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);


/// <summary>
/// 平面の描画
/// </summary>
/// <param name="plane"></param>
/// <param name="viewProjectionMatrix">ビュープロジェクション</param>
/// <param name="viewportMatrix">ビューポート</param>
/// <param name="color"></param>
void DrawPlane(const Plane& plane, const Matrix4x4 viewProjectionMatrix, const Matrix4x4 viewportMatrix, uint32_t color);





/// <summary>
/// 三角形を描画
/// </summary>
/// <param name="triangle"></param>
/// <param name="viewProjectionMatrix">ビュープロジェクション</param>
/// <param name="viewportMatrix">ビューポート</param>
/// <param name="color"></param>
void DrawTriangle(const Triangle& triangle, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);


/// <summary>
/// AABBの描画
/// </summary>
/// <param name="aabb"></param>
/// <param name="viewProjectionMatrix">ビュープロジェクション</param>
/// <param name="viewportMatrix">ビューポート</param>
/// <param name="color"></param>
void DrawAABB(const AABB& aabb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);


/// <summary>
/// ベジエ曲線を描画する関数
/// </summary>
/// <param name="controlPoint0">制御点0</param>
/// <param name="controlPoint1">制御点1</param>
/// <param name="controlPoint2">制御点2</param>
/// <param name="subdivision">分割数</param>
/// <param name="viewProjectionMatrix">ビュープロジェクション</param>
/// <param name="viewportMatrix">ビューポート</param>
/// <param name="color">色</param>
void DrawBezier(
	const Vector3& controlPoint0,
	const Vector3& controlPoint1,
	const Vector3& controlPoint2,
	const int subdivision,
	const Matrix4x4& viewProjectionMatrix,
	const Matrix4x4& viewportMatrix,
	uint32_t color);



/// <summary>
/// ベジエ曲線を描画する関数(制御点も描画する)
/// </summary>
/// <param name="controlPoint0">制御点0</param>
/// <param name="controlPoint1">制御点1</param>
/// <param name="controlPoint2">制御点2</param>
/// <param name="subdivision">分割数</param>
/// <param name="viewProjectionMatrix">ビュープロジェクション</param>
/// <param name="viewportMatrix">ビューポート</param>
/// <param name="color">色</param>
void DrawBezierAndPoints(
	const Vector3& controlPoint0,
	const Vector3& controlPoint1,
	const Vector3& controlPoint2,
	const int subdivision,
	const Matrix4x4& viewProjectionMatrix,
	const Matrix4x4& viewportMatrix,
	uint32_t color);


void DrawSpring(Spring& spring, Ball& ball, const Matrix4x4& viewProjectionMatrix,
	const Matrix4x4& viewportMatrix, uint32_t color);


void DrawPendulum(Pendulum& pendulum, Ball& ball, const Matrix4x4& viewProjectionMatrix,
	const Matrix4x4& viewportMatrix, uint32_t color);


/// <summary>
/// 円錐振り子の描画関数
/// </summary>
/// <param name="pendulum"></param>
/// <param name="ball"></param>
/// <param name="viewProjectionMatrix"></param>
/// <param name="viewportMatrix"></param>
/// <param name="color"></param>
void DrawPendulum(ConicalPendulum& pendulum, Ball& ball, const Matrix4x4& viewProjectionMatrix,
	const Matrix4x4& viewportMatrix, uint32_t color);