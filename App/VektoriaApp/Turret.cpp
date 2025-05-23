#include "Turret.h"

void CTurret::Init(CPlacement* target)
{
	m_HitboxGeo.SetAxis(eAxisY);
	m_HitboxGeo.Init(2, 3, NULL, 6);
	m_HitboxGeo.SetDrawingOff();
	m_HitboxGeo.SetName("Hans");
	AddGeo(&m_HitboxGeo);

	m_zpTarget = target;
	CFileWavefront objLoader;

	m_pzgTurretFoundation = objLoader.LoadGeoTriangleTable("models\\turret\\foundation\\foundation.obj");
	m_pzgTurretBase = objLoader.LoadGeoTriangleTable("models\\turret\\base\\base.obj");
	m_pzgTurretBarrel = objLoader.LoadGeoTriangleTable("models\\turret\\barrel\\barrel.obj");

	m_zmTurretBase.LoadPreset("WindowGothicStained");
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

		// Distance to the target
		float dist = m_vDir.Length();

		// Calculation of the ball's flight time based on the distance and ball speed
		float bulletTravelTime = dist / BULLETSPEED;

		// Prediction of the future position of the aircraft based on flight direction and speed
		CHVector predictedTargetPos = m_zpTarget->GetPosGlobal() + direction * flySpeed * bulletTravelTime;

		// Adjustment of the target direction to the predicted position of the aircraft
		m_vDir = CHVector(predictedTargetPos - GetPosGlobal());

		// Optional: Small correction, e.g. for height differences, if necessary
		m_vDir = m_vDir + CHVector(0, 1, 0) * bulletTravelTime;

		// Standardization of the direction
		m_vDir.Normal();

		float fa = m_vDir.AngleXZ();
		float faUpDown = asinf(m_vDir.y);

		UM_SETINRANGE(faUpDown, -QUARTERPI, QUARTERPI);
		
		m_zpTurretBase.RotateZ(fa - HALFPI);
		m_zpTurretBase.RotateXDelta(HALFPI);

		m_zpTurretBarrel.RotateX(-faUpDown);

		m_zpTurretBase.TranslateYDelta(1.f);
		m_zpTurretBarrel.TranslateZDelta(-0.6f);

		if (dist < 800) {
			Shoot(fTimeDelta);
		}
	}
}

void CTurret::Shoot(float fTimeDelta)
{
	m_timePassed += fTimeDelta;

	// Execute the function when enough time has passed
	if (m_timePassed <= SHOOT_FREQUENCY)
		return;

	m_timePassed = 0.0;
	CHVector vRand;
	vRand.RandomDir();
	m_BulletManager.Shoot(m_vDir + vRand * 0.02f);
}
