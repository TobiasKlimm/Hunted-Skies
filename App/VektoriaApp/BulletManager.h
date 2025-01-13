#pragma once
#include "Vektoria\Root.h"
#include "Enemy.h"

using namespace Vektoria;


class CBulletManager :
	public CPlacement
{
public:
	//CBulletManager(const CGeos& collisionTargets, const CHMat & position, const CHVector& direction, float shootFrequency, float bulletSpeed);

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
	CPlacements m_collisionTargets;
	bool m_killedEnemy;
private:
	CHVector lastPosBullets[100];
	CPlacement m_zpBulletTemplate;
	CGeoCube m_zgBullet;
	CMaterial m_zmBulletMaterial;
	CPlacements m_zpsBullets;
	CGeoTerrain* m_zgTerrain; //Bullets verschwinden wenn sie das Terrain berühren
	CHMat m_position;
	CAudio m_zaShot;
	float m_bulletSpeed;
	float m_damage = 10;

	float m_timePassed = 0.0;
};