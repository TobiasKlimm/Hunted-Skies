#include "BulletManager.h"

void CBulletManager::Init(float shootFrequency, float bulletSpeed)
{
	m_zwf.LoadPreset("OCRAExtendedRed");
	m_zw.Init(C2dRect(0.2f, 0.1f, 0.0f, 0.0f), 10, &m_zwf);

	m_shootFrequency = shootFrequency;
	m_bulletSpeed = bulletSpeed;

	//m_zmBulletMaterial.LoadPreset("AshesGlowing");
	m_zgBullet.Init(0.3f, NULL);
	//m_zgBullet.SetMaterial(&m_zmBulletMaterial);
	m_zpBulletTemplate.AddGeo(&m_zgBullet);
	m_zpBulletTemplate.SetPhysics(0.2f, 0.05f, 0.0f, 20000.1f, true);
	m_zpBulletTemplate.SwitchOff();
	m_zpsBullets.RingMake(100, m_zpBulletTemplate);
	this->AddPlacement(&m_zpBulletTemplate);
}

void CBulletManager::Tick(float fTime, float fTimeDelta) {
	UpdateBullets();
}

void CBulletManager::Shoot(const CHVector& direction, float fTimeDelta) {
	m_timePassed += fTimeDelta;
	// Führe die Funktion aus, während genug Zeit vergangen ist
	if (m_timePassed <= m_shootFrequency)
		return;
	m_timePassed = 0.0;
	if (m_zpsBullets.RingIsFull())
		m_zpsBullets.RingDec();

	m_position.CopyTranslation(this->GetPosGlobal());
	CPlacement* pzp = m_zpsBullets.RingInc();
	// Kopiere die Matrix (und damit die Startposition)
	pzp->SetMat(m_position);
	//schießt die Kugel in die Richtung die das Flugueug zeigt
	CHVector v;
	v.Copy(direction);
	pzp->SetPhysicsVelocity(v * m_bulletSpeed);
}

void CBulletManager::UpdateBullets() {
	//Iteriere durch alle Bullets
	for (unsigned i = 0; i < m_zpsBullets.GetCount(); i++) {
		CPlacement* currentBullet = m_zpsBullets.Get(i);
		CHVector currentPos = currentBullet->GetPosGlobal();
		//Wenn Bullet nicht on, resete Position
		if (currentBullet->IsOn() == false)
			lastPosBullets[i] = CHVector(0, 0, 0, 1);
		//Wenn Bullet on, aber noch keine letzte Position, setze diese
		else if (lastPosBullets[i] == CHVector(0, 0, 0, 1))
			lastPosBullets[i] = currentPos;
		//Sonst
		else {
			//Collision Detection
			CRay m_zrBulletRay;
			m_zrBulletRay.InitFromTo(lastPosBullets[i], currentPos);
			for (unsigned j = 0; j < m_collisionTargets.GetCount(); j++) {
				CPlacement* currentTarget = m_collisionTargets.Get(j);
				if (currentTarget->IsOn() && currentTarget->Intersects(m_zrBulletRay)) {
					m_zw.PrintF("%s",currentTarget->GetName());
					currentBullet->SwitchOff();
					currentTarget->GetParent()->GetName();;
				}
			}
			//Wenn Kugeln zu weit weg, switch off
			if (currentPos.Dist(m_position.GetPos()) > 1000.0f)
				currentBullet->SwitchOff();
			//Update letzte Position
			lastPosBullets[i] = currentPos;
		}
	}
}

