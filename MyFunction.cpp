#include "MyFunction.h"

/*-----------------------------------------------------------------------*/
//
//								描画関数
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
	bool isCollision = false;

	//1.平面と球の中心点との距離を求める
	float direction = Vector3Length(Vector3Subtract(plane.normal, sphere.center));

	//2.1の距離<=球の半径なら衝突
	if (direction <= sphere.radius) {
		isCollision = true;
	}

	return isCollision;
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



// 垂直なベクトルを求める
Vector3 Perpendicular(const Vector3& vector) {
	if (vector.x != 0.0f || vector.y != 0.0f) {
		return { -vector.y, vector.x, 0.0f };
	}
	return { 0.0f, -vector.z, vector.y };
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
