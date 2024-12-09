#pragma once
#include "Vektoria\Root.h"
#include "Enemy.h"

using namespace Vektoria;


class CBulletManager :
	public CPlacement
{
public:
	//CBulletManager(const CGeos& collisionTargets, const CHMat & position, const CHVector& direction, float shootFrequency, float bulletSpeed);

	void Init(float shootFrequency, float bulletSpeed);

	void Tick(float fTime, float fTimeDelta);

	void Shoot(const CHVector& direction, float fTimeDelta);

	void UpdateBullets();

	CWriting* GetWriting()
	{
		return &m_zw;
	}
	CPlacements* GetBullets()
	{
		return &m_zpsBullets;
	}
	CPlacements m_collisionTargets;
private:
	CHVector lastPosBullets[100];
	CPlacement m_zpBulletTemplate;
	CGeoSphere m_zgBullet;
	CMaterial m_zmBulletMaterial;
	CPlacements m_zpsBullets;
	CGeoTerrain m_zgTerrain; //Bullets verschwinden wenn sie das Terrain berühren
	CHMat m_position;
	float m_shootFrequency;
	float m_bulletSpeed;

	float m_timePassed = 0.0;

	CWriting m_zw; CWritingFont m_zwf;
};