#include "Turret.h"

void CTurret::Init(CPlacement* target)
{
	m_zpTarget = target;
	CFileWavefront objLoader;
	m_pzgTurret = objLoader.LoadGeoTriangleTable("models\\Turret.obj");
	m_zpTurret.AddGeo(m_pzgTurret);
	this->Scale(10);
	m_zpTurret.AddPlacement(&m_BulletManager);
	this->AddPlacement(&m_zpTurret);
	m_BulletManager.Init(0.1f, 500);
	this->SetName("Turret");
}

void CTurret::Tick(float fTime, float fTimeDelta)
{
	m_vDir = CHVector(m_zpTarget->GetPosGlobal() - m_position);
	m_vDirNormal = m_vDir;
	m_vDirNormal.Normal();
	float fa = m_vDirNormal.AngleXZ();
	float faUpDown = asinf(m_vDirNormal.y);

	UM_SETINRANGE(faUpDown, -QUARTERPI, QUARTERPI);

	m_zpTurret.RotateZ(faUpDown);
	m_zpTurret.RotateYDelta(-fa);
	if (m_vDir.Length() < 1000) {
		Shoot(fTimeDelta);
	}
}

void CTurret::Shoot(float fTimeDelta)
{
	CHVector vRand;
	vRand.RandomDir();
	m_position = this->GetPosGlobal();
	m_BulletManager.Shoot(m_vDirNormal+vRand*0.01, fTimeDelta);
}
