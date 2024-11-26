#pragma once
#include "Enemy.h"
#include "BulletManager.h"


using namespace Vektoria;

class CTurret :
	public CEnemy
{
public:
	void Init(CPlacement* target);
	void Tick(float fTime, float fTimeDelta);
	void Shoot(float fTimeDelta);
	
	CBulletManager* GetBulletManager() {
		return &m_BulletManager;
	}
	CBulletManager m_BulletManager;
	
private:
	CGeoTriangleTable* m_pzgTurret = nullptr;
	CPlacement m_zpTurret;
	CPlacement m_zpTurretPointing;

	CPlacement* m_zpTarget;

	CHVector m_vDir;
	CHVector m_vDirNormal;
};