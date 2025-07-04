﻿#include "MyFunction.h"

/*-----------------------------------------------------------------------*/
//
//								計算関数
//
/*-----------------------------------------------------------------------*/


//	正射影ベクトルを求める関数
Vector3 Project(const Vector3& v1, const Vector3& v2) {
	Vector3 project = Vector3Multiply(Vector3Normalize(v2), Vector3Dot(v1, Vector3Normalize(v2)));
	return project;
}
//　最近接点を求める関数
Vector3 ClosestPoint(const Vector3& point, const Segment& segment) {
	Vector3 project = Project(Vector3Subtract(point, segment.origin), segment.diff);
	Vector3 closestPoint = Vector3Add(segment.origin, project);

	return closestPoint;
}


//　球と球の衝突判定
bool IsCollision(const Sphere& sphere1, const Sphere& sphere2) {
	bool isCollision = false;

	//2つの球の中心転換の距離を求める
	float distance = Vector3Length(Vector3Subtract(sphere1.center, sphere2.center));

	//半径の合計より短ければ衝突
	if (distance <= sphere1.radius + sphere2.radius) {
		isCollision = true;
	}

	return isCollision;
}

//球と平面の衝突判定
bool IsCollision(const Sphere& sphere, const Plane& plane) {

	//1.点と平面との距離
	//そのままだと符号付き距離になってしまうので、絶対値(abs)を取る
	float distance = std::abs(Vector3Dot(plane.normal, sphere.center) - plane.distance);

	//2.1の距離<=球の半径なら衝突
	if (distance <= sphere.radius) {
		return true;
	}

	return false;

}


//線分と平面の衝突判定
bool IsCollision(const Segment& segment, const Plane& plane) {
	//衝突しているかどうか
	bool isCollision = false;

	//垂直判定を行うため、法線と線の内積を求める
	float dot = Vector3Dot(plane.normal, segment.diff);
	//垂直=平行であるので、衝突していない
	if (dot == 0.0f) {
		return false;
	}

	//tを求める
	float t = (plane.distance - Vector3Dot(segment.origin, plane.normal)) / dot;

	//tの値と線の種類によって衝突しているかを判定する
	//線分なので0~1
	if (t >= 0 && t <= 1) {
		isCollision = true;
	}

	return isCollision;
}

// 線分と平面の衝突点の座標を求める
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


// 三角形と線分の衝突判定
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
/// AABBとAABBの衝突判定
/// </summary>
/// <param name="aabb1"></param>
/// <param name="aabb2"></param>
/// <returns></returns>
bool IsCollision(const AABB& aabb1, const AABB& aabb2) {

	///衝突判定
	if (aabb1.min.x <= aabb2.max.x && aabb1.max.x >= aabb2.min.x &&	//x軸の衝突
		aabb1.min.y <= aabb2.max.y && aabb1.max.y >= aabb2.min.y &&	//y軸の衝突
		aabb1.min.z <= aabb2.max.z && aabb1.max.z >= aabb2.min.z	//z軸の衝突
		) {
		return true;
	}
	return false;
}


bool IsCollision(const AABB& aabb, Sphere& sphere) {
	bool isCollision = false;

	//球の中心座標がAABBの[min,max]内にclampすれば、それが最近接点になる
	Vector3 closestPoint{
		std::clamp(sphere.center.x,aabb.min.x,aabb.max.x),
		std::clamp(sphere.center.y,aabb.min.y,aabb.max.y),
		std::clamp(sphere.center.z,aabb.min.z,aabb.max.z),
	};

	//最近接点と球の中心との距離を求める
	float distance = Vector3Length(Vector3Subtract(closestPoint, sphere.center));
	//距離が半径より小さければ衝突
	if (distance <= sphere.radius) {
		isCollision = true;
	}
	return isCollision;

}

/// <summary>
/// 最大最小を正しくする関数
/// </summary>
/// <param name="aabb"></param>
void FixAABBMinMax(AABB& aabb) {

	aabb.min.x = (std::min)(aabb.min.x, aabb.max.x);
	aabb.max.x = (std::max)(aabb.min.x, aabb.max.x);

	aabb.min.y = (std::min)(aabb.min.y, aabb.max.y);
	aabb.max.y = (std::max)(aabb.min.y, aabb.max.y);

	aabb.min.z = (std::min)(aabb.min.z, aabb.max.z);
	aabb.max.z = (std::max)(aabb.min.z, aabb.max.z);
}


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

// 垂直なベクトルを求める
Vector3 Perpendicular(const Vector3& vector) {
	if (vector.x != 0.0f || vector.y != 0.0f) {
		return { -vector.y, vector.x, 0.0f };
	}
	return { 0.0f, -vector.z, vector.y };
}

//3次元空間上で２次ベジェ曲線上の点を求める関数
Vector3 Bezier(const Vector3& p0, const Vector3& p1, const Vector3& p2, float t) {

	//制御点p0,p1を線形補間
	Vector3 p01 = Lerp(p0, p1, t);
	//制御点p1,p2を線形補間
	Vector3 p12 = Lerp(p1, p2, t);
	//制御点p0p1,p1p2をさらに線形補間
	Vector3 result = Lerp(p01, p12, t);

	return result;
}

// ワールド座標をスクリーン座標に変換する関数
Vector3 MakeScreenPositionToWorld(const Vector3& v, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix)
{
	Vector3 ndcPos = Transform(v, viewProjectionMatrix);
	Vector3 screenPos = Transform(ndcPos, viewportMatrix);
	return screenPos;
}





void UpdateSpring(Spring& spring, Ball& ball) {

	Vector3 diff = ball.position - spring.anchor;
	float length = Vector3Length(diff);

	if (length != 0.0f) {

		Vector3 direction = Vector3Normalize(diff);

		Vector3 restPosition = spring.anchor + direction * spring.naturalLength;
		Vector3 displacement = length * (ball.position - restPosition);
		Vector3 restoringForce = -spring.stiffness * displacement;

		///減衰抵抗を計算
		Vector3 dampingForce = -spring.dampingCoefficient * ball.velocity;
		//減衰抵抗も加味して、物体にかかる力を決定
		Vector3 force = restoringForce + dampingForce;

		ball.acceleration = force / ball.mass;
	}
	//加速度も速度も秒を基準とする(DeltaTimeを適応する)
	ball.velocity += ball.acceleration * FrameTimer::GetInstance().GetDeltaTime();
	ball.position += ball.velocity * FrameTimer::GetInstance().GetDeltaTime();

}



void UpdatePendulum(Pendulum& pendulum, Ball& ball) {
	//振り子の角度を計算する
	pendulum.angularAcceleration = -(9.0f / pendulum.length) * std::sinf(pendulum.angle);
	pendulum.angularVelocity += pendulum.angularAcceleration * FrameTimer::GetInstance().GetDeltaTime();
	pendulum.angle += pendulum.angularVelocity * FrameTimer::GetInstance().GetDeltaTime();

	//振り子の先端の位置にボールの座標を置く
	ball.position.x = pendulum.anchor.x + std::sin(pendulum.angle) * pendulum.length;
	ball.position.y = pendulum.anchor.y - std::cos(pendulum.angle) * pendulum.length;
	ball.position.z = pendulum.anchor.z;
}

void UpdatePendulum(ConicalPendulum& pendulum, Ball& ball) {
	//振り子の角度を計算する
	pendulum.angularVelocity = std::sqrt(9.8f / (pendulum.length * std::cos(pendulum.halfApexAngle)));
	pendulum.angle += pendulum.angularVelocity * FrameTimer::GetInstance().GetDeltaTime();


	float radius = std::sin(pendulum.halfApexAngle) * pendulum.length;	//円の半径
	float height = std::cos(pendulum.halfApexAngle) * pendulum.length;	//高さ

	ball.position.x = pendulum.anchor.x + std::cos(pendulum.angle) * radius;
	ball.position.y = pendulum.anchor.y - height;	//-をつけるのはY軸が上方向だから
	ball.position.z = pendulum.anchor.z + std::sin(pendulum.angle) * radius;
}

/*-----------------------------------------------------------------------*/
//
//								描画関数
//
/*-----------------------------------------------------------------------*/

//グリッド線を描画する関数

/// <summary>
/// グリッド線を描画する関数
/// </summary>
/// <param name="viewProjectionMatrix">ビュープロジェクション</param>
/// <param name="viewportMatrix">ビューポート</param>
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






/// <summary>
/// 球体を描画する関数
/// </summary>
/// <param name="sphere">球体</param>
/// <param name="viewProjectionMatrix">ビュープロジェクション</param>
/// <param name="viewportMatrix">ビューポート</param>
/// <param name="color">色</param>
void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	const uint32_t kSubdivision = 16;//分割数
	const float kLonEvery = (2 * std::numbers::pi_v<float>) / kSubdivision;		//経度分割1つ分の角度
	const float kLatEvery = std::numbers::pi_v<float> / kSubdivision;				//緯度分割1つ分の角度

	//緯度の方向に分割　-π/2　~π/2
	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		float lat = (-std::numbers::pi_v<float> / 2.0f) + kLatEvery * latIndex;	//現在の緯度(θ)
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


/// <summary>
/// 線分を描画する関数
/// </summary>
/// <param name="segment">線分</param>
/// <param name="viewProjectionMatrix">ビュープロジェクション</param>
/// <param name="viewportMatrix">ビューポート</param>
/// <param name="color">色</param>
void DrawLine(const Segment& segment, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
	///線分の両端を求める
	Vector3 start = segment.origin;
	Vector3 end = Vector3Add(segment.origin, segment.diff);
	///スクリーン座標に変換
	Vector3 screenStart = Transform(Transform(start, viewProjectionMatrix), viewportMatrix);
	Vector3 screenEnd = Transform(Transform(end, viewProjectionMatrix), viewportMatrix);
	///描画
	Novice::DrawLine(
		static_cast<int>(screenStart.x),
		static_cast<int>(screenStart.y),
		static_cast<int>(screenEnd.x),
		static_cast<int>(screenEnd.y),
		color);


}



//平面の描画
void DrawPlane(const Plane& plane, const Matrix4x4 viewProjectionMatrix, const Matrix4x4 viewportMatrix, uint32_t color) {
	//1.平面の中心点を決める
	Vector3 center = Vector3Multiply(plane.normal, plane.distance);

	Vector3 perpendiculars[4];
	//2.法線と垂直なベクトルを求める
	perpendiculars[0] = Vector3Normalize(Perpendicular(plane.normal));

	//3.2の逆ベクトルを求める
	perpendiculars[1] = { -perpendiculars[0].x,-perpendiculars[0].y,-perpendiculars[0].z };

	//4.2の法線とのクロス積を求める
	perpendiculars[2] = Cross(plane.normal, perpendiculars[0]);

	//5.4の逆ベクトルを求める
	perpendiculars[3] = { -perpendiculars[2].x,-perpendiculars[2].y,-perpendiculars[2].z };

	//6.2~5のベクトルを中心点に、定数倍して足して4頂点を出す
	Vector3 points[4];

	for (int32_t index = 0; index < 4; ++index) {
		Vector3 extend = Vector3Multiply(perpendiculars[index], 2.0f);
		Vector3 point = Vector3Add(center, extend);
		points[index] = Transform(Transform(point, viewProjectionMatrix), viewportMatrix);
	}
	//描画
	Novice::DrawLine(static_cast<int>(points[0].x), static_cast<int>(points[0].y),
		static_cast<int>(points[3].x), static_cast<int>(points[3].y), color);
	Novice::DrawLine(static_cast<int>(points[0].x), static_cast<int>(points[0].y),
		static_cast<int>(points[2].x), static_cast<int>(points[2].y), color);
	Novice::DrawLine(static_cast<int>(points[1].x), static_cast<int>(points[1].y),
		static_cast<int>(points[2].x), static_cast<int>(points[2].y), color);
	Novice::DrawLine(static_cast<int>(points[1].x), static_cast<int>(points[1].y),
		static_cast<int>(points[3].x), static_cast<int>(points[3].y), color);

}



// 三角形を描画
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


/// <summary>
/// AABBの描画
/// </summary>
/// <param name="aabb"></param>
/// <param name="viewPeojectionMatrix"></param>
/// <param name="viewportMatrix"></param>
/// <param name="color"></param>
void DrawAABB(const AABB& aabb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {

	///AABBを構成する頂点を作る
	Vector3 vertices[8];

	///上の四頂点
	vertices[0] = { aabb.max.x,aabb.max.y,aabb.max.z };//右上奥
	vertices[1] = { aabb.min.x,aabb.max.y,aabb.max.z };//左上奥
	vertices[2] = { aabb.max.x,aabb.max.y,aabb.min.z };//右上手前
	vertices[3] = { aabb.min.x,aabb.max.y,aabb.min.z };//左上手前

	///下の四頂点
	vertices[4] = { aabb.max.x,aabb.min.y,aabb.max.z };//右下奥
	vertices[5] = { aabb.min.x,aabb.min.y,aabb.max.z };//左下奥
	vertices[6] = { aabb.max.x,aabb.min.y,aabb.min.z };//右下手前
	vertices[7] = { aabb.min.x,aabb.min.y,aabb.min.z };//左下手前

	///座標変換
	Vector3 NdcVertices[8];
	Vector3 screenVertices[8];
	for (int i = 0; i < 8; i++) {
		//スクリーン座標系まで変換をかける
		NdcVertices[i] = Transform(vertices[i], viewProjectionMatrix);
		screenVertices[i] = Transform(NdcVertices[i], viewportMatrix);
	}

	//描画しやすいようにインデックスの位置を格納
	const int edges[12][2] = {
	{0, 1}, {1, 3}, {3, 2}, {2, 0}, //上面の四角形
	{4, 5}, {5, 7}, {7, 6}, {6, 4}, //下面の四角形
	{0, 4}, {1, 5}, {2, 6}, {3, 7}  //上面と下面をつなぐ線
	};
	///AABBを描画
	for (int i = 0; i < 12; i++) {
		Novice::DrawLine(
			static_cast<int>(screenVertices[edges[i][0]].x),
			static_cast<int>(screenVertices[edges[i][0]].y),
			static_cast<int>(screenVertices[edges[i][1]].x),
			static_cast<int>(screenVertices[edges[i][1]].y),
			color
		);
	}


}



void DrawBezier(
	const Vector3& controlPoint0,
	const Vector3& controlPoint1,
	const Vector3& controlPoint2,
	const int subdivision,
	const Matrix4x4& viewProjectionMatrix,
	const Matrix4x4& viewportMatrix,
	uint32_t color)
{
	// 前の点を保持するための変数
	Vector3 previousScreenPos = { 0,0,0 };
	bool hasPreviousPoint = false;
	for (int i = 0; i <= subdivision; ++i) {
		//tの値を求める
		float t = static_cast<float>(i) / static_cast<float>(subdivision);
		//ベジエ曲線上の点を求める
		Vector3 point = Bezier(controlPoint0, controlPoint1, controlPoint2, t);
		//スクリーン座標系に変換
		Vector3 ndcPos = Transform(point, viewProjectionMatrix);
		Vector3 screenPos = Transform(ndcPos, viewportMatrix);
		//描画
		if (hasPreviousPoint) {
			Novice::DrawLine(
				static_cast<int>(previousScreenPos.x),
				static_cast<int>(previousScreenPos.y),
				static_cast<int>(screenPos.x),
				static_cast<int>(screenPos.y),
				color);
		}
		// 現在の点を次のループの前の点として保存
		previousScreenPos = screenPos;
		hasPreviousPoint = true;
	}
}



void DrawBezierAndPoints(
	const Vector3& controlPoint0,
	const Vector3& controlPoint1,
	const Vector3& controlPoint2,
	const int subdivision,
	const Matrix4x4& viewProjectionMatrix,
	const Matrix4x4& viewportMatrix,
	uint32_t color)
{
	// 前の点を保持するための変数
	Vector3 previousScreenPos = { 0,0,0 };
	bool hasPreviousPoint = false;
	for (int i = 0; i <= subdivision; ++i) {
		//tの値を求める
		float t = static_cast<float>(i) / static_cast<float>(subdivision);
		//ベジエ曲線上の点を求める
		Vector3 point = Bezier(controlPoint0, controlPoint1, controlPoint2, t);
		//スクリーン座標系に変換
		Vector3 ndcPos = Transform(point, viewProjectionMatrix);
		Vector3 screenPos = Transform(ndcPos, viewportMatrix);
		//描画
		if (hasPreviousPoint) {
			Novice::DrawLine(
				static_cast<int>(previousScreenPos.x),
				static_cast<int>(previousScreenPos.y),
				static_cast<int>(screenPos.x),
				static_cast<int>(screenPos.y),
				color);
		}
		// 現在の点を次のループの前の点として保存
		previousScreenPos = screenPos;
		hasPreviousPoint = true;
	}

	///各制御点の描画
	DrawSphere({ controlPoint0, 0.01f }, viewProjectionMatrix, viewportMatrix, 0xFF0000FF); // 赤色
	DrawSphere({ controlPoint1, 0.01f }, viewProjectionMatrix, viewportMatrix, 0x00FF00FF); // 緑色
	DrawSphere({ controlPoint2, 0.01f }, viewProjectionMatrix, viewportMatrix, 0x0000FFFF); // 青色

}


void DrawSpring(Spring& spring, Ball& ball, const Matrix4x4& viewProjectionMatrix,
	const Matrix4x4& viewportMatrix, uint32_t color) {

	Vector3 springScreenPos = MakeScreenPositionToWorld(spring.anchor, viewProjectionMatrix, viewportMatrix);
	Vector3 ballScreenPos = MakeScreenPositionToWorld(ball.position, viewProjectionMatrix, viewportMatrix);
	Novice::DrawLine(static_cast<int>(springScreenPos.x), static_cast<int>(springScreenPos.y),
		static_cast<int>(ballScreenPos.x), static_cast<int>(ballScreenPos.y),
		WHITE);

	DrawSphere({ ball.position,ball.radius }, viewProjectionMatrix, viewportMatrix, color);
}

void DrawPendulum(Pendulum& pendulum, Ball& ball, const Matrix4x4& viewProjectionMatrix,
	const Matrix4x4& viewportMatrix, uint32_t color) {

	Vector3 springScreenPos = MakeScreenPositionToWorld(pendulum.anchor, viewProjectionMatrix, viewportMatrix);
	Vector3 ballScreenPos = MakeScreenPositionToWorld(ball.position, viewProjectionMatrix, viewportMatrix);
	Novice::DrawLine(static_cast<int>(springScreenPos.x), static_cast<int>(springScreenPos.y),
		static_cast<int>(ballScreenPos.x), static_cast<int>(ballScreenPos.y),
		color);

	DrawSphere({ ball.position,ball.radius }, viewProjectionMatrix, viewportMatrix, ball.color);
}

void DrawPendulum(ConicalPendulum& pendulum, Ball& ball, const Matrix4x4& viewProjectionMatrix,
	const Matrix4x4& viewportMatrix, uint32_t color) {

	Vector3 pendulumScreenPos = MakeScreenPositionToWorld(pendulum.anchor, viewProjectionMatrix, viewportMatrix);
	Vector3 ballScreenPos = MakeScreenPositionToWorld(ball.position, viewProjectionMatrix, viewportMatrix);
	Novice::DrawLine(static_cast<int>(pendulumScreenPos.x), static_cast<int>(pendulumScreenPos.y),
		static_cast<int>(ballScreenPos.x), static_cast<int>(ballScreenPos.y),
		color);

	DrawSphere({ ball.position,ball.radius }, viewProjectionMatrix, viewportMatrix, ball.color);
}


