#pragma once
#include "Enemy.h"
#include "BulletManager.h"

#define BULLETSPEED 600
#define DAMAGE 1.0f
#define SHOOT_FREQUENCY 0.1f


using namespace Vektoria;

class CTurret :
	public CEnemy
{
public:
	void Init(CPlacement* target);
	void Tick(float fTime, float fTimeDelta, CHVector direction, float flySpeed);
	void Shoot(float fTimeDelta);
	
	CBulletManager* GetBulletManager() {
		return &m_BulletManager;
	}
	CBulletManager m_BulletManager;
	
private:
	float m_timePassed = 0.0;

	CGeoTriangleTable* m_pzgTurret = nullptr;
	CPlacement m_zpTurret;
	CPlacement m_zpTurretPointing;

	CPlacement* m_zpTarget;

	CHVector m_vDir;
	CHVector m_vDirNormal;
};