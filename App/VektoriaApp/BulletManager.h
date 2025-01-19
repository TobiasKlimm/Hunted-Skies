#pragma once
#include "Vektoria\Root.h"
#include "Entity.h"

using namespace Vektoria;


class CBulletManager : public CPlacement
{
public:
	void Init(float bulletSpeed, float damage);
	void Tick(float fTime, float fTimeDelta);
	void Shoot(const CHVector& direction);
	void UpdateBullets();
	CPlacements* GetBullets()
	{
		return &m_zpsBullets;
	}
	void SetTerrain(CGeoTerrain* terrain)
	{
		m_zgTerrain = terrain;
	}
	void AddCollisionTarget(CPlacement* p);
	bool GetKilledEnemy();

private:
	bool m_killedEnemy;
	CPlacements m_collisionTargets;
	CHVector lastPosBullets[100];
	CPlacement m_zpBulletTemplate;
	CGeoCube m_zgBullet;
	CMaterial m_zmBulletMaterial;
	CPlacements m_zpsBullets;
	CGeoTerrain* m_zgTerrain; // Bullets dissapear when hitting the terrain
	CHMat m_position;
	CAudio m_zaShot;
	float m_bulletSpeed;
	float m_damage = 10;
	float m_timePassed = 0.0;
};