#include "Turret.h"

void CTurret::Init(CPlacement* target)
{
	m_zpTarget = target;
	CFileWavefront objLoader;
	m_pzgTurret = objLoader.LoadGeoTriangleTable("models\\Turret.obj");
	m_zpTurret.AddGeo(m_pzgTurret);
	m_BulletManager.Init(BULLETSPEED, DAMAGE);
	this->AddPlacement(&m_BulletManager);
	this->AddPlacement(&m_zpTurret);
	this->SetName("Turret");
}

void CTurret::Tick(float fTime, float fTimeDelta, CHVector direction, float flySpeed)
{
	m_BulletManager.Tick(fTime, fTimeDelta);

	if (IsOn()) {
		this->Scale(10);
		this->TranslateDelta(GetPosition());
		m_vDir = CHVector(m_zpTarget->GetPosGlobal() - GetPosGlobal());
		float dist = m_vDir.Length();
		m_vDir = m_vDir+direction*flySpeed/2*dist/BULLETSPEED+CHVector(0,1,0)*dist/50;
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
}

void CTurret::Shoot(float fTimeDelta)
{
	m_timePassed += fTimeDelta;
	// Führe die Funktion aus, während genug Zeit vergangen ist
	if (m_timePassed <= SHOOT_FREQUENCY)
		return;
	m_timePassed = 0.0;
	CHVector vRand;
	vRand.RandomDir();
	m_BulletManager.Shoot(m_vDirNormal+vRand*0.01f);
}
