#include "Turret.h"

void CTurret::Init(CPlacement* target)
{
	m_zpTarget = target;
	CFileWavefront objLoader;

	m_pzgTurretFoundation = objLoader.LoadGeoTriangleTable("models\\turret\\foundation\\foundation.obj");
	m_pzgTurretBase = objLoader.LoadGeoTriangleTable("models\\turret\\base\\base.obj");
	m_pzgTurretBarrel = objLoader.LoadGeoTriangleTable("models\\turret\\barrel\\barrel.obj");

	m_zmTurretBase.LoadPreset("WindowGothicStained");
	//m_zmTurretMaterial.MakeTextureDiffuse("models\\turret\\metal.jpg");
	m_pzgTurretBase->SetMaterial(&m_zmTurretBase);

	m_zmTurretFoundation.LoadPreset("Concrete");
	m_pzgTurretFoundation->SetMaterial(&m_zmTurretFoundation);

	m_zmTurretBarrel.LoadPreset("LeatherBlack");
	m_pzgTurretBarrel->SetMaterial(&m_zmTurretBarrel);

	m_zpTurretFoundation.AddGeo(m_pzgTurretFoundation);
	m_zpTurretBase.AddGeo(m_pzgTurretBase);
	m_zpTurretBarrel.AddGeo(m_pzgTurretBarrel);

	m_zpTurretFoundation.AddPlacement(&m_zpTurretBase);
	m_zpTurretBase.AddPlacement(&m_zpTurretBarrel);
	m_zpTurretBarrel.AddPlacement(&m_BulletManager);
	
	m_BulletManager.TranslateY(3.5f);
	m_BulletManager.TranslateZDelta(-0.5f);

	m_BulletManager.Init(BULLETSPEED, DAMAGE);
	this->AddPlacement(&m_zpTurretFoundation); // Turret <= Foundation <= Base <= Barrel <= Bulletmanager
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
		m_vDir = m_vDir + direction * flySpeed / 2 * dist / BULLETSPEED + CHVector(0, 1, 0) * dist / 50;
		m_vDirNormal = m_vDir;
		m_vDirNormal.Normal();

		float fa = m_vDirNormal.AngleXZ();
		float faUpDown = asinf(m_vDirNormal.y);

		UM_SETINRANGE(faUpDown, -QUARTERPI, QUARTERPI);
		
		m_zpTurretBase.RotateZ(fa - HALFPI);
		m_zpTurretBase.RotateXDelta(HALFPI);

		m_zpTurretBarrel.RotateX(-faUpDown + 0.17 /* Offset, dass Barrel genauer auf Flugzeug zeigt */);

		m_zpTurretBase.TranslateYDelta(1.f);
		m_zpTurretBarrel.TranslateZDelta(-0.6f);

		if (m_vDir.Length() < 1000) {
			Shoot(fTimeDelta);
		}
	}
}

void CTurret::Shoot(float fTimeDelta)
{
	m_timePassed += fTimeDelta;
	// F�hre die Funktion aus, w�hrend genug Zeit vergangen ist
	if (m_timePassed <= SHOOT_FREQUENCY)
		return;
	m_timePassed = 0.0;
	CHVector vRand;
	vRand.RandomDir();
	m_BulletManager.Shoot(m_vDirNormal + vRand * 0.01f);
}
