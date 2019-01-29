#include "colision.h"
#define swap(a,b) { a += b; b = a - b; a -= b;}
bool CSphereColision::Collision_detection_of_Sphere_and_Sphere(D3DXVECTOR3* hit_position, const SphereInfo sphere_01, const SphereInfo sphere_02)
{
	// 中心間距離と半径の合計による衝突判定( 2乗 )
	D3DXVECTOR3 center_to_center_vector;
	float    two_radius;

	NecessaryLengthCalculation_Sphere_Sphere(&center_to_center_vector, &two_radius, &sphere_02, &sphere_01);

	if (!Is_Sphere_and_Sphere_two_Square_Range_by_Collison_detection(&center_to_center_vector, two_radius))
	{
		// めり込み具合を算出
		AmountOfSqueezingVectorCalculation_Sphere_Sphere(hit_position, &center_to_center_vector, two_radius);
		return false;
	}



	return true;
}

//================================================================================
//
// [ 必要な長さを算出関数( 球_球 ) ]
//
//================================================================================
void CSphereColision::NecessaryLengthCalculation_Sphere_Sphere(D3DXVECTOR3 *center_to_center_vector, float *two_radius, const SphereInfo *sphere0, const SphereInfo *sphere1)
{
	*center_to_center_vector = sphere0->modelpos - sphere1->modelpos;
	*two_radius = sphere0->r + sphere1->r;
}

/*******************************************
球と球2乗の距離で衝突判定関数
********************************************/
bool CSphereColision::Is_Sphere_and_Sphere_two_Square_Range_by_Collison_detection(const D3DXVECTOR3 *center_to_center_vector, float two_radius)
{
	return D3DXVec3Length(center_to_center_vector) > (two_radius);
}

//================================================================================
//
// [ めり込みベクトル算出関数( 球_球 ) ]
//
//================================================================================
void CSphereColision::AmountOfSqueezingVectorCalculation_Sphere_Sphere(D3DXVECTOR3 *hit_vector, D3DXVECTOR3 *center_to_center_vector, float two_radius)
{
	if (hit_vector)
	{
		D3DXVECTOR3 centerlength;
		centerlength = *center_to_center_vector;
		D3DXVec3Normalize(center_to_center_vector, center_to_center_vector);
		float length = D3DXVec3Length(&centerlength) - two_radius;

		*hit_vector = *center_to_center_vector * length; //方向ベクトル*めり込んでいる量
	}
}

bool AABBColision::intersectAABB(const AABB & box1, const AABB & box2/* D3DXVECTOR3* hit*/)
{
	if (box1.min.x > box2.max.x) return false;
	if (box1.max.x < box2.min.x) return false;
	if (box1.min.y > box2.max.y) return false;
	if (box1.max.y < box2.min.y) return false;
	if (box1.min.z > box2.max.z) return false;
	if (box1.max.z < box2.min.z) return false;

	AABB boxIntersect;
	boxIntersect.min.x = max(box1.min.x, box2.min.x);
	boxIntersect.max.x = min(box1.max.x, box2.max.x);
	boxIntersect.min.y = max(box1.min.y, box2.min.y);
	boxIntersect.max.y = min(box1.max.y, box2.max.y);
	boxIntersect.min.z = max(box1.min.z, box2.min.z);
	boxIntersect.max.z = min(box1.max.z, box2.max.z);

	////衝突した位置を求めている。
	//hit->x = (boxIntersect.max.x + boxIntersect.min.x) / 2.0f;
	//hit->y = (boxIntersect.max.y + boxIntersect.min.y) / 2.0f;
	//hit->z = (boxIntersect.max.z + boxIntersect.min.z) / 2.0f;

	return true;
}

void AABBColision::AABBColisionCheck(const AABB &playerpos, D3DXVECTOR3 * hit)
{
	/*対象となる1方向のめり込み量を返す処理*/
	D3DXVECTOR3 playerlength = playerpos.min - playerpos.max;
	
	if (playerlength.x >= 0.4f)
	{
		hit->x = playerlength.x;
		hit->y = 0.0f;
		hit->z = 0.0f;
		playerlength.y = 0.0f;
		playerlength.z = 0.0f;
	}
	if (playerlength.x <= -0.4f)
	{
		hit->x = playerlength.x;
		hit->y = 0.0f;
		hit->z = 0.0f;
		playerlength.y = 0.0f;
		playerlength.z = 0.0f;
	}

	if (playerlength.y >= 0.4f)
	{
		hit->x = 0.0f;
		hit->y = playerlength.y;
		hit->z = 0.0f;
		playerlength.x = 0.0f;
		playerlength.z = 0.0f;
	}
	if (playerlength.y <= -0.4f)
	{
		hit->x = 0.0f;
		hit->y = playerlength.y;
		hit->z = 0.0f;
		playerlength.x = 0.0f;
		playerlength.z = 0.0f;
	}

	if (playerlength.z <= -0.4f)
	{
		hit->x = 0.0f;
		hit->y = 0.0f;
		hit->z = playerlength.z;
		playerlength.x = 0.0f;
		playerlength.y = 0.0f;
	}
	if (playerlength.z >= 0.4f)
	{
		hit->x = 0.0f;
		hit->y = 0.0f;
		hit->z = playerlength.z;
		playerlength.x = 0.0f;
		playerlength.y = 0.0f;
	}
}
