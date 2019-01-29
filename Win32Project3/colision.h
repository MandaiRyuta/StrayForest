#pragma once
#include "cscene.h"
#include <d3dx9.h>
#include <math.h>

class CSphereColision
{
public:
	CSphereColision() {}
	~CSphereColision() {}

public:
	static bool Collision_detection_of_Sphere_and_Sphere(D3DXVECTOR3* hit_position, const SphereInfo sphere_01, const SphereInfo sphere_02);
	static void NecessaryLengthCalculation_Sphere_Sphere(D3DXVECTOR3 *center_to_center_vector, float *two_radius, const SphereInfo *sphere0, const SphereInfo *sphere1);
	static bool Is_Sphere_and_Sphere_two_Square_Range_by_Collison_detection(const D3DXVECTOR3 *center_to_center_vector, float two_radius);
	static void AmountOfSqueezingVectorCalculation_Sphere_Sphere(D3DXVECTOR3 *hit_vector, D3DXVECTOR3 *center_to_center_vector, float two_radius);
};

class AABBColision
{
public:
	AABBColision() {}
	~AABBColision() {}

public:
	static bool intersectAABB(const AABB &box1, const AABB &box2 /*D3DXVECTOR3* hit*/);
	static void AABBColisionCheck(const AABB &playerpos,D3DXVECTOR3* hit);
private:
	D3DXVECTOR3 min_;
	D3DXVECTOR3 max_;
};

class TraiangleColision
{
public:
	TraiangleColision() {}
	~TraiangleColision() {}
public:
	//考え；敵の座標から三角形の視野を作り、プレイヤーの座標がもし三角形の範囲内に入っていたらアクションを起こすための関数。
	//static bool RayTriangleIntersect(const D3DXVECTOR3 TargetPosition);
};